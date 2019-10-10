
file(GLOB SRCS *.cpp)

ADD_EXECUTABLE(first ${SRCS})

# Very unpleasently adding all path of all include chain
# should we use include in cc files rather than header and use forward declaration ?
target_include_directories(first
    PUBLIC 
        $<INSTALL_INTERFACE:../../include>    
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../include>
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../src/seq-parser>
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../external/fmt/include>    

        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../build/googletest-src/googletest/include>

    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

TARGET_LINK_LIBRARIES(first
    seqparser
    gtest
    gmock
)

add_test(NAME first
         COMMAND first)