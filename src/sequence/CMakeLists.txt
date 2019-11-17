message(STATUS "Sequence: src/sequence")

# Library
add_library(sequence SHARED sequence.cpp collection.cpp)
# Note that headers are optional, and do not affect add_library, but they will not
# show up in IDEs unless they are listed in add_library.


# We need this directory, and users of our library will need it too
target_include_directories(sequence
    PUBLIC 
        $<INSTALL_INTERFACE:../../include>    
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../external/fmt/include>    

    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

# All users of this library will need at least C++17
# target_compile_features(seqparser PUBLIC cxx_std_17)

# # IDEs should put the headers in a nice place
# source_group(TREE "${PROJECT_SOURCE_DIR}/include" PREFIX "Header Files" FILES ${HEADER_LIST})s


# ---------------------------------------------------------------------
# Create targets
target_link_libraries (sequence PUBLIC Boost::filesystem)
target_link_libraries (sequence PRIVATE fmt::fmt)