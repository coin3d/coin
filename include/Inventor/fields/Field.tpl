//$ TEMPLATE Field(fieldname)
private:
  static SoType classTypeId;

public:
  static void * createInstance(void);
  static SoType getClassTypeId(void);
  virtual SoType getTypeId(void) const;

  static void initClass(void);
  static void cleanClass(void);

  virtual void copyFrom(const SoField & field);
  const fieldname & operator = (const fieldname & field);
  virtual SbBool isSame(const SoField & field) const;
