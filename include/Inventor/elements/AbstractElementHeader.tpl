//$ TEMPLATE AbstractElementHeader(_classname_)
//$ INSERT TEMPLATE AbstractElementClassTypeHeader(_classname_)
public:
  virtual ~_classname_(void);
  static  int getClassStackIndex(void);

  static void initClass(void);

protected:
  _classname_(void);
  static int classStackIndex;
