# =============================================================================
#
#       Filename:  RemoveSpikeNoiseFilter.cxx
#
#    Description:  A filter that removes spike noises
#
#        Version:  1.0
#        Created:  09/14/2014 01:26:42 PM
#       Revision:  none
#       Compiler:  gcc
#
#         Author:  Siavash Ameli
#   Organization:  University Of California, Berkeley
#
# =============================================================================

cmake_minimum_required(VERSION 3.12)
project(RemoveSpikeNoiseFilter CXX)

# ===
# VTK
# ===

find_package(VTK REQUIRED)
if(NOT VTK_FOUND)
    message(FATAL_ERROR "VTK not found.")
endif(NOT VTK_FOUND)

# ================
# Source Inclusion
# ================

set(PROJECT_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(PROJECT_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(EXECUTABLE_NAME "RemoveSpikeNoise")
add_executable(${EXECUTABLE_NAME}
    ${PROJECT_SOURCE_DIR}/main.cxx
    ${PROJECT_SOURCE_DIR}/RemoveSpikeNoiseFilter.cxx)
target_link_libraries(${EXECUTABLE_NAME} ${VTK_LIBRARIES})

# ==================
# Output Directories
# ==================

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/bin CACHE PATH "Directory for all executables")

# ==============
# Compiler Flags
# ==============

set(CMAKE_CXX_FLAGS "-Wall -std=c++0x ${CMAKE_CXX_FLAGS}")

# ============
# OpenMP Flags
# ============

set(USE_OPENMP ON CACHE BOOL "Use OpenMP?")
if(USE_OPENMP)
    find_package(OpenMP)
    if(OPENMP_FOUND)
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FALGS} ${OpenMP_CXX_FLAGS}")
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
    endif(OPENMP_FOUND)
endif(USE_OPENMP)
