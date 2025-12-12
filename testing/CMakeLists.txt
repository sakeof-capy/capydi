set(LIBRARY_NAME testing)

file(GLOB_RECURSE SOURCES bin/*.cpp src/*.cpp)
file(GLOB_RECURSE HEADERS include/*.hpp)

add_executable(${LIBRARY_NAME} ${HEADERS} ${SOURCES})

target_link_libraries(${LIBRARY_NAME} PRIVATE 
    Catch2::Catch2WithMain 
    capydi_lib
)

target_include_directories(${LIBRARY_NAME}
    PUBLIC
        $<BUILD_INTERFACE:
            ${CMAKE_CURRENT_SOURCE_DIR}/include
            ${CMAKE_CURRENT_SOURCE_DIR}/include/${LIBRARY_NAME}
        >
        $<INSTALL_INTERFACE:include/>
)
