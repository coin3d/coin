//$ TEMPLATE DetailSource(_classname_)

SoType _classname_::classTypeId;

SoType 
_classname_::getTypeId() const
{
  return _classname_::classTypeId;
}

SoType 
_classname_::getClassTypeId()
{
  return _classname_::classTypeId;
}

void 
_classname_::initClass()
{
  _classname_::classTypeId = 
    SoType::createType(inherited::getClassTypeId(),
                       SbName("_classname_"));
}
