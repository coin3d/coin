//$ TEMPLATE SFieldRequired(_fieldname_)
//$ INSERT TEMPLATE FieldId(_fieldname_)
//$ INSERT TEMPLATE FieldEqSame(_fieldname_)

/*!
  Copy field value from \a field into this object.
*/
const _fieldname_ &
_fieldname_::operator = (const _fieldname_ & field)
{
  this->setValue(field.getValue());
  return *this;
}
