//$ TEMPLATE FieldInitClass(fieldname)
  // Make sure we only initialize once.
  assert(So##fieldname::classTypeId == SoType::badType());
  // Make sure superclass has been initialized before subclass.
  assert(inherited::getClassTypeId() != SoType::badType());

  So##fieldname::classTypeId =
    SoType::createType(So##fieldname::getClassTypeId(),
                       "fieldname", &So##fieldname::createInstance);
