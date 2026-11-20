set pagination off
set print thread-events off
set breakpoint pending on
set $cache_builds = 0
set $bbox_builds = 0
set $texture_uploads = 0
set $first_context = -1
set $second_context = -1
break SoVertexArrayTexCoordCache::SoVertexArrayTexCoordCache
commands
silent
set $cache_builds = $cache_builds + 1
continue
end
break SoDefaultTextureBBoxCache::SoDefaultTextureBBoxCache
commands
silent
set $bbox_builds = $bbox_builds + 1
continue
end
break cc_glglue_glBufferData
commands
silent
if target == 0x8892 && (size == 5408 || size == 5824)
set $texture_uploads = $texture_uploads + 1
end
continue
end
break SoVBO::bindBuffer
commands
silent
if this->target == 0x8892 && (this->datasize == 5408 || this->datasize == 5824)
if $first_context == -1
set $first_context = contextid
else
if contextid != $first_context
set $second_context = contextid
end
end
end
continue
end
run
printf "CACHE_BUILDS=%d BBOX_BUILDS=%d TEXTURE_UPLOADS=%d TWO_CONTEXTS=%d\n", $cache_builds, $bbox_builds, $texture_uploads, ($first_context != -1 && $second_context != -1 && $first_context != $second_context)
