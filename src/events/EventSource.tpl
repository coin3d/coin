//$ TEMPLATE EventSource( className )
/*!
  \var className::classTypeId
  Unique type identification for the className class type.
*/
SoType className::classTypeId = SoType::badType();

/*!
  This is a virtual function overloaded in all subclasses which will
  return the type identificator of the class instance.

  \sa getClassTypeId(), isOfType()
*/
SoType
className::getTypeId(void) const
{
  return className::classTypeId;
}

/*!
  Returns the type identificator for the className class.

  \sa getTypeId(), isOfType()
*/
SoType
className::getClassTypeId(void)
{
  return className::classTypeId;
}
