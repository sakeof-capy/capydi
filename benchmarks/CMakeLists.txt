set(EXECUTABLE_NAME capydi_benchmark)

add_executable(${EXECUTABLE_NAME} main.cpp)

target_link_libraries(${EXECUTABLE_NAME} PRIVATE capymeta)

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_compile_options(
    ${EXECUTABLE_NAME} PRIVATE
    -ftime-report
    -ftemplate-depth=2048
    -fplugin=externis
    -fplugin-arg-externis-trace=trace.json
    )    
endif()
