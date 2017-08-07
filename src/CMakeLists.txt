ADD_LIBRARY(
  fulldict
  indicator.c
  full.c
  types.c
  util.c
)
INSTALL(
  DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../include/full-dict
  DESTINATION include
  FILES_MATCHING PATTERN "*.h"
)
INSTALL(
  TARGETS fulldict
  ARCHIVE
  DESTINATION lib
)
