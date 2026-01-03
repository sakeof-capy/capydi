if(NOT CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    message(FATAL_ERROR "Only GNU compiler is supported")
endif()

set(COMMON_COMPILE_OPTIONS 
    -ftime-report
    -ftemplate-depth=2048
    -fplugin=externis
    -fplugin-arg-externis-trace=trace.json
)

set(COMMON_LIBRARIES
    capydi
    capymeta
)

file(GLOB BENCHMARK_SOURCES "src/*.cpp")

foreach(BENCH_SOURCE ${BENCHMARK_SOURCES})
    get_filename_component(BENCH_NAME ${BENCH_SOURCE} NAME_WE)
    add_executable(${BENCH_NAME} ${BENCH_SOURCE})
    target_link_libraries(${BENCH_NAME} PRIVATE ${COMMON_LIBRARIES})
    target_compile_options(${BENCH_NAME} PRIVATE ${COMMON_COMPILE_OPTIONS})
endforeach()
