# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.15

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/local/Cellar/cmake/3.15.3/bin/cmake

# The command to remove a file.
RM = /usr/local/Cellar/cmake/3.15.3/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/willchambers/Projects/VulkanPractice

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/willchambers/Projects/VulkanPractice/build

# Include any dependencies generated for this target.
include CMakeFiles/basicRenderer.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/basicRenderer.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/basicRenderer.dir/flags.make

CMakeFiles/basicRenderer.dir/basicRender.cpp.o: CMakeFiles/basicRenderer.dir/flags.make
CMakeFiles/basicRenderer.dir/basicRender.cpp.o: ../basicRender.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/willchambers/Projects/VulkanPractice/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/basicRenderer.dir/basicRender.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/basicRenderer.dir/basicRender.cpp.o -c /Users/willchambers/Projects/VulkanPractice/basicRender.cpp

CMakeFiles/basicRenderer.dir/basicRender.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/basicRenderer.dir/basicRender.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/willchambers/Projects/VulkanPractice/basicRender.cpp > CMakeFiles/basicRenderer.dir/basicRender.cpp.i

CMakeFiles/basicRenderer.dir/basicRender.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/basicRenderer.dir/basicRender.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/willchambers/Projects/VulkanPractice/basicRender.cpp -o CMakeFiles/basicRenderer.dir/basicRender.cpp.s

# Object files for target basicRenderer
basicRenderer_OBJECTS = \
"CMakeFiles/basicRenderer.dir/basicRender.cpp.o"

# External object files for target basicRenderer
basicRenderer_EXTERNAL_OBJECTS =

libbasicRenderer.a: CMakeFiles/basicRenderer.dir/basicRender.cpp.o
libbasicRenderer.a: CMakeFiles/basicRenderer.dir/build.make
libbasicRenderer.a: CMakeFiles/basicRenderer.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/willchambers/Projects/VulkanPractice/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX static library libbasicRenderer.a"
	$(CMAKE_COMMAND) -P CMakeFiles/basicRenderer.dir/cmake_clean_target.cmake
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/basicRenderer.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/basicRenderer.dir/build: libbasicRenderer.a

.PHONY : CMakeFiles/basicRenderer.dir/build

CMakeFiles/basicRenderer.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/basicRenderer.dir/cmake_clean.cmake
.PHONY : CMakeFiles/basicRenderer.dir/clean

CMakeFiles/basicRenderer.dir/depend:
	cd /Users/willchambers/Projects/VulkanPractice/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/willchambers/Projects/VulkanPractice /Users/willchambers/Projects/VulkanPractice /Users/willchambers/Projects/VulkanPractice/build /Users/willchambers/Projects/VulkanPractice/build /Users/willchambers/Projects/VulkanPractice/build/CMakeFiles/basicRenderer.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/basicRenderer.dir/depend

