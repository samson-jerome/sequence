
# Build tests
add_executable (tests tests_main.cpp common_operations.cpp)
target_link_libraries (tests sequence)

#
add_test(
    NAME tests
    COMMAND $<TARGET_FILE:tests> --success
)