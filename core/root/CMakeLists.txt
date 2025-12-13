set(LIBRARY_NAME capydi)

file(GLOB_RECURSE SOURCES bin/*.cpp src/*.cpp)
file(GLOB_RECURSE HEADERS include/*.hpp)

add_library(${LIBRARY_NAME} INTERFACE)

target_include_directories(${LIBRARY_NAME} INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)
