set(EXECUTABLE_NAME capydi)
set(LIBRARY_NAME capydi_lib)

file(GLOB_RECURSE SOURCES bin/*.cpp src/*.cpp)
file(GLOB_RECURSE HEADERS include/*.hpp)

add_executable(${EXECUTABLE_NAME} ${SOURCES} ${HEADERS})
add_library(${LIBRARY_NAME} INTERFACE)

target_include_directories(${EXECUTABLE_NAME}
    PUBLIC
        $<BUILD_INTERFACE:
            ${CMAKE_CURRENT_SOURCE_DIR}/include
            ${CMAKE_CURRENT_SOURCE_DIR}/include/${EXECUTABLE_NAME}
        >
        $<INSTALL_INTERFACE:include/>
)

target_include_directories(${LIBRARY_NAME} 
    INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include/${EXECUTABLE_NAME}
)
