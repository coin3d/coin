//$ TEMPLATE DetailSource(_classname_)

SoType _classname_::classTypeId;

SoType 
_classname_::getTypeId(void) const
{
  return _classname_::classTypeId;
}

SoType 
_classname_::getClassTypeId(void)
{
  return _classname_::classTypeId;
}

void 
_classname_::initClass(void)
{
  _classname_::classTypeId = 
    SoType::createType(inherited::getClassTypeId(),
                       SbName("_classname_"));
}
