//$ TEMPLATE NodeAbstractClassTypeImplementation(classname)
SoType classname::classTypeId = SoType::badType();

/*!
  Returns the unique type identifier for the classname class.
*/
SoType
classname::getClassTypeId(void)
{
  return classname::classTypeId;
}

/*!
  Returns type identifier for an object.
*/
SoType
classname::getTypeId(void) const
{
  return classname::classTypeId;
}
