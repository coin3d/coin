//$ TEMPLATE SFieldDerivedValue(valueref)
//$ INSERT TEMPLATE SFieldRW()
public:
  valueref operator = (const valueref newvalue)
    { this->setValue(newvalue); return this->value; }
