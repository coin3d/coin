//$ TEMPLATE MFieldConstructor(_classname_)

/*!
  Constructor.
*/
_classname_::_classname_(void)
{
  // Make sure we have initialized class.
  assert(_classname_::classTypeId != SoType::badType());
  this->values = NULL;
}

/*!
  Destructor.
*/
_classname_::~_classname_(void)
{
  this->deleteAllValues();
}
