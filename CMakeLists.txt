cmake_minimum_required(VERSION 3.26)
project(Game VERSION 0.0.1 LANGUAGES CXX)

# Ensure the platform is Windows
if(NOT WIN32)
    message(FATAL_ERROR "This project only supports Windows for now.")
endif()

# Default to Debug if no build type is specified (for single-config generators)
if (NOT CMAKE_CONFIGURATION_TYPES AND NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
    message(STATUS "No build type specified - defaulting to Debug.")
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/$<CONFIG>)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS src/*.cpp)
file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS src/*.h)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

set_target_properties(${PROJECT_NAME} PROPERTIES WIN32_EXECUTABLE ON)

target_include_directories(${PROJECT_NAME} PRIVATE src)
target_precompile_headers(${PROJECT_NAME} PRIVATE src/pch.h)

target_compile_definitions(${PROJECT_NAME} PRIVATE
    UNICODE
    _UNICODE
    WIN32_LEAN_AND_MEAN
    NOMINMAX
    $<$<CONFIG:Debug>:_DEBUG>
)

# Warning and optimization flags for different compilers
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE
        /W4
        /permissive-

        $<$<CONFIG:Debug>:/Od;/Zi;/RTC1>
        $<$<CONFIG:Release>:/O2;/GL>
    )
    target_link_options(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:Release>:/LTCG>
    )
else()
    target_compile_options(${PROJECT_NAME} PRIVATE
        -Wall
        -Wextra
        -Wpedantic

        $<$<CONFIG:Debug>:-O0;-g>
        $<$<CONFIG:Release>:-O3>
    )
endif()

target_link_libraries(${PROJECT_NAME} PRIVATE
    d3d12
    dxgi
    dxguid
)

# ---------------------------------------------------------------------------------------
#                                  Shader Compilation
# ---------------------------------------------------------------------------------------

find_program(DXC_EXECUTABLE dxc)

# Ensure dxc.exe is installed
if(NOT DXC_EXECUTABLE)
    message(FATAL_ERROR "dxc.exe not found. Please ensure Windows SDK is installed.")
endif()

set(SHADER_SOURCE_DIR ${CMAKE_SOURCE_DIR}/assets/shaders)
set(SHADER_OUTPUT_DIR ${CMAKE_BINARY_DIR}/bin/${CMAKE_BUILD_TYPE}/shaders)

file(MAKE_DIRECTORY ${SHADER_OUTPUT_DIR})

file(GLOB_RECURSE HLSL_FILES CONFIGURE_DEPENDS ${SHADER_SOURCE_DIR}/*.hlsl)

set(SHADER_OUTPUTS "")

# Compile each shader source file twice (once for vertex shader and once for pixel shader)
foreach(HLSL_FILE ${HLSL_FILES})
    get_filename_component(SHADER_NAME ${HLSL_FILE} NAME_WE)

    set(VS_OUTPUT ${SHADER_OUTPUT_DIR}/${SHADER_NAME}_VS.cso)
    set(PS_OUTPUT ${SHADER_OUTPUT_DIR}/${SHADER_NAME}_PS.cso)

    add_custom_command(
        OUTPUT  ${VS_OUTPUT}
        COMMAND ${DXC_EXECUTABLE} -T vs_6_0 -E VSMain -Fo ${VS_OUTPUT} $<$<CONFIG:Debug>:-Od> $<$<CONFIG:Release>:-O3> ${HLSL_FILE}
        DEPENDS ${HLSL_FILE}
        COMMENT "Compiling vertex shader from ${HLSL_FILE}"
    )
    add_custom_command(
        OUTPUT  ${PS_OUTPUT}
        COMMAND ${DXC_EXECUTABLE} -T ps_6_0 -E PSMain -Fo ${PS_OUTPUT} $<$<CONFIG:Debug>:-Od> $<$<CONFIG:Release>:-O3> ${HLSL_FILE}
        DEPENDS ${HLSL_FILE}
        COMMENT "Compiling pixel shader from ${HLSL_FILE}"
    )
 
    list(APPEND SHADER_OUTPUTS ${VS_OUTPUT} ${PS_OUTPUT})
endforeach()

add_custom_target(Shaders ALL DEPENDS ${SHADER_OUTPUTS})
add_dependencies(${PROJECT_NAME} Shaders)