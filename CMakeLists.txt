cmake_minimum_required(VERSION 2.8)

project(httpservtest)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_COMPILER g++)
INCLUDE_DIRECTORIES(SYSTEM /usr/local/include )
LINK_DIRECTORIES(/usr/local/lib)
FIND_LIBRARy(FOLLY folly)
FIND_LIBRARY(WANGLE wangle)
FIND_LIBRARY(PTHREAD pthread)
find_library(GLOG glog)
find_library(GFLAGS gflags)
find_library(SSL ssl)
find_library(CRYPTO crypto)
find_library(PROXYGEN proxygenhttpserver)
find_library(DCONV double-conversion)
find_library(GTEST gtest)
find_library(GTEST_MAIN gtest_main)
set(MYLIBS
    ${FOLLY}
    ${WANGLE}
    ${PTHREAD}
    ${GLOG}
    ${GFLAGS}
    ${SSL}
    ${CRYPTO}
    ${DCONV}
    ${PROXYGEN})
set(MYLIBS_TEST
    ${GTEST}
    ${GTEST_MAIN}
    ${MYLIBS})

set(SRC myhandler.cpp timehandler.cpp servicenode.cpp serviceregistry.cpp serviceregistryhandler.cpp)
set(TEST_SRC ${SRC} test_main.cpp servicenodetest.cpp)
add_executable(${PROJECT_NAME} main.cpp ${SRC})
add_executable(${PROJECT_NAME}_test ${TEST_SRC})
target_link_libraries(${PROJECT_NAME} ${MYLIBS})
target_link_libraries(${PROJECT_NAME}_test ${MYLIBS_TEST})
