//$ TEMPLATE MFieldValue(_fieldname_, _valuetype_, _valueref_)

/*!
  FIXME: write function documentation
*/
int
_fieldname_::fieldSizeof(void) const
{
  return sizeof(_valuetype_);
}

/*!
  FIXME: write function documentation
*/
void *
_fieldname_::valuesPtr(void)
{
  return (void *)this->values;
}

/*!
  FIXME: write function documentation
*/
void
_fieldname_::setValuesPtr(void * ptr)
{
  this->values = (_valuetype_ *)ptr;
}

/*!
  FIXME: write function documentation
*/
int
_fieldname_::find(_valueref_ value, SbBool addIfNotFound)
{
  for (int i=0; i < this->num; i++) if (this->values[i] == value) return i;

  if (addIfNotFound) this->set1Value(this->num, value);
  return -1;
}

/*!
  FIXME: write function documentation
*/
void
_fieldname_::setValues(const int start, const int num,
                       const _valuetype_ * newValues)
{
  if (start+num > this->maxNum) this->allocValues(start+num);
  else if (start+num > this->num) this->num = start+num;

  for (int i=0; i < num; i++)
    this->values[i+start] = (_valuetype_) newValues[i];
  this->valueChanged();
}

/*!
  FIXME: write function documentation
*/
void
_fieldname_::set1Value(const int idx, _valueref_ value)
{
  if (idx+1 > this->maxNum) this->allocValues(idx+1);
  else if (idx+1 > this->num) this->num = idx+1;
  this->values[idx] = value;
  this->valueChanged();
}

/*!
  FIXME: write function documentation
*/
void
_fieldname_::setValue(_valueref_ value)
{
  this->allocValues(1);
  this->values[0] = value;
  this->valueChanged();
}

/*!
  FIXME: write function documentation
*/
SbBool
_fieldname_::operator == (const _fieldname_ & field) const
{
  if (this == &field) return TRUE;
  if (this->getNum() != field.getNum()) return FALSE;

  const _valuetype_ * const lhs = this->getValues(0);
  const _valuetype_ * const rhs = field.getValues(0);
  for (int i = 0; i < num; i++) if (lhs[i] != rhs[i]) return FALSE;
  return TRUE;
}

/*!
  FIXME: write function documentation
*/
void
_fieldname_::deleteAllValues(void)
{
  this->allocValues(0);
}

/*!
  FIXME: write function documentation
*/
void
_fieldname_::copyValue(int to, int from)
{
  assert(this->values && SbMax(to, from) < num);
  this->values[to] = this->values[from];
  this->valueChanged();
}
