//$ TEMPLATE DetailHeader(_classname_)
public:
  virtual SoType getTypeId() const;
  static SoType getClassTypeId();
  static void initClass();
private:
  static SoType classTypeId;
