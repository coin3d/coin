//$ TEMPLATE MFieldDerivedValue(valueref)
//$ INSERT TEMPLATE MFieldRW()
public:
  /*! Make field contain a just the single value \a val. */
  valueref operator = (valueref val)
    { this->setValue(val); return val; }
