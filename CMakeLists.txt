cmake_minimum_required(VERSION 3.21)
project(capydi)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

if (PROJECT_IS_TOP_LEVEL)
    include(cmake/Dependencies.cmake)
endif()

find_package(Doxygen REQUIRED)

add_custom_target(
    generate_docs
    COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_SOURCE_DIR}/docs/Doxyfile
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/docs
    COMMENT "Generating API documentation with Doxygen"
    VERBATIM
)

add_subdirectory(core)
add_subdirectory(examples)

if (PROJECT_IS_TOP_LEVEL)
    add_subdirectory(testing)
    add_subdirectory(benchmarks)
endif()
