//$ TEMPLATE InitActionSource( actionName )
  assert(actionName::getClassTypeId() == SoType::badType());
  assert(inherited::getClassTypeId() != SoType::badType());

  actionName::classTypeId = 
      SoType::createType(inherited::getClassTypeId(),
                         #actionName);
  enabledElements = new SoEnabledElementsList(inherited::enabledElements);
  methods = new SoActionMethodList(inherited::methods);
