cmake_minimum_required(VERSION 3.15)
project(fastqr VERSION 1.0.23 LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Optimization flags for maximum performance
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS_RELEASE "-Ofast -DNDEBUG -march=native -flto")
set(CMAKE_C_FLAGS_RELEASE "-Ofast -DNDEBUG -march=native -flto")

# Options
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)
option(FASTQR_BUILD_EXAMPLES "Build examples" ON)
option(FASTQR_BUILD_BINDINGS "Build language bindings" ON)

# Find dependencies
find_package(PkgConfig REQUIRED)

# Find libpng via pkg-config (more reliable than find_package)
pkg_check_modules(PNG REQUIRED libpng)

# Find libqrencode
pkg_check_modules(QRENCODE REQUIRED libqrencode)

# Add library directories
link_directories(${QRENCODE_LIBRARY_DIRS})

# Object library for internal use (no linking yet)
add_library(fastqr_obj OBJECT
    src/fastqr.cpp
)

target_include_directories(fastqr_obj
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        ${QRENCODE_INCLUDE_DIRS}
        ${PNG_INCLUDE_DIRS}
)

target_compile_options(fastqr_obj
    PRIVATE
        ${QRENCODE_CFLAGS_OTHER}
)

# Main library (for install and linking by other projects)
add_library(fastqr
    $<TARGET_OBJECTS:fastqr_obj>
)

target_include_directories(fastqr
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_link_directories(fastqr
    PRIVATE
        ${QRENCODE_LIBRARY_DIRS}
)

target_link_libraries(fastqr
    PRIVATE
        ${QRENCODE_LIBRARIES}
        ${PNG_LIBRARIES}
)

# Set library output name
set_target_properties(fastqr PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION 1
    OUTPUT_NAME fastqr
)

# CLI tool
add_executable(fastqr-cli
    src/cli.cpp
)

# For standalone CLI binary, link with object library and static dependencies
if(NOT BUILD_SHARED_LIBS)
    # Add PNG library directories to search path
    link_directories(${PNG_LIBRARY_DIRS})

    # Find static versions of libraries
    find_library(PNG_STATIC_LIBRARY
        NAMES libpng.a libpng16.a
        PATHS
            ${PNG_LIBRARY_DIRS}
            /usr/local/lib
            /opt/homebrew/lib
            /usr/lib
        NO_DEFAULT_PATH
    )

    find_library(QRENCODE_STATIC_LIBRARY
        NAMES libqrencode.a
        PATHS
            /usr/local/lib           # Linux: from source build
            ${QRENCODE_LIBRARY_DIRS}
            /opt/homebrew/lib        # macOS: from Homebrew
            /usr/lib
        NO_DEFAULT_PATH
    )

    if(PNG_STATIC_LIBRARY AND QRENCODE_STATIC_LIBRARY)
        message(STATUS "✓ Found static libpng: ${PNG_STATIC_LIBRARY}")
        message(STATUS "✓ Found static libqrencode: ${QRENCODE_STATIC_LIBRARY}")

        # Link CLI directly with object library + static libs (no dependency propagation)
        target_link_libraries(fastqr-cli
            PRIVATE
                fastqr_obj
                ${QRENCODE_STATIC_LIBRARY}
                ${PNG_STATIC_LIBRARY}
                z  # zlib is needed by libpng
        )

        # On Linux, ensure full static linking
        if(UNIX AND NOT APPLE)
            # Try to link everything statically on Linux
            target_link_options(fastqr-cli PRIVATE
                -static-libgcc
                -static-libstdc++
            )

            # Also find static libz if possible
            find_library(ZLIB_STATIC_LIBRARY
                NAMES libz.a
                PATHS
                    /usr/local/lib
                    /usr/lib
                    /usr/lib/x86_64-linux-gnu
                    /usr/lib/aarch64-linux-gnu
                NO_DEFAULT_PATH
            )

            if(ZLIB_STATIC_LIBRARY)
                message(STATUS "✓ Found static libz: ${ZLIB_STATIC_LIBRARY}")
                # Replace z with static version
                target_link_libraries(fastqr-cli PRIVATE ${ZLIB_STATIC_LIBRARY})
            endif()
        endif()
    else()
        message(WARNING "Static libraries not found, falling back to dynamic linking")
        message(WARNING "  libpng: ${PNG_STATIC_LIBRARY}")
        message(WARNING "  libqrencode: ${QRENCODE_STATIC_LIBRARY}")

        target_link_libraries(fastqr-cli
            PRIVATE fastqr
        )
    endif()
else()
    # Shared library build
    target_link_libraries(fastqr-cli
        PRIVATE fastqr
    )
endif()

set_target_properties(fastqr-cli PROPERTIES
    OUTPUT_NAME fastqr
)

# Examples
if(FASTQR_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

# Install
include(GNUInstallDirs)

install(TARGETS fastqr fastqr-cli
    EXPORT fastqrTargets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install(FILES include/fastqr.h
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(EXPORT fastqrTargets
    FILE fastqrTargets.cmake
    NAMESPACE fastqr::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/fastqr
)

# Create config file
include(CMakePackageConfigHelpers)

configure_package_config_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/fastqrConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/fastqrConfig.cmake
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/fastqr
)

write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/fastqrConfigVersion.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/fastqrConfig.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/fastqrConfigVersion.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/fastqr
)

