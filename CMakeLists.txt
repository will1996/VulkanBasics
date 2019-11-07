cmake_minimum_required(VERSION 3.7.0 FATAL_ERROR)
project(practice VERSION 0.1.0)
set(CMAKE_CXX_STANDARD 17) 

include(CTest)
enable_testing()

add_executable(practice basicVulkan.cpp)

find_package(glfw3 3.3 REQUIRED)
target_link_libraries(practice glfw)

find_package(Vulkan REQUIRED)
target_include_directories(practice PRIVATE Vulkan::Vulkan)
target_link_libraries(practice Vulkan::Vulkan)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
