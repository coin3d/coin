set pagination off
set print thread-events off
set breakpoint pending on
set $created = 0
set $destroyed = 0
break DefaultTextureRegistry::DefaultTextureRegistry
commands
silent
set $created = $created + 1
continue
end
break DefaultTextureRegistry::~DefaultTextureRegistry
commands
silent
set $destroyed = $destroyed + 1
continue
end
run
printf "REGISTRIES_CREATED=%d REGISTRIES_DESTROYED=%d\n", $created, $destroyed
