//$ TEMPLATE ActionClassTypeSource(_actionname_)

SoType _actionname_::classTypeId = SoType::badType();

/*!
  Returns the unique type identifier for the classname class.
*/
SoType
_actionname_::getClassTypeId(void)
{
  return classTypeId;
}

/*!
  Returns type identifier for an object.
*/
SoType
_actionname_::getTypeId(void) const
{
  return classTypeId;
}
