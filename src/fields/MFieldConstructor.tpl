//$ TEMPLATE MFieldConstructor(_fieldname_)

/*!
  Constructor.
*/
_fieldname_::_fieldname_(void)
{
  this->values = NULL;
}

/*!
  Destructor.
*/
_fieldname_::~_fieldname_(void)
{
  this->deleteAllValues();
}
