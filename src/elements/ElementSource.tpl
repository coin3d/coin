//$ TEMPLATE ElementSource(_classname_)
//$ INSERT TEMPLATE ElementClassTypeSource(_classname_)

/*!
  \var _classname_::classStackIndex

  This is the static state stack index for the
  _classname_ class.
*/
int _classname_::classStackIndex;

/*!
  This static method returns the state stack index for the _classname_ class.
*/
int
_classname_::getClassStackIndex(void)
{
  return _classname_::classStackIndex;
}
