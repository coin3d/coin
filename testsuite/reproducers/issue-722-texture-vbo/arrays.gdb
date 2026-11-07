set pagination off
set print thread-events off
set breakpoint pending on
set $array_calls = 0
set $vbo_calls = 0
break SoShape::startVertexArray
commands
silent
set $array_calls = $array_calls + 1
continue
end
break SoShape::finishVertexArray
commands
silent
set $vbo_calls = $vbo_calls + (vbo != 0)
continue
end
run
printf "ARRAY_CALLS=%d VBO_CALLS=%d\n", $array_calls, $vbo_calls
