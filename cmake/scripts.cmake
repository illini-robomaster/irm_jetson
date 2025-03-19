add_custom_target(setup COMMENT "Run setup")
add_custom_target(setup-clean
  COMMENT "Clean setup"
)

# Custom function for setup scripts
function(irm_add_script TARGET_NAME)
  set(DEST ${CMAKE_CURRENT_BINARY_DIR}/${TARGET_NAME})

  # Create install targets
  add_custom_target(setup-${TARGET_NAME}_install
    COMMAND ${DEST} install
    COMMENT "Running ${TARGET_NAME} install noninteractively"
    VERBATIM
  )

  # Create clean targets
  add_custom_target(setup-${TARGET_NAME}_clean
    COMMAND ${DEST} clean
    COMMENT "Running ${TARGET_NAME} clean noninteractively"
    VERBATIM
  )

  add_dependencies(setup setup-${TARGET_NAME}_install)
  add_dependencies(setup-clean setup-${TARGET_NAME}_clean)
endfunction()
