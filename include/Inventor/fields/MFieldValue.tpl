//$ TEMPLATE MFieldValue(fieldname, valuetype, valueref)
//$ INSERT TEMPLATE MFieldRW()
public:
  /*! Returns the element at the \a idx position. */
  valueref operator [] (const int idx) const
    { this->evaluate(); return this->values[idx]; }
  /*! Returns a pointer to an array of element starting at the \a start position. */
  const valuetype * getValues(const int start) const
    { this->evaluate(); return (const valuetype *)(this->values + start); }
  int find(valueref value, SbBool addIfNotFound = FALSE);
  void setValues(const int start, const int num, const valuetype * values);
  void set1Value(const int idx, valueref value);
  void setValue(valueref value);
  /*! Make field contain a just the single value \a val. */
  valueref operator = (valueref val)
    { this->setValue(val); return val; }
  SbBool operator == (const fieldname & field) const;
  /*! Returns \a TRUE if this field is \e not equal to \a field. */
  SbBool operator != (const fieldname & field) const
    { return ! operator == (field); }
  /*! Returns a pointer to the array of values for editing. \e Must be matched
      with a call to finishEditing() upon completion. */
  valuetype * startEditing(void)
    { this->evaluate(); return this->values; }
  /*! Call this method to notify Coin that you're through editing the data. */
  void finishEditing(void)
    { this->valueChanged(); }

protected:
  virtual void deleteAllValues(void);
  virtual void copyValue(int to, int from);
  virtual int fieldSizeof(void) const;
  virtual void * valuesPtr(void);
  virtual void setValuesPtr(void * ptr);

  /*! Pointer to array of values. */
  valuetype * values;
