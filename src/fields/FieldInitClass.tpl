//$ TEMPLATE FieldInitClass(_fieldname_)
  // Make sure we only initialize once.
  assert(So##_fieldname_::classTypeId == SoType::badType());
  // Make sure superclass has been initialized before subclass.
  assert(inherited::getClassTypeId() != SoType::badType());

  So##_fieldname_::classTypeId =
    SoType::createType(inherited::getClassTypeId(),
                       "_fieldname_", &So##_fieldname_::createInstance);
