//$ TEMPLATE AbstractElementHeader( className )
//$ INSERT TEMPLATE AbstractElementClassTypeHeader( className )
public:
  virtual ~className(void);
  static  int getClassStackIndex(void);

  static  void initClass(void);
  static  void cleanClass(void);

protected:
          className(void);
  static  int classStackIndex;
