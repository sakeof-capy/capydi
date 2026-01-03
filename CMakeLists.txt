cmake_minimum_required(VERSION 3.10)
project(capydi)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_subdirectory(dependencies/Catch2)

find_package(Doxygen REQUIRED)

add_custom_target(
    generate_docs
    COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_SOURCE_DIR}/docs/Doxyfile
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/docs
    COMMENT "Generating API documentation with Doxygen"
    VERBATIM
)

add_subdirectory(core)
add_subdirectory(testing)
add_subdirectory(benchmarks)
