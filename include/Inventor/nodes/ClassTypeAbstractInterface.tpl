//$ TEMPLATE ClassTypeAbstractInterface( FIXME needs at least one arg FIXME )
private:
  static SoType classTypeId;

public:
  static SoType getClassTypeId(void);
  virtual SoType getTypeId(void) const;
