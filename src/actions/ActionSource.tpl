//$ TEMPLATE ActionSource(_actionname_)
//$ INSERT TEMPLATE ActionClassTypeSource(_actionname_)

// static variables
SoEnabledElementsList * _actionname_::enabledElements;
SoActionMethodList * _actionname_::methods;

/*!
  \fn _actionname_::enabledElements
  FIXME: write doc.
*/

/*!
  \fn _actionname_::methods
  FIXME: write doc.
*/

/*!
  This method returns the list of enabled elements for the given action class.
*/
const SoEnabledElementsList &
_actionname_::getEnabledElements(void) const
{
  return *enabledElements;
}

/*!
  This method adds a method to be perfomed by the action class on the given
  node type.
*/
void 
_actionname_::addMethod(const SoType type, SoActionMethod method)
{
  methods->addMethod(type, method);
}

/*!
  This method enables an element in the state stack for the action class.
*/
void 
_actionname_::enableElement(const SoType type, const int stackIndex)
{
  enabledElements->enable(type, stackIndex);
}
