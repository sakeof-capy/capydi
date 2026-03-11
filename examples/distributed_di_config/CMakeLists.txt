set(BINARY_NAME distributed_di_config_example)
set(BINARY_MAIN_FILE bin/main.cpp)

set(GENERATED_DI_HPP_FILE ${CMAKE_CURRENT_SOURCE_DIR}/include/di_generated/DI.hpp)

file(GLOB_RECURSE SOURCES src/*.cpp)
file(GLOB_RECURSE HEADERS include/*.hpp)

add_executable(${BINARY_NAME})

target_sources(${BINARY_NAME}
    PRIVATE
        ${BINARY_MAIN_FILE}
        ${SOURCES}
        ${HEADERS}
)

target_link_libraries(${BINARY_NAME} 
    PRIVATE 
        capydi
        capymeta
)

target_include_directories(${BINARY_NAME}
    PUBLIC
        $<BUILD_INTERFACE:
            ${CMAKE_CURRENT_SOURCE_DIR}/include
            ${CMAKE_CURRENT_SOURCE_DIR}/include/${BINARY_NAME}
        >
        $<INSTALL_INTERFACE:include/>
)

add_custom_target(run_di_aggregator
    COMMAND $<TARGET_FILE:capygator> ${CMAKE_SOURCE_DIR} ${GENERATED_DI_HPP_FILE} . capydi
    BYPRODUCTS ${GENERATED_DI_HPP_FILE}
    COMMENT "Running DI-aggregator"
)

add_dependencies(${BINARY_NAME} run_di_aggregator)
