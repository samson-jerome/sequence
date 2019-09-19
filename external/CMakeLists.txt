project(external C CXX)

# set(BUILD_TESTING_BCKP ${BUILD_TESTING})
# set(BUILD_TESTING OFF CACHE BOOL "Force disable of tests for external dependencies" FORCE)

# set(CUSTOM_COVERAGE_EXCLUDE ${CUSTOM_COVERAGE_EXCLUDE} "external" PARENT_SCOPE) # Replaced in CTestCustom.cmake.in


#Use fmtlib for formatting
add_subdirectory(fmt EXCLUDE_FROM_ALL)
# target_set_warnings(fmt DISABLE ALL)              # Treat fmtlib as a system include as to ignore the warnings

#Add new external (non-interface) targets to this list
set_target_properties(fmt PROPERTIES FOLDER "external")
