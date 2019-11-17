# Apps
message(STATUS "Sequence: apps")

add_executable(lss lss/src/lss.cpp)

target_include_directories(lss
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../src/sequence>)

target_link_libraries(lss PUBLIC sequence)