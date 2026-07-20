cmake_minimum_required(VERSION 3.16)
project(atomic_eqplugin CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(IMGUI_DIR ${CMAKE_SOURCE_DIR}/ext/imgui)

add_library(imgui STATIC
    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_tables.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp
    ${IMGUI_DIR}/imgui_impl_win32.cpp
    ${IMGUI_DIR}/imgui_impl_dx11.cpp
)
target_include_directories(imgui PUBLIC ${IMGUI_DIR})


# Plugin-DLL
add_library(atomic_eqplugin SHARED
    src/dllmain.cpp
    src/gui.cpp
    src/script.cpp
)
target_include_directories(atomic_eqplugin PRIVATE src)
target_link_libraries(atomic_eqplugin PRIVATE imgui d3d11 dxgi d3dcompiler dwmapi)
set_target_properties(atomic_eqplugin PROPERTIES PREFIX "" OUTPUT_NAME "Atomic")

if(MINGW)
    target_link_options(atomic_eqplugin PRIVATE -static -static-libgcc -static-libstdc++)
endif()
