//$ TEMPLATE NodeClassTypeSource(classname)
SoType classname::classTypeId = SoType::badType();

/*!
  Returns a new instance of the classname node class.
*/
void *
classname::createInstance(void)
{
  return new classname;
}

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
