cmake_minimum_required(VERSION 3.15)

project(WinCore LANGUAGES CXX C)

option(BACKEND_SDL2 "Build with SDL2 backend" OFF)
option(BACKEND_SDL3 "Build with SDL3 backend" ON)
option(BACKEND_NULL "Build with Null backend" OFF)

if (BACKEND_SDL2)
    if (WIN32)
        set(SDL2_INCLUDE_DIRS "dependencies/SDL2-2.30.3/include")
        set(SDL2_LIBRARIES SDL2main SDL2)

        if(CMAKE_SIZEOF_VOID_P EQUAL 8)
            link_directories("dependencies/SDL2-2.30.3/lib/x64")
            file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/SDL2-2.30.3/lib/x64/SDL2.dll DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/)
            file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/SDL2-2.30.3/lib/x64/SDL2.dll DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/examples/GDI)
            file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/SDL2-2.30.3/lib/x64/SDL2.dll DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/examples/GL1)
        elseif(CMAKE_SIZEOF_VOID_P EQUAL 4)
            link_directories("dependencies/SDL2-2.30.3/lib/x86")
            file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/SDL2-2.30.3/lib/x86/SDL2.dll DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/)
            file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/SDL2-2.30.3/lib/x86/SDL2.dll DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/examples/GDI)
            file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/SDL2-2.30.3/lib/x86/SDL2.dll DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/examples/GL1)
        endif()
    else()
        find_package(SDL2 REQUIRED)
    endif()

    include_directories(${SDL2_INCLUDE_DIRS})
    include_directories("source/WinCore/Backends/SDL2")
    add_compile_definitions(WINCORE_BACKEND_SDL2)
endif()

if (BACKEND_SDL3)
    if (WIN32)
        set(SDL3_INCLUDE_DIRS "dependencies/SDL3-3.4.2/include")
        set(SDL3_LIBRARIES SDL3)

        if(CMAKE_SIZEOF_VOID_P EQUAL 8)
            link_directories("dependencies/SDL3-3.4.2/lib/x64")
            file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/SDL3-3.4.2/lib/x64/SDL3.dll DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/)
            file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/SDL3-3.4.2/lib/x64/SDL3.dll DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/examples/GDI)
            file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/SDL3-3.4.2/lib/x64/SDL3.dll DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/examples/GL1)
        elseif(CMAKE_SIZEOF_VOID_P EQUAL 4)
            link_directories("dependencies/SDL3-3.4.2/lib/x86")
            file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/SDL3-3.4.2/lib/x86/SDL3.dll DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/)
            file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/SDL3-3.4.2/lib/x86/SDL3.dll DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/examples/GDI)
            file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/SDL3-3.4.2/lib/x86/SDL3.dll DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/examples/GL1)
        endif()
    else()
        find_package(SDL3 REQUIRED)
    endif()

    include_directories(${SDL3_INCLUDE_DIRS})
    include_directories("source/WinCore/Backends/SDL3")
    add_compile_definitions(WINCORE_BACKEND_SDL3)
endif()

if (BACKEND_NULL)
    include_directories("source/WinCore/Backends/Null")
    add_compile_definitions(WINCORE_BACKEND_NULL)
endif()

include_directories("include")
include_directories("source")
include_directories("dependencies")

file(GLOB WINCORE_SOURCES 
    "source/WinCore/Application.cpp"
    "source/WinCore/BaseWindow.cpp"
    "source/WinCore/ClassRegistrator.cpp"
    "source/WinCore/WindowManager.cpp"
    "source/WinCore/WindowClassA.cpp"
    "source/WinCore/WindowCreator.cpp"
    "source/WinCore/SharedCreator.cpp"
    "source/WinCore/User32.cpp"
    "source/WinCore/Gdi32.cpp"
    "source/WinCore/Kernel32.cpp"
    "source/WinCore/OpenGL32.cpp"
)

if (BACKEND_SDL2)
    file(GLOB BACKEND_SOURCES 
        "source/WinCore/Backends/SDL2/WinCore/EventHandler.cpp"
        "source/WinCore/Backends/SDL2/WinCore/EventTranslator.cpp"
        "source/WinCore/Backends/SDL2/WinCore/Initializer.cpp"
        "source/WinCore/Backends/SDL2/WinCore/Shared.cpp"
        "source/WinCore/Backends/SDL2/WinCore/Window.cpp"
    )
endif()

if (BACKEND_SDL3)
    file(GLOB BACKEND_SOURCES 
        "source/WinCore/Backends/SDL3/WinCore/EventHandler.cpp"
        "source/WinCore/Backends/SDL3/WinCore/EventTranslator.cpp"
        "source/WinCore/Backends/SDL3/WinCore/Initializer.cpp"
        "source/WinCore/Backends/SDL3/WinCore/Window.cpp"
        "source/WinCore/Backends/SDL3/WinCore/Shared.cpp"
        "source/WinCore/Backends/SDL3/WinCore/OpenGLFunc.cpp"
        "source/WinCore/Backends/SDL3/WinCore/Ticks.cpp"
        "source/WinCore/Backends/SDL3/WinCore/Cursor.cpp"
        "source/WinCore/Backends/SDL3/WinCore/AsyncKey.cpp"
    )
endif()

if (BACKEND_NULL)
    file(GLOB BACKEND_SOURCES 
        "source/WinCore/Backends/Null/WinCore/EventHandler.cpp"
        "source/WinCore/Backends/Null/WinCore/Initializer.cpp"
        "source/WinCore/Backends/Null/WinCore/Window.cpp"
    )
endif()

add_library(WinCore SHARED ${WINCORE_SOURCES} ${BACKEND_SOURCES})

target_compile_definitions(WinCore PRIVATE WINCORE_BUILD_DLL)

set_target_properties(WinCore PROPERTIES
    CXX_VISIBILITY_PRESET hidden
    VISIBILITY_INLINES_HIDDEN ON
)

if (BACKEND_SDL2)
    target_link_libraries(WinCore PRIVATE ${SDL2_LIBRARIES})
endif()

if (BACKEND_SDL3)
    target_link_libraries(WinCore PRIVATE ${SDL3_LIBRARIES})
endif()

if (WIN32)
    target_link_libraries(WinCore PRIVATE opengl32)
else()
    find_package(OpenGL REQUIRED)
    target_link_libraries(WinCore PRIVATE OpenGL::GL)
endif()

if (WIN32 AND MINGW)
    target_link_options(WinCore PRIVATE "-Wl,--allow-multiple-definition")
endif()

add_custom_command(
    TARGET WinCore POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    $<TARGET_FILE:WinCore>
    "${CMAKE_CURRENT_BINARY_DIR}/examples/GDI"
    "${CMAKE_CURRENT_BINARY_DIR}/examples/GL1"
    COMMENT "Copying WinCore library to examples directory..."
)

add_subdirectory(examples)
