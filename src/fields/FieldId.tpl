//$ TEMPLATE FieldId(_fieldname_)

SoType _fieldname_::classTypeId = SoType::badType();

/*!
  Virtual method which returns the type identifier for an object.

  \sa getClassTypeId()
*/
SoType
_fieldname_::getTypeId(void) const
{
  return _fieldname_::classTypeId;
}

/*!
  Returns a unique type identifier for the _fieldname_ class.

  \sa getTypeId(), SoType
 */
SoType
_fieldname_::getClassTypeId(void)
{
  return _fieldname_::classTypeId;
}

/*!
  Constructs and returns a new instance of the _fieldname_ class.
*/
void *
_fieldname_::createInstance(void)
{
  return new _fieldname_;
}
