//$ TEMPLATE SField(_classname_, _valueref_)
//$ INSERT TEMPLATE SFieldRequired(_classname_)

/*!
  Constructor.
*/
_classname_::_classname_(void)
{
}

/*!
  Destructor.
*/
_classname_::~_classname_()
{
}

/*!
  Set this field's value.

  \sa getValue()
*/
void
_classname_::setValue(_valueref_ value)
{
  this->value = value;
  // FIXME: how about doing a new vs old comparison here? 19990620 mortene.
  this->valueChanged();
}

/*!
  Returns \a TRUE if this field is equal to \a field.
*/
SbBool
_classname_::operator == (const _classname_ & field) const
{
  return (this->getValue() == field.getValue());
}
