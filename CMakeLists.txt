cmake_minimum_required(VERSION 3.5.0)
include(CMakePrintHelpers)

project(kalman-filter-proj VERSION 0.1.0 LANGUAGES C CXX)
set (CMAKE_CXX_STANDARD 17)

include(CTest)
enable_testing()

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
# set(CMAKE_VERBOSE_MAKEFILE on)

set(SOURCES
    src/main.cpp
)

find_package(Eigen3 3.1.2)

if (EIGEN3_FOUND) 
    add_executable(${PROJECT_NAME} ${SOURCES})

    target_include_directories(${PROJECT_NAME}
        PRIVATE
            ${PROJECT_SOURCE_DIR}/inc
            ${EIGEN3_INCLUDE_DIRS}
    )   

    if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        target_compile_options(my_executable PRIVATE -Wno-deprecated-declarations)
    endif()
    # cmake_print_variables(CMAKE_C_COMPILER CMAKE_MAJOR_VERSION DOES_NOT_EXIST)
    cmake_print_variables(EIGEN3_INCLUDE_DIRS)

endif()




 

