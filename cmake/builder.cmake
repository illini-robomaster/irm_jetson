# Custom function to create executable and run target
function(add_custom_executable TARGET_NAME)
# Parse arguments
  set(options VERBOSE)
  set(oneValueArgs "")
  set(multiValueArgs SOURCES)
  cmake_parse_arguments(ARG "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

# Create executable
  add_executable(${TARGET_NAME})
  target_sources(${TARGET_NAME} PRIVATE ${ARG_SOURCES})
  target_include_directories(${TARGET_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/src/include)

# Create run target
  add_custom_target(run-${TARGET_NAME}
      COMMAND ${TARGET_NAME}
      DEPENDS ${TARGET_NAME}
      COMMENT "Running ${TARGET_NAME} executable"
  )
endfunction()
