//$ TEMPLATE MFieldRequired(_fieldname_)
//$ INSERT TEMPLATE FieldId(_fieldname_)
//$ INSERT TEMPLATE FieldEqSame(_fieldname_)

/*!
  Copy all field values from \a field into this object.
*/
const _fieldname_ &
_fieldname_::operator = (const _fieldname_ & field)
{
  if (field.getNum() < this->getNum()) this->deleteAllValues();
  this->setValues(0, field.getNum(), field.getValues(0));
  return *this;
}
