/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2002 by Systems in Motion.  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Systems in Motion about acquiring
 *  a Coin Professional Edition License.
 *
 *  See <URL:http://www.coin3d.org> for  more information.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY.
 *  <URL:http://www.sim.no>.
 *
\**************************************************************************/

/*!
  \class SoConcatenate SoConcatenate.h Inventor/engines/SoConcatenate.h
  \brief The SoConcatenate class is used to concatenate several inputs into one output.
  \ingroup engines

  Takes all the values from the 10 input multivalue fields in turn and
  concatenates them into the multivalue output.


  Note that this engine's output field deviates a little from the
  "standard" output mechanism of the majority of engine classes: the
  SoConcatenate::output is not a permanent SoEngineOutput instance,
  but a \e pointer to a SoEngineOutput instance.  The reason for this
  is that it is necessary to allocate the output field dynamically to
  make it match what the SoConcatenate::input is connected to since
  the type of the SoConcatenate::output always should be the same as
  the type of the SoConcatenate::input.


  \ENGINE_TYPELESS_FILEFORMAT

  \verbatim
  Concatenate {
    type <multivaluefieldtype>
    [...fields...]
  }
  \endverbatim
*/

#include <Inventor/engines/SoConcatenate.h>
#include <Inventor/engines/SoSubEngineP.h>

#include <Inventor/SbString.h>
#include <Inventor/SoInput.h>
#include <Inventor/SoOutput.h>
#include <Inventor/errors/SoReadError.h>
#include <Inventor/lists/SoEngineOutputList.h>
#include <Inventor/fields/SoFields.h>

#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG

/*!
  \var SoMField * SoConcatenate::input[10]
  The multivalue input fields which we will concatenate into the
  output.
*/
/*!
  \var SoEngineOutput * SoConcatenate::output

  (SoMField) This is the field output containing the concatenated
  values of all the input fields.

  The type of the field will of course match the type of the input
  field.
*/

// Can't use the standard SO_ENGINE_SOURCE macro, as this engine
// doesn't keep a class-global set of inputs and outputs: we need to
// make an instance of SoFieldData and SoEngineOutputData for every
// instance of the class, since the input and output fields are
// dynamically allocated.
SO_INTERNAL_ENGINE_SOURCE_DYNAMIC_IO(SoConcatenate);


/**************************************************************************/

// Default constructor. Leaves engine in invalid state. Should only be
// used from import code or copy code.
SoConcatenate::SoConcatenate(void)
{
  this->dynamicinput = NULL;
  this->dynamicoutput = NULL;
  for (int i=0; i < SoConcatenate::NUMINPUTS; i++) this->input[i] = NULL;
  this->output = NULL;
}

static SbBool
SoConcatenate_valid_type(SoType t)
{
  return (t.isDerivedFrom(SoMField::getClassTypeId()) &&
          t.canCreateInstance());
}


/*!
  Constructor. The type of the input/output is specified in \a type.
*/
SoConcatenate::SoConcatenate(SoType type)
{
  this->dynamicinput = NULL;
  this->dynamicoutput = NULL;
  for (int i=0; i < SoConcatenate::NUMINPUTS; i++) this->input[i] = NULL;
  this->output = NULL;

#if COIN_DEBUG
  if (!SoConcatenate_valid_type(type)) {
    SoDebugError::post("SoConcatenate::SoConcatenate",
                       "invalid type '%s' for input field, "
                       "field must be non-abstract and a multi-value type.",
                       type == SoType::badType() ? "badType" :
                       type.getName().getString());
  }
#endif // COIN_DEBUG

  this->initialize(type);
}


// doc from parent
void
SoConcatenate::initClass(void)
{
  SO_ENGINE_INTERNAL_INIT_CLASS(SoConcatenate);
}

// Set up the input and output fields of the engine. This is done from
// either the non-default constructor or the readInstance() import
// code.
void
SoConcatenate::initialize(const SoType inputfieldtype)
{
  assert(this->input[0] == NULL);
  assert(SoConcatenate_valid_type(inputfieldtype));

  SO_ENGINE_INTERNAL_CONSTRUCTOR(SoConcatenate);

  // Instead of SO_ENGINE_ADD_INPUT().
  this->dynamicinput = new SoFieldData(SoConcatenate::inputdata);
  for (int i=0; i < SoConcatenate::NUMINPUTS; i++) {
    this->input[i] = (SoMField *)inputfieldtype.createInstance();
    this->input[i]->setNum(0);
    this->input[i]->setContainer(this);
    SbString s = "input";
    s.addIntString(i);
    this->dynamicinput->addField(this, s.getString(), this->input[i]);
  }

  // Instead of SO_ENGINE_ADD_OUTPUT().
  this->output = new SoEngineOutput;
  this->dynamicoutput = new SoEngineOutputData(SoConcatenate::outputdata);
  this->dynamicoutput->addOutput(this, "output", this->output, inputfieldtype);
  this->output->setContainer(this);
}

/*!
  Destructor.
*/
SoConcatenate::~SoConcatenate()
{
  delete this->dynamicinput;
  delete this->dynamicoutput;

  for (int i=0; i < SoConcatenate::NUMINPUTS; i++) delete this->input[i];
  delete this->output;
}

// Documented in superclass. Overridden to initialize type of input
// before reading.
SbBool
SoConcatenate::readInstance(SoInput * in, unsigned short flags)
{
  // This code is identical to readInstance() of SoSelectOne and
  // SoGate, so migrate changes.

  SbName tmp;
  if (!in->read(tmp) || tmp != "type") {
    SoReadError::post(in,
                      "\"type\" keyword is missing, erroneous format for "
                      "engine class '%s'.",
                      this->getTypeId().getName().getString());
    return FALSE;
  }
  SbName fieldname;
  if (!in->read(fieldname)) {
    SoReadError::post(in, "Couldn't read input type for engine.");
    return FALSE;
  }
  SoType inputtype = SoType::fromName(fieldname);
  if (!SoConcatenate_valid_type(inputtype)) {
    SoReadError::post(in, "Type \"%s\" for input field is not valid "
                      "(field must be non-abstract and a multi-value type).",
                      fieldname.getString());
    return FALSE;
  }

  this->initialize(inputtype);
  return SoEngine::readInstance(in, flags);
}

// Documented in superclass. Overridden to write type of inputs.
void
SoConcatenate::writeInstance(SoOutput * out)
{
  // This code is identical to writeInstance() of SoSelectOne and
  // SoGate, so migrate changes.

  if (this->writeHeader(out, FALSE, TRUE)) return;

  SbBool binarywrite = out->isBinary();

  if (!binarywrite) out->indent();
  out->write("type");
  if (!binarywrite) out->write(' ');
  out->write(this->input[0]->getTypeId().getName());
  if (binarywrite) out->write((unsigned int)0);
  else out->write('\n');

  this->getFieldData()->write(out, this);
  this->writeFooter(out);
}

// Documented in superclass.
void
SoConcatenate::copyContents(const SoFieldContainer * from,
                            SbBool copyconnections)
{
  SoConcatenate * concatenatesrc = (SoConcatenate *)from;
  if (concatenatesrc->input[0]) { this->initialize(concatenatesrc->input[0]->getTypeId()); }
  inherited::copyContents(from, copyconnections);
}

// macro used to generate code for all mfield subclasses
#define OUTPUT_FUNC(_fieldtype_, _funcname_) \
static void _funcname_ (const int numconnections, const int inputstop, \
                        const int numvalues, SoEngineOutput * output, SoMField ** input) \
{ \
  assert(output->getConnectionType() == _fieldtype_::getClassTypeId()); \
  for (int i = 0; i < numconnections; i++) { \
    _fieldtype_ * out = (_fieldtype_*) (*output)[i]; \
    if (!out->isReadOnly()) { \
      int cnt = 0; \
      out->setNum(numvalues); \
      for (int j = 0; j <= inputstop; j++) { \
        _fieldtype_ * in = (_fieldtype_ *) input[j]; \
        assert(in != NULL); \
        out->setValues(cnt, in->getNum(), in->getValues(0)); \
        cnt += in->getNum(); \
      } \
    } \
  } \
}

OUTPUT_FUNC(SoMFBitMask, somfbitmask_out);
OUTPUT_FUNC(SoMFBool, somfbool_out);
OUTPUT_FUNC(SoMFColor, somfcolor_out);
OUTPUT_FUNC(SoMFEngine, somfengine_out);
OUTPUT_FUNC(SoMFEnum, somfenum_out);
OUTPUT_FUNC(SoMFFloat, somffloat_out);
OUTPUT_FUNC(SoMFInt32, somfint32_out);
OUTPUT_FUNC(SoMFMatrix, somfmatrix_out);
OUTPUT_FUNC(SoMFName, somfname_out);
OUTPUT_FUNC(SoMFNode, somfnode_out);
OUTPUT_FUNC(SoMFPath, somfpath_out);
OUTPUT_FUNC(SoMFPlane, somfplane_out);
OUTPUT_FUNC(SoMFRotation, somfrotation_out);
OUTPUT_FUNC(SoMFShort, somfshort_out);
OUTPUT_FUNC(SoMFString, somfstring_out);
OUTPUT_FUNC(SoMFTime, somftime_out);
OUTPUT_FUNC(SoMFUInt32, somfuint32_out);
OUTPUT_FUNC(SoMFUShort, somfushort_out);
OUTPUT_FUNC(SoMFVec2f, somfvec2f_out);
OUTPUT_FUNC(SoMFVec3f, somfvec3f_out);
OUTPUT_FUNC(SoMFVec4f, somfvec4f_out);

#undef OUTPUT_FUNC

// documented in superclass
void
SoConcatenate::evaluate(void)
{
  // we can't use SO_ENGINE_OUTPUT here, so the functionality is
  // duplicated in the for-loops.

  int i;

  // we can do this check only once
  if (!this->output->isEnabled()) return;

  int inputstop = -1; // store the last field that has at least one value
  int numvalues = 0;  // store the total number of values
  for (i = 0; i < SoConcatenate::NUMINPUTS; i++) {
    int cnt = this->input[i]->getNum();
    if (cnt) {
      numvalues += cnt;
      inputstop = i;
    }
  }
  
  const int numconnections = this->output->getNumConnections();
  const SoType type = this->output->getConnectionType();
  
  // FIXME: is it safe to use type.isOfType() instead of the ==
  // operator?  pederb, 2001-09-26
  if (type == SoMFBitMask::getClassTypeId()) {
    somfbitmask_out(numconnections, inputstop, numvalues, this->output, this->input);
  }
  else if (type == SoMFBool::getClassTypeId()) {
    somfbool_out(numconnections, inputstop, numvalues, this->output, this->input);
  }
  else if (type == SoMFColor::getClassTypeId()) {
    somfcolor_out(numconnections, inputstop, numvalues, this->output, this->input);
  }
  else if (type == SoMFEngine::getClassTypeId()) {
    somfengine_out(numconnections, inputstop, numvalues, this->output, this->input);
  }
  else if (type == SoMFEnum::getClassTypeId()) {
    somfenum_out(numconnections, inputstop, numvalues, this->output, this->input);
  }
  else if (type == SoMFFloat::getClassTypeId()) {
    somffloat_out(numconnections, inputstop, numvalues, this->output, this->input);
  }
  else if (type == SoMFInt32::getClassTypeId()) {
    somfint32_out(numconnections, inputstop, numvalues, this->output, this->input);
  }
  else if (type == SoMFMatrix::getClassTypeId()) {
    somfmatrix_out(numconnections, inputstop, numvalues, this->output, this->input);
  }
  else if (type == SoMFName::getClassTypeId()) {
    somfname_out(numconnections, inputstop, numvalues, this->output, this->input);
  }
  else if (type == SoMFNode::getClassTypeId()) {
    somfnode_out(numconnections, inputstop, numvalues, this->output, this->input);
  }
  else if (type == SoMFPath::getClassTypeId()) {
    somfpath_out(numconnections, inputstop, numvalues, this->output, this->input);
  }
  else if (type == SoMFPlane::getClassTypeId()) {
    somfplane_out(numconnections, inputstop, numvalues, this->output, this->input);
  }
  else if (type == SoMFRotation::getClassTypeId()) {
    somfrotation_out(numconnections, inputstop, numvalues, this->output, this->input);
  }
  else if (type == SoMFShort::getClassTypeId()) {
    somfshort_out(numconnections, inputstop, numvalues, this->output, this->input);
  }
  else if (type == SoMFString::getClassTypeId()) {
    somfstring_out(numconnections, inputstop, numvalues, this->output, this->input);
  }
  else if (type == SoMFTime::getClassTypeId()) {
    somftime_out(numconnections, inputstop, numvalues, this->output, this->input);
  }
  else if (type == SoMFUInt32::getClassTypeId()) {
    somfuint32_out(numconnections, inputstop, numvalues, this->output, this->input);
  }
  else if (type == SoMFUShort::getClassTypeId()) {
    somfushort_out(numconnections, inputstop, numvalues, this->output, this->input);
  }
  else if (type == SoMFVec2f::getClassTypeId()) {
    somfvec2f_out(numconnections, inputstop, numvalues, this->output, this->input);
  }
  else if (type == SoMFVec3f::getClassTypeId()) {
    somfvec3f_out(numconnections, inputstop, numvalues, this->output, this->input);
  }
  else if (type == SoMFVec4f::getClassTypeId()) {
    somfvec4f_out(numconnections, inputstop, numvalues, this->output, this->input);
  }  
  else {     
    // fallback, in case of new (user-defined) field types. Warn once
    // in case we forget to add code for some field.

#if COIN_DEBUG
    static int first = 1;
    if (first) {
      first = 0;
      SoDebugError::postWarning("SoConcatenate::evaluate",
                                "Unknown field type: %s.\n  Using unoptimized method to copy values.",
                                this->output->getConnectionType().getName().getString());
  }
#endif

    SbString value;
    for (i = 0; i < numconnections; i++) {
      SoMField * out = (SoMField*) (*this->output)[i];
      if (!out->isReadOnly()) {
        int cnt = 0;
        out->setNum(numvalues);
        for (int j = 0; j <= inputstop; j++) {
          SoMField * in = this->input[j];
          const int num = in->getNum();  
          for (int k = 0; k < num; k++) {
            in->get1(k, value);
            out->set1(cnt, value.getString());
            cnt++;
          }
        }
      }
    } 
  }
}
