//$ TEMPLATE Field(_fieldname_)
private:
  static SoType classTypeId;

public:
  static void * createInstance(void);
  static SoType getClassTypeId(void);
  virtual SoType getTypeId(void) const;

  static void initClass(void);

  virtual void copyFrom(const SoField & field);
  const _fieldname_ & operator = (const _fieldname_ & field);
  virtual SbBool isSame(const SoField & field) const;
