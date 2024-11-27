cmake_minimum_required(VERSION 3.11)

project(base_raylib_project
        VERSION 1.0
        DESCRIPTION "A base project for raylib usage"
        LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 17)

# Avoid extensions
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    set(CMAKE_CXX_EXTENSIONS OFF)
endif()

# Default build type if not specified
set(default_build_type "Release")
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    message(STATUS "Setting build type to '${default_build_type}' as none was specified.")
    set(CMAKE_BUILD_TYPE "${default_build_type}" CACHE STRING "Choose the type of build." FORCE)
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

# Add the cmake/ local folder
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})

# Prepare for LTO
include(CheckIPOSupported)
check_ipo_supported(RESULT LTO_result OUTPUT output)

if(NOT LTO_result)
    message(WARNING "IPO is not supported: ${output}")
endif()

# Submodules

find_package(Git QUIET)
if(GIT_FOUND AND EXISTS "${PROJECT_SOURCE_DIR}/.git")
    # Update submodules as needed
    option(GIT_SUBMODULE "Check submodules during build" ON)

    if(GIT_SUBMODULE)
        message(STATUS "Submodule update")
        execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive
                WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                RESULT_VARIABLE GIT_SUBMOD_RESULT)
        if(NOT GIT_SUBMOD_RESULT EQUAL "0")
            message(FATAL_ERROR "git submodule update --init --recursive failed with ${GIT_SUBMOD_RESULT}, please checkout submodules")
        endif()
    endif()
endif()

if(NOT EXISTS "${PROJECT_SOURCE_DIR}/extern/raylib/CMakeLists.txt")
    message(FATAL_ERROR "The submodules were not downloaded! GIT_SUBMODULE was turned off or failed. Please update submodules and try again.")
endif()

# RayLib & Google Test
cmake_policy(SET CMP0072 NEW)
set(OpenGL_GL_PREFERENCE "GLVND") # Set manually, even if the previous line should handle it...
add_subdirectory(extern/raylib)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
add_subdirectory(extern/googletest)

# Executable
set(EXE_NAME "base_raylib_project")
set(TEST_EXE_NAME ${EXE_NAME}_tests)

set(MAIN_FILE_LIST
)


add_executable(${EXE_NAME} src/main.cpp ${MAIN_FILE_LIST})
set_target_properties(${EXE_NAME} PROPERTIES
        POSITION_INDEPENDENT_CODE ON
        VS_DEBUGGER_WORKING_DIRECTORY "$(TargetDir)")

target_link_libraries(${EXE_NAME} raylib)

if(LTO_result AND (CMAKE_BUILD_TYPE STREQUAL "Release"))
    set_target_properties(${EXE_NAME} PROPERTIES INTERPROCEDURAL_OPTIMIZATION TRUE)
    message("IPO activated for ${EXE_NAME}")
endif()

# Tests for main project
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    include(CTest)
    #    if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME AND BUILD_TESTING)
    #        add_subdirectory(tests)
    #    endif()
endif()

# Add tests
add_executable(${TEST_EXE_NAME} tests/main_test.cpp ${MAIN_FILE_LIST})
target_link_libraries(${TEST_EXE_NAME} gtest)
