//$ TEMPLATE MFieldAlloc(_fieldname_, _valuetype_)
/*!
  Allocate \a number value slots in this field.
*/
void
_fieldname_::allocValues(int number)
{
  assert(number >= 0);

  if (number == 0) {
    delete[] this->values;
    this->values = NULL;
  }
  else {
    if (this->values) {
      // FIXME: this is extremely ineffective if allocValues() is called
      // repeatedly with a number value which is 1 higher than the last
      // (which is often the case). 990417 mortene.
      _valuetype_ * newblock = new _valuetype_[number];
      for (int i=0; i < SbMin(this->num, number); i++)
        newblock[i] = this->values[i];

      delete[] this->values;
      this->values = newblock;
    }
    else {
      this->values = new _valuetype_[number];
    }
  }

  SbBool valchanged = number < this->num ? TRUE : FALSE;
  this->num = number;
  this->maxNum = number;
  if (valchanged) this->valueChanged();
}

