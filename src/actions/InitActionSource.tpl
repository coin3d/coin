//$ TEMPLATE InitActionSource(_actionname_)
  assert(_actionname_::getClassTypeId() == SoType::badType());
  assert(inherited::getClassTypeId() != SoType::badType());

  _actionname_::classTypeId =
      SoType::createType(inherited::getClassTypeId(),
                         #_actionname_);
  enabledElements = new SoEnabledElementsList(inherited::enabledElements);
  methods = new SoActionMethodList(inherited::methods);
