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
include glfw-3.3/examples/CMakeFiles/simple.dir/depend.make

# Include the progress variables for this target.
include glfw-3.3/examples/CMakeFiles/simple.dir/progress.make

# Include the compile flags for this target's objects.
include glfw-3.3/examples/CMakeFiles/simple.dir/flags.make

glfw-3.3/examples/simple.app/Contents/Resources/glfw.icns: ../glfw-3.3/examples/glfw.icns
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Copying OS X content glfw-3.3/examples/simple.app/Contents/Resources/glfw.icns"
	$(CMAKE_COMMAND) -E copy /Users/willchambers/Projects/VulkanPractice/glfw-3.3/examples/glfw.icns glfw-3.3/examples/simple.app/Contents/Resources/glfw.icns

glfw-3.3/examples/CMakeFiles/simple.dir/simple.c.o: glfw-3.3/examples/CMakeFiles/simple.dir/flags.make
glfw-3.3/examples/CMakeFiles/simple.dir/simple.c.o: ../glfw-3.3/examples/simple.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/willchambers/Projects/VulkanPractice/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object glfw-3.3/examples/CMakeFiles/simple.dir/simple.c.o"
	cd /Users/willchambers/Projects/VulkanPractice/build/glfw-3.3/examples && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/simple.dir/simple.c.o   -c /Users/willchambers/Projects/VulkanPractice/glfw-3.3/examples/simple.c

glfw-3.3/examples/CMakeFiles/simple.dir/simple.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/simple.dir/simple.c.i"
	cd /Users/willchambers/Projects/VulkanPractice/build/glfw-3.3/examples && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/willchambers/Projects/VulkanPractice/glfw-3.3/examples/simple.c > CMakeFiles/simple.dir/simple.c.i

glfw-3.3/examples/CMakeFiles/simple.dir/simple.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/simple.dir/simple.c.s"
	cd /Users/willchambers/Projects/VulkanPractice/build/glfw-3.3/examples && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/willchambers/Projects/VulkanPractice/glfw-3.3/examples/simple.c -o CMakeFiles/simple.dir/simple.c.s

glfw-3.3/examples/CMakeFiles/simple.dir/__/deps/glad_gl.c.o: glfw-3.3/examples/CMakeFiles/simple.dir/flags.make
glfw-3.3/examples/CMakeFiles/simple.dir/__/deps/glad_gl.c.o: ../glfw-3.3/deps/glad_gl.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/willchambers/Projects/VulkanPractice/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object glfw-3.3/examples/CMakeFiles/simple.dir/__/deps/glad_gl.c.o"
	cd /Users/willchambers/Projects/VulkanPractice/build/glfw-3.3/examples && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/simple.dir/__/deps/glad_gl.c.o   -c /Users/willchambers/Projects/VulkanPractice/glfw-3.3/deps/glad_gl.c

glfw-3.3/examples/CMakeFiles/simple.dir/__/deps/glad_gl.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/simple.dir/__/deps/glad_gl.c.i"
	cd /Users/willchambers/Projects/VulkanPractice/build/glfw-3.3/examples && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/willchambers/Projects/VulkanPractice/glfw-3.3/deps/glad_gl.c > CMakeFiles/simple.dir/__/deps/glad_gl.c.i

glfw-3.3/examples/CMakeFiles/simple.dir/__/deps/glad_gl.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/simple.dir/__/deps/glad_gl.c.s"
	cd /Users/willchambers/Projects/VulkanPractice/build/glfw-3.3/examples && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/willchambers/Projects/VulkanPractice/glfw-3.3/deps/glad_gl.c -o CMakeFiles/simple.dir/__/deps/glad_gl.c.s

# Object files for target simple
simple_OBJECTS = \
"CMakeFiles/simple.dir/simple.c.o" \
"CMakeFiles/simple.dir/__/deps/glad_gl.c.o"

# External object files for target simple
simple_EXTERNAL_OBJECTS =

glfw-3.3/examples/simple.app/Contents/MacOS/simple: glfw-3.3/examples/CMakeFiles/simple.dir/simple.c.o
glfw-3.3/examples/simple.app/Contents/MacOS/simple: glfw-3.3/examples/CMakeFiles/simple.dir/__/deps/glad_gl.c.o
glfw-3.3/examples/simple.app/Contents/MacOS/simple: glfw-3.3/examples/CMakeFiles/simple.dir/build.make
glfw-3.3/examples/simple.app/Contents/MacOS/simple: glfw-3.3/src/libglfw3.a
glfw-3.3/examples/simple.app/Contents/MacOS/simple: glfw-3.3/examples/CMakeFiles/simple.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/willchambers/Projects/VulkanPractice/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking C executable simple.app/Contents/MacOS/simple"
	cd /Users/willchambers/Projects/VulkanPractice/build/glfw-3.3/examples && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/simple.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
glfw-3.3/examples/CMakeFiles/simple.dir/build: glfw-3.3/examples/simple.app/Contents/MacOS/simple
glfw-3.3/examples/CMakeFiles/simple.dir/build: glfw-3.3/examples/simple.app/Contents/Resources/glfw.icns

.PHONY : glfw-3.3/examples/CMakeFiles/simple.dir/build

glfw-3.3/examples/CMakeFiles/simple.dir/clean:
	cd /Users/willchambers/Projects/VulkanPractice/build/glfw-3.3/examples && $(CMAKE_COMMAND) -P CMakeFiles/simple.dir/cmake_clean.cmake
.PHONY : glfw-3.3/examples/CMakeFiles/simple.dir/clean

glfw-3.3/examples/CMakeFiles/simple.dir/depend:
	cd /Users/willchambers/Projects/VulkanPractice/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/willchambers/Projects/VulkanPractice /Users/willchambers/Projects/VulkanPractice/glfw-3.3/examples /Users/willchambers/Projects/VulkanPractice/build /Users/willchambers/Projects/VulkanPractice/build/glfw-3.3/examples /Users/willchambers/Projects/VulkanPractice/build/glfw-3.3/examples/CMakeFiles/simple.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : glfw-3.3/examples/CMakeFiles/simple.dir/depend

