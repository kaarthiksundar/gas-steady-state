# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.13

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
CMAKE_COMMAND = /Applications/CMake.app/Contents/bin/cmake

# The command to remove a file.
RM = /Applications/CMake.app/Contents/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/kaarthik/Documents/research/gas-steady-state

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/kaarthik/Documents/research/gas-steady-state/build

# Include any dependencies generated for this target.
include src/CMakeFiles/gas-steady-state.dir/depend.make

# Include the progress variables for this target.
include src/CMakeFiles/gas-steady-state.dir/progress.make

# Include the compile flags for this target's objects.
include src/CMakeFiles/gas-steady-state.dir/flags.make

src/CMakeFiles/gas-steady-state.dir/main.cpp.o: src/CMakeFiles/gas-steady-state.dir/flags.make
src/CMakeFiles/gas-steady-state.dir/main.cpp.o: ../src/main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/kaarthik/Documents/research/gas-steady-state/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object src/CMakeFiles/gas-steady-state.dir/main.cpp.o"
	cd /Users/kaarthik/Documents/research/gas-steady-state/build/src && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/gas-steady-state.dir/main.cpp.o -c /Users/kaarthik/Documents/research/gas-steady-state/src/main.cpp

src/CMakeFiles/gas-steady-state.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/gas-steady-state.dir/main.cpp.i"
	cd /Users/kaarthik/Documents/research/gas-steady-state/build/src && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/kaarthik/Documents/research/gas-steady-state/src/main.cpp > CMakeFiles/gas-steady-state.dir/main.cpp.i

src/CMakeFiles/gas-steady-state.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/gas-steady-state.dir/main.cpp.s"
	cd /Users/kaarthik/Documents/research/gas-steady-state/build/src && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/kaarthik/Documents/research/gas-steady-state/src/main.cpp -o CMakeFiles/gas-steady-state.dir/main.cpp.s

src/CMakeFiles/gas-steady-state.dir/filenames.cpp.o: src/CMakeFiles/gas-steady-state.dir/flags.make
src/CMakeFiles/gas-steady-state.dir/filenames.cpp.o: ../src/filenames.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/kaarthik/Documents/research/gas-steady-state/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object src/CMakeFiles/gas-steady-state.dir/filenames.cpp.o"
	cd /Users/kaarthik/Documents/research/gas-steady-state/build/src && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/gas-steady-state.dir/filenames.cpp.o -c /Users/kaarthik/Documents/research/gas-steady-state/src/filenames.cpp

src/CMakeFiles/gas-steady-state.dir/filenames.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/gas-steady-state.dir/filenames.cpp.i"
	cd /Users/kaarthik/Documents/research/gas-steady-state/build/src && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/kaarthik/Documents/research/gas-steady-state/src/filenames.cpp > CMakeFiles/gas-steady-state.dir/filenames.cpp.i

src/CMakeFiles/gas-steady-state.dir/filenames.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/gas-steady-state.dir/filenames.cpp.s"
	cd /Users/kaarthik/Documents/research/gas-steady-state/build/src && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/kaarthik/Documents/research/gas-steady-state/src/filenames.cpp -o CMakeFiles/gas-steady-state.dir/filenames.cpp.s

# Object files for target gas-steady-state
gas__steady__state_OBJECTS = \
"CMakeFiles/gas-steady-state.dir/main.cpp.o" \
"CMakeFiles/gas-steady-state.dir/filenames.cpp.o"

# External object files for target gas-steady-state
gas__steady__state_EXTERNAL_OBJECTS =

../bin/gas-steady-state: src/CMakeFiles/gas-steady-state.dir/main.cpp.o
../bin/gas-steady-state: src/CMakeFiles/gas-steady-state.dir/filenames.cpp.o
../bin/gas-steady-state: src/CMakeFiles/gas-steady-state.dir/build.make
../bin/gas-steady-state: /Users/kaarthik/Documents/research/Gravity/lib/libgravity.a
../bin/gas-steady-state: src/CMakeFiles/gas-steady-state.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/kaarthik/Documents/research/gas-steady-state/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX executable ../../bin/gas-steady-state"
	cd /Users/kaarthik/Documents/research/gas-steady-state/build/src && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/gas-steady-state.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/CMakeFiles/gas-steady-state.dir/build: ../bin/gas-steady-state

.PHONY : src/CMakeFiles/gas-steady-state.dir/build

src/CMakeFiles/gas-steady-state.dir/clean:
	cd /Users/kaarthik/Documents/research/gas-steady-state/build/src && $(CMAKE_COMMAND) -P CMakeFiles/gas-steady-state.dir/cmake_clean.cmake
.PHONY : src/CMakeFiles/gas-steady-state.dir/clean

src/CMakeFiles/gas-steady-state.dir/depend:
	cd /Users/kaarthik/Documents/research/gas-steady-state/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/kaarthik/Documents/research/gas-steady-state /Users/kaarthik/Documents/research/gas-steady-state/src /Users/kaarthik/Documents/research/gas-steady-state/build /Users/kaarthik/Documents/research/gas-steady-state/build/src /Users/kaarthik/Documents/research/gas-steady-state/build/src/CMakeFiles/gas-steady-state.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/CMakeFiles/gas-steady-state.dir/depend

