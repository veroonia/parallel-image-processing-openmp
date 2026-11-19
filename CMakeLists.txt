cmake_minimum_required(VERSION 3.20)

if(WIN32 AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    if(DEFINED ENV{VCPKG_ROOT})
        set(_vcpkg_toolchain
            "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake")
    else()
        file(GLOB _vcpkg_toolchains
            "C:/Program Files/Microsoft Visual Studio/*/*/VC/vcpkg/scripts/buildsystems/vcpkg.cmake")
        list(SORT _vcpkg_toolchains)
        list(GET _vcpkg_toolchains -1 _vcpkg_toolchain)
    endif()

    if(EXISTS "${_vcpkg_toolchain}")
        set(CMAKE_TOOLCHAIN_FILE "${_vcpkg_toolchain}"
            CACHE FILEPATH "vcpkg toolchain file")
    endif()
endif()

if(WIN32 AND NOT DEFINED VCPKG_INSTALLED_DIR)
    set(VCPKG_INSTALLED_DIR
        "C:/Users/Public/vcpkg_installed/parallel-image-processing-openmp"
        CACHE PATH "Space-free vcpkg package directory")
endif()

if(WIN32)
    set(ENV{VCPKG_DOWNLOADS} "C:/Users/Public/vcpkg-downloads")
endif()

project(parallel_image_processing_openmp LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(OpenCV REQUIRED COMPONENTS core imgcodecs)
find_package(OpenMP REQUIRED)

add_executable(image_processor
    src/main.cpp
    src/processor.cpp
    src/gaussian.cpp
    src/sobel.cpp
    src/benchmark.cpp
)

target_include_directories(image_processor PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${OpenCV_INCLUDE_DIRS}
)

target_link_libraries(image_processor PRIVATE
    ${OpenCV_LIBS}
    OpenMP::OpenMP_CXX
)

set_target_properties(image_processor PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
)

set_property(
    DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    PROPERTY VS_STARTUP_PROJECT image_processor
)

enable_testing()

add_executable(gaussian_test
    tests/test_gaussian.cpp
    src/gaussian.cpp
)

target_include_directories(gaussian_test PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${OpenCV_INCLUDE_DIRS}
)

target_link_libraries(gaussian_test PRIVATE ${OpenCV_LIBS})

add_test(NAME gaussian_test COMMAND gaussian_test)
set_tests_properties(gaussian_test PROPERTIES
    WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
)
