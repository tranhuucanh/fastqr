cmake_minimum_required(VERSION 3.15)
project(fastqr VERSION 1.0.10 LANGUAGES CXX C)

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
find_package(PNG REQUIRED)

# Find libqrencode
pkg_check_modules(QRENCODE REQUIRED libqrencode)

# Add library directories
link_directories(${QRENCODE_LIBRARY_DIRS})

# Main library
add_library(fastqr
    src/fastqr.cpp
)

target_include_directories(fastqr
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        ${QRENCODE_INCLUDE_DIRS}
        ${PNG_INCLUDE_DIRS}
)

target_link_directories(fastqr
    PRIVATE
        ${QRENCODE_LIBRARY_DIRS}
)

target_link_libraries(fastqr
    PRIVATE
        ${QRENCODE_LIBRARIES}
        PNG::PNG
)

target_compile_options(fastqr
    PRIVATE
        ${QRENCODE_CFLAGS_OTHER}
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

# Force static linking for CLI binary
if(NOT BUILD_SHARED_LIBS)
    # Link static library and all dependencies statically
    target_link_libraries(fastqr-cli
        PRIVATE 
            fastqr
            ${QRENCODE_STATIC_LIBRARIES}
    )
    
    # On macOS/Linux, force static linking of system libraries where possible
    if(APPLE)
        target_link_options(fastqr-cli PRIVATE 
            -static-libgcc 
            -static-libstdc++
        )
    elseif(UNIX)
        target_link_options(fastqr-cli PRIVATE 
            -static-libgcc 
            -static-libstdc++
            -Wl,-Bstatic
            -Wl,-Bdynamic
        )
    endif()
else()
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

