/**************************************************************************\
 * Copyright (c) Kongsberg Oil & Gas Technologies AS
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 * 
 * Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 * 
 * Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 * 
 * Neither the name of the copyright holder nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
\**************************************************************************/

/*!
  \class SoCallbackList SoCallbackList.h Inventor/lists/SoCallbackList.h
  \brief The SoCallbackList is a container for callback function pointers.

  \ingroup coin_general

  This list stores callback function pointers (along with
  user-specified extra data to pass to the callbacks) and provides a
  method for triggering the callback functions.
*/

#include <Inventor/lists/SoCallbackList.h>

#include "lists/SoCallbackListP.h"
#include "misc/SbHash.h"

#include <atomic>
#include <memory>
#include <mutex>
#include <vector>

inline unsigned int
SbHashFunc(const SbPList * key) noexcept
{
  return SbHashFunc(reinterpret_cast<size_t>(key));
}

namespace {
std::atomic<bool> haveOwnedData(false);

struct OwnedCallback {
  int index;
  SoCallbackListCB * invoke;
  std::shared_ptr<void> data;
};
typedef std::vector<OwnedCallback> OwnedCallbacks;

struct CallbackOwners {
  std::mutex mutex;
  SbHash<const SbPList *, OwnedCallbacks> lists;
};

CallbackOwners & callbackOwners()
{
  // Keep the registry itself available during static/Coin shutdown.
  // Per-list entries are removed as soon as their ownership ends.
  static CallbackOwners * owners = new CallbackOwners;
  return *owners;
}

OwnedCallbacks snapshotOwners(const SbPList * list)
{
  if (!haveOwnedData.load()) return OwnedCallbacks();
  CallbackOwners & owners = callbackOwners();
  std::lock_guard<std::mutex> lock(owners.mutex);
  OwnedCallbacks entries;
  owners.lists.get(list, entries);
  return entries;
}

void releaseOwner(const SbPList * list, int index)
{
  // Destroy userdata after unlocking: a deleter may itself use a list.
  if (!haveOwnedData.load()) return;
  std::shared_ptr<void> removed;
  CallbackOwners & owners = callbackOwners();
  {
    std::lock_guard<std::mutex> lock(owners.mutex);
    OwnedCallbacks entries;
    if (!owners.lists.get(list, entries)) return;
    for (size_t i = 0; i < entries.size();) {
      if (entries[i].index == index) {
        removed = entries[i].data;
        entries.erase(entries.begin() + i);
      }
      else {
        if (entries[i].index > index) --entries[i].index;
        ++i;
      }
    }
    if (entries.empty()) owners.lists.erase(list);
    else owners.lists.put(list, entries);
    haveOwnedData.store(owners.lists.getNumElements() != 0);
  }
}

void releaseOwners(const SbPList * list)
{
  if (!haveOwnedData.load()) return;
  OwnedCallbacks removed;
  CallbackOwners & owners = callbackOwners();
  {
    std::lock_guard<std::mutex> lock(owners.mutex);
    if (!owners.lists.get(list, removed)) return;
    owners.lists.erase(list);
    haveOwnedData.store(owners.lists.getNumElements() != 0);
  }
}
} // namespace

void
SoCallbackListP::clearData(const SbPList * list)
{
  releaseOwners(list);
}

void
SoCallbackListP::copyData(const SbPList * source, const SbPList * destination)
{
  if (!haveOwnedData.load()) return;
  OwnedCallbacks previous;
  CallbackOwners & owners = callbackOwners();
  {
    std::lock_guard<std::mutex> lock(owners.mutex);
    OwnedCallbacks sourceentries;
    const bool hassource = owners.lists.get(source, sourceentries);
    const bool hasdestination = owners.lists.get(destination, previous);
    if (hassource && hasdestination) {
      // The new snapshot is complete before changing the old entry.
      owners.lists[destination].swap(sourceentries);
    }
    else if (hassource) {
      try {
        owners.lists.put(destination, sourceentries);
      }
      catch (...) {
        // put() can insert before a later bucket resize throws.
        owners.lists.erase(destination);
        throw;
      }
    }
    else if (hasdestination) owners.lists.erase(destination);
    haveOwnedData.store(owners.lists.getNumElements() != 0);
  }
}

void
SoCallbackListP::swapData(const SbPList * lhs, const SbPList * rhs)
{
  if (lhs == rhs || !haveOwnedData.load()) return;
  CallbackOwners & owners = callbackOwners();
  std::lock_guard<std::mutex> lock(owners.mutex);
  OwnedCallbacks ignored;
  const bool haslhs = owners.lists.get(lhs, ignored);
  const bool hasrhs = owners.lists.get(rhs, ignored);
  if (!haslhs && !hasrhs) return;

  // Materialize any missing key before changing either existing value.
  // Roll it back if SbHash inserts and then fails while resizing.
  const SbPList * missing = haslhs ? rhs : lhs;
  if (!haslhs || !hasrhs) {
    try {
      owners.lists.put(missing, OwnedCallbacks());
    }
    catch (...) {
      owners.lists.erase(missing);
      throw;
    }
  }
  owners.lists[lhs].swap(owners.lists[rhs]);
  if (owners.lists[lhs].empty()) owners.lists.erase(lhs);
  if (owners.lists[rhs].empty()) owners.lists.erase(rhs);
  haveOwnedData.store(owners.lists.getNumElements() != 0);
}

void
SoCallbackListP::swapLists(SbPList * lhs, SbPList * rhs)
{
  if (lhs == rhs) return;
  const bool lhsbuiltin = lhs->itembuffer == lhs->builtinbuffer;
  const bool rhsbuiltin = rhs->itembuffer == rhs->builtinbuffer;
  if (!lhsbuiltin && !rhsbuiltin) {
    int size = lhs->itembuffersize;
    lhs->itembuffersize = rhs->itembuffersize;
    rhs->itembuffersize = size;
    int length = lhs->numitems;
    lhs->numitems = rhs->numitems;
    rhs->numitems = length;
    void ** buffer = lhs->itembuffer;
    lhs->itembuffer = rhs->itembuffer;
    rhs->itembuffer = buffer;
  }
  else if (lhsbuiltin && rhsbuiltin) {
    const int lhslength = lhs->numitems;
    const int rhslength = rhs->numitems;
    const int minitems = lhslength < rhslength ? lhslength : rhslength;
    for (int i = 0; i < minitems; ++i) {
      void * item = lhs->builtinbuffer[i];
      lhs->builtinbuffer[i] = rhs->builtinbuffer[i];
      rhs->builtinbuffer[i] = item;
    }
    for (int i = minitems; i < lhslength; ++i)
      rhs->builtinbuffer[i] = lhs->builtinbuffer[i];
    for (int i = minitems; i < rhslength; ++i)
      lhs->builtinbuffer[i] = rhs->builtinbuffer[i];
    lhs->numitems = rhslength;
    rhs->numitems = lhslength;
  }
  else {
    SbPList * dynamiclist = lhsbuiltin ? rhs : lhs;
    SbPList * builtinlist = lhsbuiltin ? lhs : rhs;
    for (int i = 0; i < builtinlist->numitems; ++i)
      dynamiclist->builtinbuffer[i] = builtinlist->builtinbuffer[i];
    void ** buffer = dynamiclist->itembuffer;
    const int size = dynamiclist->itembuffersize;
    const int length = dynamiclist->numitems;
    dynamiclist->itembuffer = dynamiclist->builtinbuffer;
    dynamiclist->itembuffersize = SbPList::DEFAULTSIZE;
    dynamiclist->numitems = builtinlist->numitems;
    builtinlist->itembuffer = buffer;
    builtinlist->itembuffersize = size;
    builtinlist->numitems = length;
  }
}

void
SoCallbackListP::reserveCallback(SoCallbackList * list)
{
  // Grow both arrays before either logical list changes length. A failed
  // allocation may change capacity, but leaves the callback pairs intact.
  const int numfuncs = list->funclist.getLength();
  list->funclist.append(NULL);
  list->funclist.truncate(numfuncs);

  const int numdata = list->datalist.getLength();
  list->datalist.append(NULL);
  list->datalist.truncate(numdata);
}

void
SoCallbackListP::addCallback(SoCallbackList * list, SoCallbackListCB * identity,
                             void * userdata, SoCallbackListCB * invoke,
                             void * context, void (*destroy)(void *))
{
  OwnedCallback entry = { list->getNumCallbacks(), invoke,
                         std::shared_ptr<void>(context, destroy) };
  reserveCallback(list);
  CallbackOwners & owners = callbackOwners();
  {
    std::lock_guard<std::mutex> lock(owners.mutex);
    OwnedCallbacks staged;
    const bool existing = owners.lists.get(&list->datalist, staged);
    staged.push_back(entry);
    if (existing) {
      // The entry is known to exist, so operator[] does not allocate.
      owners.lists[&list->datalist].swap(staged);
    }
    else {
      try {
        owners.lists.put(&list->datalist, staged);
      }
      catch (...) {
        // SbHash::put may throw while resizing after it inserts the entry.
        owners.lists.erase(&list->datalist);
        throw;
      }
    }
  }
  // Both SbPLists have room; these appends cannot allocate.
  list->funclist.append((void *) identity);
  list->datalist.append(userdata);
  haveOwnedData.store(true);
}

#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG

/*!
  \typedef void SoCallbackListCB(void * userdata, void * callbackdata)

  The type definition for callback functions. The \a userdata is supplied
  alongside the function pointer when it is added to the list. When the
  callback is invoked the specific \a userdata for that function is supplied,
  as well as the \a callbackdata that is sent to all functions invoked.
*/

/*!
  Default constructor.
*/
SoCallbackList::SoCallbackList(void)
{
}

/*!
  Copy constructor. Both callback arrays and their ownership registry are
  committed only after the members have been fully constructed. Recompile
  clients to use this out-of-line copy instead of the former implicit one.
*/
SoCallbackList::SoCallbackList(const SoCallbackList & list)
{
  this->funclist.copy(list.funclist);
  this->datalist.copy(list.datalist);
}

/*!
  Destructor.
*/
SoCallbackList::~SoCallbackList(void)
{
  releaseOwners(&this->datalist);
}

/*!
  Assignment keeps both operands intact if the replacement copy or registry
  preparation throws. Existing clients need recompilation for this method.
*/
SoCallbackList &
SoCallbackList::operator=(const SoCallbackList & list)
{
  if (this == &list) return *this;
  SoCallbackList replacement(list);
  SoCallbackListP::swapData(&this->datalist, &replacement.datalist);
  SoCallbackListP::swapLists(&this->funclist, &replacement.funclist);
  SoCallbackListP::swapLists(&this->datalist, &replacement.datalist);
  return *this;
}

/*!
  Append the callback function \a f to the list. It will be passed the
  \a userdata upon invocation.
*/
void
SoCallbackList::addCallback(SoCallbackListCB * f, void * userdata)
{
  // FIXME: Shouldn't we check if the callback is already in the list?
  // 20050723 kyrah.
  SoCallbackListP::reserveCallback(this);
  this->funclist.append((void*)f);
  this->datalist.append(userdata);
}

/*!
  Remove the last registration matching callback \a f and \a userdata.
*/
void
SoCallbackList::removeCallback(SoCallbackListCB * f, void * userdata)
{
  int idx = this->getNumCallbacks() - 1;

  // FIXME: Why are we not using a dictionary here? (The question is
  // of course whether it should be allowed to have the same callback
  // entry in the list twice...) 20050723 kyrah.
  while (idx != -1) {
    if ((this->funclist[idx] == (void*)f) && (this->datalist[idx] == userdata)) {
      this->funclist.remove(idx);
      this->datalist.remove(idx);
      releaseOwner(&this->datalist, idx);
      break;
    }
    idx--;
  }

#if COIN_DEBUG
  // FIXME: Is this warning really necessary? Shouldn't it be possible
  // to do the equivalent of setCallback(NULL,NULL) -- i.e. "remove if
  // already exists, else do nothing"? 20050723 kyrah.
  if (idx == -1) {
    SoDebugError::post("SoCallbackList::removeCallback",
                       "Tried to remove non-existent callback function.");
    return;
  }
#endif // COIN_DEBUG
}

/*!
  Remove all callbacks in the list.
*/
void
SoCallbackList::clearCallbacks(void)
{
  this->funclist.truncate(0);
  this->datalist.truncate(0);
  releaseOwners(&this->datalist);
}

/*!
  Returns number of callback functions.
*/
int
SoCallbackList::getNumCallbacks(void) const
{
  return this->funclist.getLength();
}

/*!
  Invoke all callback functions, passing the userdata and the \a
  callbackdata as the first and second argument, respectively.

  All callbacks registered when the method is invoked will be
  triggered, even though if the code in one callback removes another
  callback.

  It is safe for a callback to remove itself or any other callbacks,
  clear the list, or add callbacks during execution. Callbacks added during
  execution are first eligible for a subsequent invocation. Each nested
  invocation takes its own snapshot of the registrations then present.
*/
void
SoCallbackList::invokeCallbacks(void * callbackdata)
{
  const OwnedCallbacks owners = snapshotOwners(&this->datalist);
  SbPList flcopy(this->funclist);
  SbPList dlcopy(this->datalist);

  // Owned entries are ordered by registration index. Walk both snapshots
  // once, so dispatch remains linear even with mixed raw/typed callbacks.
  size_t ownedidx = 0;
  for (int idx=0; idx < flcopy.getLength(); idx++) {
    if (ownedidx < owners.size() && owners[ownedidx].index == idx) {
      const OwnedCallback & entry = owners[ownedidx++];
      entry.invoke(entry.data.get(), callbackdata);
    }
    else {
      SoCallbackListCB * func = (SoCallbackListCB*) flcopy[idx];
      func(dlcopy.operator[](idx), callbackdata);
    }
  }
}

#ifdef COIN_TEST_SUITE

#include <Inventor/nodes/SoSelection.h>
#include <Inventor/SbString.h>

class CallbackListTestSelection : public SoSelection {
public:
  SoCallbackList & callbacks() { return *this->changeCBList; }
  void fire() { this->changeCBList->invokeCallbacks(this); }
};

static void callbacklist_typed(void * data, SoSelection *)
{
  *static_cast<SbString *>(data) += "T";
}

static void callbacklist_generic(void * data, void *)
{
  *static_cast<SbString *>(data) += "G";
}

BOOST_AUTO_TEST_CASE(typed_callback_copies_preserve_dispatch_and_lifetime)
{
  SbString trace;
  CallbackListTestSelection * selection = new CallbackListTestSelection;
  selection->ref();
  selection->addChangeCallback(callbacklist_typed, &trace);
  selection->callbacks().addCallback(callbacklist_generic, &trace);
  {
    SoCallbackList copy(selection->callbacks());
    SoCallbackList assigned;
    assigned = copy;
    assigned = assigned;
    selection->callbacks().clearCallbacks();
    selection->unref();
    copy.invokeCallbacks(NULL);
    BOOST_CHECK(trace == "TG");
    copy.clearCallbacks();
    assigned.invokeCallbacks(NULL);
    BOOST_CHECK(trace == "TGTG");

    // Overwriting an owned list with a raw list must discard its adapters.
    SoCallbackList raw;
    raw.addCallback(callbacklist_generic, &trace);
    assigned = raw;
    assigned.invokeCallbacks(NULL);
    BOOST_CHECK(trace == "TGTGG");
    assigned.clearCallbacks();
    BOOST_CHECK_EQUAL(assigned.getNumCallbacks(), 0);
  }
}

BOOST_AUTO_TEST_CASE(mixed_callback_removal_preserves_indices_and_last_duplicate)
{
  SbString trace;
  CallbackListTestSelection * selection = new CallbackListTestSelection;
  selection->ref();
  selection->addChangeCallback(callbacklist_typed, &trace);
  selection->callbacks().addCallback(callbacklist_generic, &trace);
  selection->addChangeCallback(callbacklist_typed, &trace);
  selection->removeChangeCallback(callbacklist_typed, &trace);
  selection->fire();
  BOOST_CHECK(trace == "TG"); // Removing the first duplicate would produce GT.
  selection->callbacks().removeCallback(callbacklist_generic, &trace);
  selection->fire();
  BOOST_CHECK(trace == "TGT");
  selection->callbacks().removeCallback(
    reinterpret_cast<SoCallbackListCB *>(callbacklist_typed), &trace);
  selection->fire();
  BOOST_CHECK(trace == "TGT");
  BOOST_CHECK_EQUAL(selection->callbacks().getNumCallbacks(), 0);
  selection->unref();
}

struct CallbackListMutation {
  SbString trace;
  bool changed;
};

static void callbacklist_later(void * data, SoSelection *)
{
  static_cast<CallbackListMutation *>(data)->trace += "B";
}

static void callbacklist_added(void * data, SoSelection *)
{
  static_cast<CallbackListMutation *>(data)->trace += "C";
}

static void callbacklist_mutate(void * data, SoSelection * node)
{
  CallbackListMutation * state = static_cast<CallbackListMutation *>(data);
  state->trace += "A";
  if (!state->changed) {
    state->changed = true;
    node->removeChangeCallback(callbacklist_later, data);
    node->addChangeCallback(callbacklist_added, data);
  }
}

static void callbacklist_clear_and_reenter(void * data, SoSelection * node)
{
  CallbackListMutation * state = static_cast<CallbackListMutation *>(data);
  state->trace += "D";
  CallbackListTestSelection * selection = static_cast<CallbackListTestSelection *>(node);
  selection->callbacks().clearCallbacks();
  selection->fire();
}

BOOST_AUTO_TEST_CASE(callback_mutations_affect_only_later_invocations)
{
  CallbackListMutation state;
  state.changed = false;
  CallbackListTestSelection * selection = new CallbackListTestSelection;
  selection->ref();
  selection->addChangeCallback(callbacklist_mutate, &state);
  selection->addChangeCallback(callbacklist_later, &state);
  selection->fire();
  BOOST_CHECK(state.trace == "AB");
  selection->fire();
  BOOST_CHECK(state.trace == "ABAC");
  selection->callbacks().clearCallbacks();
  selection->addChangeCallback(callbacklist_clear_and_reenter, &state);
  selection->addChangeCallback(callbacklist_later, &state);
  selection->fire();
  BOOST_CHECK(state.trace == "ABACDB");
  selection->fire();
  BOOST_CHECK(state.trace == "ABACDB");
  selection->unref();
}

#endif // COIN_TEST_SUITE
