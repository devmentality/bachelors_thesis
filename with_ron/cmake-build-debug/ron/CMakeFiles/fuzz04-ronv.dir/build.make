# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.19

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Disable VCS-based implicit rules.
% : %,v


# Disable VCS-based implicit rules.
% : RCS/%


# Disable VCS-based implicit rules.
% : RCS/%,v


# Disable VCS-based implicit rules.
% : SCCS/s.%


# Disable VCS-based implicit rules.
% : s.%


.SUFFIXES: .hpux_make_needs_suffix_list


# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /snap/clion/151/bin/cmake/linux/bin/cmake

# The command to remove a file.
RM = /snap/clion/151/bin/cmake/linux/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/devmentality/Documents/bachelors/bachelors_thesis/with_ron

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/devmentality/Documents/bachelors/bachelors_thesis/with_ron/cmake-build-debug

# Include any dependencies generated for this target.
include ron/CMakeFiles/fuzz04-ronv.dir/depend.make

# Include the progress variables for this target.
include ron/CMakeFiles/fuzz04-ronv.dir/progress.make

# Include the compile flags for this target's objects.
include ron/CMakeFiles/fuzz04-ronv.dir/flags.make

ron/CMakeFiles/fuzz04-ronv.dir/fuzz/ronv.cpp.o: ron/CMakeFiles/fuzz04-ronv.dir/flags.make
ron/CMakeFiles/fuzz04-ronv.dir/fuzz/ronv.cpp.o: ../ron/fuzz/ronv.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/devmentality/Documents/bachelors/bachelors_thesis/with_ron/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object ron/CMakeFiles/fuzz04-ronv.dir/fuzz/ronv.cpp.o"
	cd /home/devmentality/Documents/bachelors/bachelors_thesis/with_ron/cmake-build-debug/ron && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/fuzz04-ronv.dir/fuzz/ronv.cpp.o -c /home/devmentality/Documents/bachelors/bachelors_thesis/with_ron/ron/fuzz/ronv.cpp

ron/CMakeFiles/fuzz04-ronv.dir/fuzz/ronv.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/fuzz04-ronv.dir/fuzz/ronv.cpp.i"
	cd /home/devmentality/Documents/bachelors/bachelors_thesis/with_ron/cmake-build-debug/ron && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/devmentality/Documents/bachelors/bachelors_thesis/with_ron/ron/fuzz/ronv.cpp > CMakeFiles/fuzz04-ronv.dir/fuzz/ronv.cpp.i

ron/CMakeFiles/fuzz04-ronv.dir/fuzz/ronv.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/fuzz04-ronv.dir/fuzz/ronv.cpp.s"
	cd /home/devmentality/Documents/bachelors/bachelors_thesis/with_ron/cmake-build-debug/ron && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/devmentality/Documents/bachelors/bachelors_thesis/with_ron/ron/fuzz/ronv.cpp -o CMakeFiles/fuzz04-ronv.dir/fuzz/ronv.cpp.s

# Object files for target fuzz04-ronv
fuzz04__ronv_OBJECTS = \
"CMakeFiles/fuzz04-ronv.dir/fuzz/ronv.cpp.o"

# External object files for target fuzz04-ronv
fuzz04__ronv_EXTERNAL_OBJECTS =

ron/fuzz04-ronv: ron/CMakeFiles/fuzz04-ronv.dir/fuzz/ronv.cpp.o
ron/fuzz04-ronv: ron/CMakeFiles/fuzz04-ronv.dir/build.make
ron/fuzz04-ronv: ron/libron_static.a
ron/fuzz04-ronv: ron/CMakeFiles/fuzz04-ronv.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/devmentality/Documents/bachelors/bachelors_thesis/with_ron/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable fuzz04-ronv"
	cd /home/devmentality/Documents/bachelors/bachelors_thesis/with_ron/cmake-build-debug/ron && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/fuzz04-ronv.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
ron/CMakeFiles/fuzz04-ronv.dir/build: ron/fuzz04-ronv

.PHONY : ron/CMakeFiles/fuzz04-ronv.dir/build

ron/CMakeFiles/fuzz04-ronv.dir/clean:
	cd /home/devmentality/Documents/bachelors/bachelors_thesis/with_ron/cmake-build-debug/ron && $(CMAKE_COMMAND) -P CMakeFiles/fuzz04-ronv.dir/cmake_clean.cmake
.PHONY : ron/CMakeFiles/fuzz04-ronv.dir/clean

ron/CMakeFiles/fuzz04-ronv.dir/depend:
	cd /home/devmentality/Documents/bachelors/bachelors_thesis/with_ron/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/devmentality/Documents/bachelors/bachelors_thesis/with_ron /home/devmentality/Documents/bachelors/bachelors_thesis/with_ron/ron /home/devmentality/Documents/bachelors/bachelors_thesis/with_ron/cmake-build-debug /home/devmentality/Documents/bachelors/bachelors_thesis/with_ron/cmake-build-debug/ron /home/devmentality/Documents/bachelors/bachelors_thesis/with_ron/cmake-build-debug/ron/CMakeFiles/fuzz04-ronv.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : ron/CMakeFiles/fuzz04-ronv.dir/depend
