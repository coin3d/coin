//$ TEMPLATE AbstractElementClassTypeSource( className )

/*!
  \var className::classTypeId

  This is the static class type identifier for
  className.
*/

SoType className::classTypeId = SoType::badType(); // static

/*!
  This static method returns the SoType type for className.
*/

SoType
className::getClassTypeId( // static
  void )
{
  return className::classTypeId;
} // getClassTypeId()
