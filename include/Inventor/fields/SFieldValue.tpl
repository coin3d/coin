//$ TEMPLATE SFieldValue(fieldname, valuetype, valueref)
//$ INSERT TEMPLATE SFieldRW()
public:
  /*! Returns this field's value. */
  valueref getValue(void) const
    { this->evaluate(); return this->value; }
  void setValue(valueref newvalue);
  /*! Copy value from \a newvalue into this field. */
  valueref operator = (valueref newvalue)
    { this->setValue(newvalue); return this->value; }

  int operator == (const fieldname & field) const;
  /*! Returns \a TRUE if this field is not equal to \a field. */
  int operator != (const fieldname & field) const
    { return ! operator == (field); }

protected:
  /*! The value contained in the field. */
  valuetype value;
