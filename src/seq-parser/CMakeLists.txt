message("seq-parser")

# Library
add_library(seqparser parser.cpp)
# Note that headers are optional, and do not affect add_library, but they will not
# show up in IDEs unless they are listed in add_library.


message(${CMAKE_CURRENT_SOURCE_DIR})
# We need this directory, and users of our library will need it too
target_include_directories(seqparser
    PUBLIC 
        $<INSTALL_INTERFACE:../../include>    
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

# # This depends on (header only) boost
# target_link_libraries(modern_library PRIVATE Boost::boost)

# # All users of this library will need at least C++11
# target_compile_features(modern_library PUBLIC cxx_std_11)

# # IDEs should put the headers in a nice place
# source_group(TREE "${PROJECT_SOURCE_DIR}/include" PREFIX "Header Files" FILES ${HEADER_LIST})
