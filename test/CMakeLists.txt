
# Build tests
add_executable (tests "tests_main.cpp" "common_operations.cpp" "using_collection.cpp")

# Note, using shared lib with MSVC
# https://gernotklingler.com/blog/creating-using-shared-libraries-different-compilers-different-operating-systems/
#include (GenerateExportHeader)          
#GENERATE_EXPORT_HEADER(sequence           # generates the export header shared_EXPORTS.h automatically
#    BASE_NAME sequence
#    EXPORT_MACRO_NAME sequence_EXPORTS
#    EXPORT_FILE_NAME sequence_EXPORTS.h
#    STATIC_DEFINE SHARED_EXPORTS_BUILT_AS_STATIC)

target_link_libraries (tests PRIVATE sequence::sequence)

#
add_test(
    NAME tests
    COMMAND $<TARGET_FILE:tests> --success
)