//$ TEMPLATE ElementClassTypeSource(_classname_)

/*!
  \var _classname_::classTypeId

  This is the static class type identifier for the
  _classname_ class.
*/

SoType _classname_::classTypeId = SoType::badType();

/*!
  This method returns the SoType object for the element class of
  the instance.
*/
SoType
_classname_::getClassTypeId(void)
{
  return _classname_::classTypeId;
}

/*!
  This static method creates an object instance of the _classname_ class.
*/
void *
_classname_::createInstance(void)
{
  return (void *) new _classname_;
}
