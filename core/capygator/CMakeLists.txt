set(BINARY_NAME capygator)

set(BINARY_MAIN_FILE bin/main.cpp)
file(GLOB_RECURSE SOURCES src/*.cpp)
file(GLOB_RECURSE HEADERS include/*.hpp)

add_executable(${BINARY_NAME})

target_sources(${BINARY_NAME}
    PRIVATE
        ${BINARY_MAIN_FILE}
        ${SOURCES}
        ${HEADERS}
)

target_include_directories(${BINARY_NAME}
    PUBLIC
        $<BUILD_INTERFACE:
            ${CMAKE_CURRENT_SOURCE_DIR}/include
            ${CMAKE_CURRENT_SOURCE_DIR}/include/${BINARY_NAME}
        >
        $<INSTALL_INTERFACE:include/>
)

