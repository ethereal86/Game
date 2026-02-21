cmake_minimum_required(VERSION 3.26)
project(Game VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

file(GLOB PROJECT_SOURCES "src/*.cpp")
file(GLOB PROJECT_HEADERS "src/*.h")

add_executable(${PROJECT_NAME} ${PROJECT_SOURCES} ${PROJECT_HEADERS})

target_compile_definitions(${PROJECT_NAME} PRIVATE
    $<$<CONFIG:Debug>:_DEBUG>
    UNICODE
    _UNICODE
)

if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4 /Zi $<$<CONFIG:Debug>:/Od>)
    target_link_options(${PROJECT_NAME} PRIVATE $<$<CONFIG:Debug>:/DEBUG>)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -g $<$<CONFIG:Debug>:-Og>)
endif()

if(WIN32)
    set(DX12_LIBS d3d12 dxgi)
    
    if(MINGW)
        list(APPEND DX12_LIBS dxguid uuid)
    endif()

    target_link_libraries(${PROJECT_NAME} PRIVATE 
        ${DX12_LIBS}
        user32
        gdi32
    )
endif()