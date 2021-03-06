cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(shape2D-triangulator)

include_directories(${CMAKE_SOURCE_DIR}/include)

set(SRC_DIR "${CMAKE_SOURCE_DIR}/src")
set(SRCS 
    main.cpp
    ${SRC_DIR}/triangulator.cpp
)
set(SRCS_TEST 
    testmain.cpp
    ${SRC_DIR}/triangulator.cpp
    ${SRC_DIR}/tests.cpp
)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/CMake")
find_package(CppUnit REQUIRED)
include_directories(${CPPUNIT_INCLUDE_DIR})
LIST(APPEND UnitTestLibs ${CPPUNIT_LIBRARY})

add_executable(main ${SRCS})
add_executable(testmain testmain.cpp ${SRCS_TEST})
target_link_libraries(testmain ${CPPUNIT_LIBRARY})