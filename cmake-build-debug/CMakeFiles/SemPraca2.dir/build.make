﻿# CMAKE generated file: DO NOT EDIT!
# Generated by "NMake Makefiles" Generator, CMake Version 3.27

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

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

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE
NULL=nul
!ENDIF
SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = "C:\Program Files\JetBrains\CLion 2023.3\bin\cmake\win\x64\bin\cmake.exe"

# The command to remove a file.
RM = "C:\Program Files\JetBrains\CLion 2023.3\bin\cmake\win\x64\bin\cmake.exe" -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = C:\Users\spotk\Desktop\FRI\5.semester\PoS\Wildfire-simulation---POS

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = C:\Users\spotk\Desktop\FRI\5.semester\PoS\Wildfire-simulation---POS\cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles\SemPraca2.dir\depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles\SemPraca2.dir\compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles\SemPraca2.dir\progress.make

# Include the compile flags for this target's objects.
include CMakeFiles\SemPraca2.dir\flags.make

CMakeFiles\SemPraca2.dir\main.cpp.obj: CMakeFiles\SemPraca2.dir\flags.make
CMakeFiles\SemPraca2.dir\main.cpp.obj: C:\Users\spotk\Desktop\FRI\5.semester\PoS\Wildfire-simulation---POS\main.cpp
CMakeFiles\SemPraca2.dir\main.cpp.obj: CMakeFiles\SemPraca2.dir\compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=C:\Users\spotk\Desktop\FRI\5.semester\PoS\Wildfire-simulation---POS\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/SemPraca2.dir/main.cpp.obj"
	$(CMAKE_COMMAND) -E cmake_cl_compile_depends --dep-file=CMakeFiles\SemPraca2.dir\main.cpp.obj.d --working-dir=C:\Users\spotk\Desktop\FRI\5.semester\PoS\Wildfire-simulation---POS\cmake-build-debug --filter-prefix="Note: including file: " -- C:\PROGRA~1\MIB055~1\2022\COMMUN~1\VC\Tools\MSVC\1435~1.322\bin\Hostx64\x64\cl.exe @<<
 /nologo /TP $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) /showIncludes /FoCMakeFiles\SemPraca2.dir\main.cpp.obj /FdCMakeFiles\SemPraca2.dir\ /FS -c C:\Users\spotk\Desktop\FRI\5.semester\PoS\Wildfire-simulation---POS\main.cpp
<<

CMakeFiles\SemPraca2.dir\main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/SemPraca2.dir/main.cpp.i"
	C:\PROGRA~1\MIB055~1\2022\COMMUN~1\VC\Tools\MSVC\1435~1.322\bin\Hostx64\x64\cl.exe > CMakeFiles\SemPraca2.dir\main.cpp.i @<<
 /nologo /TP $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Users\spotk\Desktop\FRI\5.semester\PoS\Wildfire-simulation---POS\main.cpp
<<

CMakeFiles\SemPraca2.dir\main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/SemPraca2.dir/main.cpp.s"
	C:\PROGRA~1\MIB055~1\2022\COMMUN~1\VC\Tools\MSVC\1435~1.322\bin\Hostx64\x64\cl.exe @<<
 /nologo /TP $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) /FoNUL /FAs /FaCMakeFiles\SemPraca2.dir\main.cpp.s /c C:\Users\spotk\Desktop\FRI\5.semester\PoS\Wildfire-simulation---POS\main.cpp
<<

# Object files for target SemPraca2
SemPraca2_OBJECTS = \
"CMakeFiles\SemPraca2.dir\main.cpp.obj"

# External object files for target SemPraca2
SemPraca2_EXTERNAL_OBJECTS =

SemPraca2.exe: CMakeFiles\SemPraca2.dir\main.cpp.obj
SemPraca2.exe: CMakeFiles\SemPraca2.dir\build.make
SemPraca2.exe: CMakeFiles\SemPraca2.dir\objects1.rsp
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=C:\Users\spotk\Desktop\FRI\5.semester\PoS\Wildfire-simulation---POS\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable SemPraca2.exe"
	"C:\Program Files\JetBrains\CLion 2023.3\bin\cmake\win\x64\bin\cmake.exe" -E vs_link_exe --intdir=CMakeFiles\SemPraca2.dir --rc=C:\PROGRA~2\WI3CF2~1\10\bin\100190~1.0\x64\rc.exe --mt=C:\PROGRA~2\WI3CF2~1\10\bin\100190~1.0\x64\mt.exe --manifests -- C:\PROGRA~1\MIB055~1\2022\COMMUN~1\VC\Tools\MSVC\1435~1.322\bin\Hostx64\x64\link.exe /nologo @CMakeFiles\SemPraca2.dir\objects1.rsp @<<
 /out:SemPraca2.exe /implib:SemPraca2.lib /pdb:C:\Users\spotk\Desktop\FRI\5.semester\PoS\Wildfire-simulation---POS\cmake-build-debug\SemPraca2.pdb /version:0.0 /machine:x64 /debug /INCREMENTAL /subsystem:console  kernel32.lib user32.lib gdi32.lib winspool.lib shell32.lib ole32.lib oleaut32.lib uuid.lib comdlg32.lib advapi32.lib 
<<

# Rule to build all files generated by this target.
CMakeFiles\SemPraca2.dir\build: SemPraca2.exe
.PHONY : CMakeFiles\SemPraca2.dir\build

CMakeFiles\SemPraca2.dir\clean:
	$(CMAKE_COMMAND) -P CMakeFiles\SemPraca2.dir\cmake_clean.cmake
.PHONY : CMakeFiles\SemPraca2.dir\clean

CMakeFiles\SemPraca2.dir\depend:
	$(CMAKE_COMMAND) -E cmake_depends "NMake Makefiles" C:\Users\spotk\Desktop\FRI\5.semester\PoS\Wildfire-simulation---POS C:\Users\spotk\Desktop\FRI\5.semester\PoS\Wildfire-simulation---POS C:\Users\spotk\Desktop\FRI\5.semester\PoS\Wildfire-simulation---POS\cmake-build-debug C:\Users\spotk\Desktop\FRI\5.semester\PoS\Wildfire-simulation---POS\cmake-build-debug C:\Users\spotk\Desktop\FRI\5.semester\PoS\Wildfire-simulation---POS\cmake-build-debug\CMakeFiles\SemPraca2.dir\DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles\SemPraca2.dir\depend
