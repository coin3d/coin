//$ TEMPLATE InitElementSource( className )
  assert(className::classTypeId == SoType::badType());
  assert(inherited::getClassTypeId() != SoType::badType());

  className::classTypeId = SoType::createType(
    inherited::getClassTypeId(),
    #className,
    &className::createInstance);

  if (inherited::classStackIndex < 0) {
    className::classStackIndex =
      createStackIndex( className::classTypeId );
  } else {
    className::classStackIndex =
      inherited::classStackIndex;
  }
