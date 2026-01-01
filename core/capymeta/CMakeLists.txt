set(LIBRARY_NAME capymeta)

option(
    CAPYMETA_WITH_BOOST
    "Enable Boost-based metaprogramming support in capymeta"
    OFF
)

file(GLOB_RECURSE HEADERS include/*.hpp)

add_library(${LIBRARY_NAME} INTERFACE ${HEADERS})

if(CAPYMETA_WITH_BOOST)
    cmake_policy(SET CMP0167 NEW)
    find_package(Boost QUIET CONFIG)
endif()

if(CAPYMETA_WITH_BOOST AND Boost_FOUND)
    target_link_libraries(${LIBRARY_NAME} INTERFACE Boost::headers)
    target_compile_definitions(${LIBRARY_NAME} INTERFACE BOOST_FOUND)
endif()

if(NOT CAPYMETA_WITH_BOOST)
    message(STATUS "${LIBRARY_NAME}: Boost support is DISABLED by user (CAPYMETA_WITH_BOOST=OFF)")
elseif(NOT Boost_FOUND)
    message(FATAL_ERROR "${LIBRARY_NAME}: Boost support requested but Boost was NOT FOUND. Please install Boost or disable CAPYMETA_WITH_BOOST.")
else()
    message(STATUS "${LIBRARY_NAME}: Boost support ENABLED, using Boost version ${Boost_VERSION}")
endif()

target_include_directories(${LIBRARY_NAME} INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)
