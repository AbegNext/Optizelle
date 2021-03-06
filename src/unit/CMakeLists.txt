# Common includes
include_directories(utility)

# Check if we want to build the unit tests
mark_as_advanced(CLEAR ENABLE_CPP_UNIT)
set(ENABLE_CPP_UNIT OFF CACHE BOOL "Enable unit tests for C++?")
if(ENABLE_PYTHON)
    mark_as_advanced(CLEAR ENABLE_PYTHON_UNIT)
    set(ENABLE_PYTHON_UNIT OFF CACHE BOOL "Enable unit tests for Python?")
endif()
if(ENABLE_MATLAB)
    mark_as_advanced(CLEAR ENABLE_MATLAB_UNIT)
    set(ENABLE_MATLAB_UNIT OFF CACHE BOOL "Enable unit tests for MATLAB?")
endif()
if(ENABLE_OCTAVE)
    mark_as_advanced(CLEAR ENABLE_OCTAVE_UNIT)
    set(ENABLE_OCTAVE_UNIT OFF CACHE BOOL "Enable unit tests for Octave?")
endif()

# Basic unit tests and utility functions
add_subdirectory(restart)
add_subdirectory(linear_algebra)
add_subdirectory(utility)
add_subdirectory(core)
add_subdirectory(config)
add_subdirectory(exceptions)
