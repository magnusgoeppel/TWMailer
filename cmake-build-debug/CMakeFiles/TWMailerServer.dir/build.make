# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.22

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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = "/mnt/c/Users/User/Documents/GitHub/Grundlagen verteilter Systeme Labor/TWMailerPro"

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = "/mnt/c/Users/User/Documents/GitHub/Grundlagen verteilter Systeme Labor/TWMailerPro/cmake-build-debug"

# Include any dependencies generated for this target.
include CMakeFiles/TWMailerServer.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/TWMailerServer.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/TWMailerServer.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/TWMailerServer.dir/flags.make

CMakeFiles/TWMailerServer.dir/twmailer-server.cpp.o: CMakeFiles/TWMailerServer.dir/flags.make
CMakeFiles/TWMailerServer.dir/twmailer-server.cpp.o: ../twmailer-server.cpp
CMakeFiles/TWMailerServer.dir/twmailer-server.cpp.o: CMakeFiles/TWMailerServer.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/mnt/c/Users/User/Documents/GitHub/Grundlagen verteilter Systeme Labor/TWMailerPro/cmake-build-debug/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/TWMailerServer.dir/twmailer-server.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/TWMailerServer.dir/twmailer-server.cpp.o -MF CMakeFiles/TWMailerServer.dir/twmailer-server.cpp.o.d -o CMakeFiles/TWMailerServer.dir/twmailer-server.cpp.o -c "/mnt/c/Users/User/Documents/GitHub/Grundlagen verteilter Systeme Labor/TWMailerPro/twmailer-server.cpp"

CMakeFiles/TWMailerServer.dir/twmailer-server.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/TWMailerServer.dir/twmailer-server.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/mnt/c/Users/User/Documents/GitHub/Grundlagen verteilter Systeme Labor/TWMailerPro/twmailer-server.cpp" > CMakeFiles/TWMailerServer.dir/twmailer-server.cpp.i

CMakeFiles/TWMailerServer.dir/twmailer-server.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/TWMailerServer.dir/twmailer-server.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/mnt/c/Users/User/Documents/GitHub/Grundlagen verteilter Systeme Labor/TWMailerPro/twmailer-server.cpp" -o CMakeFiles/TWMailerServer.dir/twmailer-server.cpp.s

# Object files for target TWMailerServer
TWMailerServer_OBJECTS = \
"CMakeFiles/TWMailerServer.dir/twmailer-server.cpp.o"

# External object files for target TWMailerServer
TWMailerServer_EXTERNAL_OBJECTS =

TWMailerServer: CMakeFiles/TWMailerServer.dir/twmailer-server.cpp.o
TWMailerServer: CMakeFiles/TWMailerServer.dir/build.make
TWMailerServer: CMakeFiles/TWMailerServer.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir="/mnt/c/Users/User/Documents/GitHub/Grundlagen verteilter Systeme Labor/TWMailerPro/cmake-build-debug/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable TWMailerServer"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/TWMailerServer.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/TWMailerServer.dir/build: TWMailerServer
.PHONY : CMakeFiles/TWMailerServer.dir/build

CMakeFiles/TWMailerServer.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/TWMailerServer.dir/cmake_clean.cmake
.PHONY : CMakeFiles/TWMailerServer.dir/clean

CMakeFiles/TWMailerServer.dir/depend:
	cd "/mnt/c/Users/User/Documents/GitHub/Grundlagen verteilter Systeme Labor/TWMailerPro/cmake-build-debug" && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" "/mnt/c/Users/User/Documents/GitHub/Grundlagen verteilter Systeme Labor/TWMailerPro" "/mnt/c/Users/User/Documents/GitHub/Grundlagen verteilter Systeme Labor/TWMailerPro" "/mnt/c/Users/User/Documents/GitHub/Grundlagen verteilter Systeme Labor/TWMailerPro/cmake-build-debug" "/mnt/c/Users/User/Documents/GitHub/Grundlagen verteilter Systeme Labor/TWMailerPro/cmake-build-debug" "/mnt/c/Users/User/Documents/GitHub/Grundlagen verteilter Systeme Labor/TWMailerPro/cmake-build-debug/CMakeFiles/TWMailerServer.dir/DependInfo.cmake" --color=$(COLOR)
.PHONY : CMakeFiles/TWMailerServer.dir/depend

