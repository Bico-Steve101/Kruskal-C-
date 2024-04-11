cmake_minimum_required(VERSION 3.26)
project(Use)

set(CMAKE_CXX_STANDARD 17)

add_executable(Use main.cpp)


#find_package(Matplot++ REQUIRED)
#target_link_libraries(Use  Matplot++::matplot)
#include_directories("C:\\Program Files\\Matplot++ 1.2.0\\include")
#link_directories("C:\\Program Files\\Matplot++ 1.2.0\\lib")
#

set(GRAPHVIZ_INCLUDE_DIR "C:\\Users\\steve\\Desktop\\windows_10_msbuild_Release_graphviz-9.0.0-win32 2\\Graphviz")
include_directories(${GRAPHVIZ_INCLUDE_DIR})