//$ TEMPLATE AbstractElementSource( className )
//$ INSERT TEMPLATE AbstractElementClassTypeSource( className )

/*!
  \var className::classStackIndex

  This is the static state stack index for the
  className class.
*/

int className::classStackIndex; // static

/*!
  This static method returns the state stack index for the className class.
*/

int
className::getClassStackIndex( // static
  void )
{
  return className::classStackIndex;
}
