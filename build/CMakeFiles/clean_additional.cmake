# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\StudentAssociationManagementSystem_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\StudentAssociationManagementSystem_autogen.dir\\ParseCache.txt"
  "StudentAssociationManagementSystem_autogen"
  )
endif()
