GENERATE_ROOT_TEST_SCRIPT(${CMAKE_SOURCE_DIR}/example/Tutorial8/macros/unpack_mbs.C)
add_test(unpack_mbs
         ${CMAKE_BINARY_DIR}/example/Tutorial8/macros/unpack_mbs.sh)
SET_TESTS_PROPERTIES(unpack_mbs PROPERTIES TIMEOUT "30")
SET_TESTS_PROPERTIES(unpack_mbs PROPERTIES PASS_REGULAR_EXPRESSION "Macro finished successfully")

