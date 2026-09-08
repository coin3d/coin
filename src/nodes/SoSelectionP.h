#ifndef COIN_SOSELECTIONP_H
#define COIN_SOSELECTIONP_H

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

#ifndef COIN_INTERNAL
#error this is a private header file
#endif /* !COIN_INTERNAL */

#include <Inventor/nodes/SoSelection.h>
#include <Inventor/lists/SbList.h>
#include <Inventor/errors/SoDebugError.h>

// Internal helper classes, used in place of SoCallbackList for
// SoSelection's five callback lists (selCBList, deselCBList,
// startCBList, finishCBList, changeCBList).
//
// SoCallbackList stores callbacks type-erased as SoCallbackListCB
// (void(*)(void*,void*)) and invokes them through that generic type
// regardless of what a given callback was actually declared with --
// e.g. addSelectionCallback() used to cast the caller's
// SoSelectionPathCB* (void(*)(void*,SoPath*)) to SoCallbackListCB* to
// store it, and invokeCallbacks() then called back through the
// generic type. Calling a function through a function pointer of a
// type other than the one it was defined with is undefined behavior
// (caught by e.g. -fsanitize=function), even though it has always
// worked in practice on every ABI Coin supports, since void* and any
// other object/class pointer type have identical size and
// representation everywhere.
//
// These classes avoid the type mismatch altogether by never erasing
// the type in the first place: callbacks are stored and invoked with
// their real, declared type the whole way through, so there is
// nothing to cast. Mirror SoCallbackList::{addCallback,
// removeCallback, invokeCallbacks}'s documented behavior, including
// that invoking iterates over a snapshot so it remains safe for a
// callback to add or remove callbacks (including itself). Defined in
// this shared private header (rather than local to SoSelection.cpp,
// like the analogous SoDraggerCBList/SoGLPreRenderCBList) because
// SoExtSelection.cpp also invokes startCBList/finishCBList directly.
//
// selCBList/deselCBList take SoSelectionPathCB (void(*)(void*,SoPath*));
// startCBList/finishCBList/changeCBList take SoSelectionClassCB
// (void(*)(void*,SoSelection*)) -- hence two separate list classes.
class SoSelectionPathCBList {
public:
  void addCallback(SoSelectionPathCB * func, void * data) {
    this->funcs.append(func);
    this->datas.append(data);
  }
  void removeCallback(SoSelectionPathCB * func, void * data) {
    for (int i = this->funcs.getLength() - 1; i >= 0; i--) {
      if (this->funcs[i] == func && this->datas[i] == data) {
        this->funcs.remove(i);
        this->datas.remove(i);
        return;
      }
    }
#if COIN_DEBUG
    SoDebugError::post("SoSelection::removeCallback",
                        "Tried to remove non-existent callback function.");
#endif // COIN_DEBUG
  }
  void invokeCallbacks(SoPath * path) {
    SbList<SoSelectionPathCB *> funcscopy(this->funcs);
    SbList<void *> datascopy(this->datas);
    for (int i = 0; i < funcscopy.getLength(); i++) {
      funcscopy[i](datascopy[i], path);
    }
  }
private:
  SbList<SoSelectionPathCB *> funcs;
  SbList<void *> datas;
};

class SoSelectionClassCBList {
public:
  void addCallback(SoSelectionClassCB * func, void * data) {
    this->funcs.append(func);
    this->datas.append(data);
  }
  void removeCallback(SoSelectionClassCB * func, void * data) {
    for (int i = this->funcs.getLength() - 1; i >= 0; i--) {
      if (this->funcs[i] == func && this->datas[i] == data) {
        this->funcs.remove(i);
        this->datas.remove(i);
        return;
      }
    }
#if COIN_DEBUG
    SoDebugError::post("SoSelection::removeCallback",
                        "Tried to remove non-existent callback function.");
#endif // COIN_DEBUG
  }
  void invokeCallbacks(SoSelection * sel) {
    SbList<SoSelectionClassCB *> funcscopy(this->funcs);
    SbList<void *> datascopy(this->datas);
    for (int i = 0; i < funcscopy.getLength(); i++) {
      funcscopy[i](datascopy[i], sel);
    }
  }
private:
  SbList<SoSelectionClassCB *> funcs;
  SbList<void *> datas;
};

#endif // !COIN_SOSELECTIONP_H
