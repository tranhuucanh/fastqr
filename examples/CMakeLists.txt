add_executable(example_basic basic.cpp)
target_link_libraries(example_basic PRIVATE fastqr)

install(TARGETS example_basic
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}/examples
)

