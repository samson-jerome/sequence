
# Build tests
add_executable (first first.cpp)
target_link_libraries (first sequence)

#
add_test(
    NAME first
    COMMAND $<TARGET_FILE:first> --success
)