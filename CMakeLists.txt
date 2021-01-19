cmake_minimum_required (VERSION 3.10)
project(bnb)

if(NOT DEFINED STEINWURF_RESOLVE)
    message(STATUS "Resolving dependencies...")
    execute_process(
        COMMAND python waf resolve
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
    set(STEINWURF_RESOLVE "${CMAKE_CURRENT_SOURCE_DIR}/resolve_symlinks")
endif()

file(GLOB bnb_headers ./src/bnb/*.hpp)

add_library(bnb INTERFACE)
add_library(steinwurf::bnb ALIAS bnb)

if (NOT TARGET bitter)
add_subdirectory("${STEINWURF_RESOLVE}/bitter" bitter)
endif()

if (NOT TARGET endian)
add_subdirectory("${STEINWURF_RESOLVE}/endian" endian)
endif()

target_link_libraries(bnb
    INTERFACE steinwurf::endian
    INTERFACE steinwurf::bitter)

target_include_directories(bnb INTERFACE src)

target_compile_features(bnb INTERFACE cxx_std_11)

install(FILES ${bnb_headers} DESTINATION ${CMAKE_INSTALL_PREFIX}/include/bnb)
