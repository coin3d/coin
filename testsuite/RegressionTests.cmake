# Keep independent regression registrations in separate files so stacked PRs
# do not all edit the same insertion point in CMakeLists.txt.
file(GLOB COIN_REGRESSION_CONFIGS
  "${CMAKE_CURRENT_LIST_DIR}/regressions/*.cmake")
foreach(regression_config ${COIN_REGRESSION_CONFIGS})
  include("${regression_config}")
endforeach()
