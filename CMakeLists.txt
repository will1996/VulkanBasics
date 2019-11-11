cmake_minimum_required(VERSION 3.7.0 FATAL_ERROR)
project(practice VERSION 0.1.0)
set(CMAKE_CXX_STANDARD 17) 

include(CTest)
enable_testing()


add_executable(sample  VulkanSample.cpp)
add_executable(practice basicVulkan.cpp)
add_library(basicRenderer basicRender.cpp basicRender.hpp)

add_subdirectory(glfw-3.3)
find_package(glfw3 3.3 CONFIG REQUIRED)

target_link_libraries(sample glfw)
target_link_libraries(basicRenderer glfw)

find_package(Vulkan REQUIRED)
target_include_directories(sample PRIVATE Vulkan::Vulkan)
target_link_libraries(sample Vulkan::Vulkan)


target_include_directories(basicRenderer PRIVATE Vulkan::Vulkan)
target_link_libraries(basicRenderer Vulkan::Vulkan)

target_link_libraries(practice PRIVATE basicRenderer)
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
