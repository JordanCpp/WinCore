
cmake_minimum_required(VERSION 3.15)

project(WinCore)

set(BACKEND_SDL2 ON)

if (BACKEND_SDL2)
    if (WIN32)
        set(SDL2_INCLUDE_DIRS "dependencies/SDL2-2.30.3/include")
        set(SDL2_LIBRARIES SDL2main SDL2)

        if(CMAKE_SIZEOF_VOID_P EQUAL 8)
            link_directories("dependencies/SDL2-2.30.3/lib/x64")
            file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/SDL2-2.30.3/lib/x64/SDL2.dll DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/)
        elseif(CMAKE_SIZEOF_VOID_P EQUAL 4)
            link_directories("dependencies/SDL2-2.30.3/lib/x86")
            file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/SDL2-2.30.3/lib/x86/SDL2.dll DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/)
        endif()
    else()
        find_package(SDL2 REQUIRED)
    endif()

    include_directories(${SDL2_INCLUDE_DIRS})
    include_directories("source/WinCore/Backends/SDL2")
endif()

if (BACKEND_SDL3)
    if (WIN32)
        set(SDL3_INCLUDE_DIRS "dependencies/SDL3-3.4.2/include")
        set(SDL3_LIBRARIES SDL3)

        if(CMAKE_SIZEOF_VOID_P EQUAL 8)
            link_directories("dependencies/SDL3-3.4.2/lib/x64")
            file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/SDL3-3.4.2/lib/x64/SDL3.dll DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/)
        elseif(CMAKE_SIZEOF_VOID_P EQUAL 4)
            link_directories("dependencies/SDL3-3.4.2/lib/x86")
            file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/SDL3-3.4.2/lib/x86/SDL3.dll DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/)
        endif()
    else()
        find_package(SDL3 REQUIRED)
    endif()

    include_directories(${SDL3_INCLUDE_DIRS})
    include_directories("source/WinCore/Backends/SDL3")
endif()

if (BACKEND_NULL)
    include_directories("source/WinCore/Backends/Null")
endif()

include_directories("include")
include_directories("source")

file(GLOB WINCORE_SOURCES 
"source/WinCore/Application.cpp"
"source/WinCore/BaseWindow.cpp"
"source/WinCore/ClassRegistrator.cpp"
"source/WinCore/WindowManager.cpp"
"source/WinCore/WindowClassA.cpp"
"source/WinCore/WindowCreator.cpp"
"source/WinCore/User32.cpp"
"source/WinCore/Gdi32.cpp")

if (BACKEND_SDL2)
    file(GLOB BACKEND_SOURCES 
        "source/WinCore/Backends/SDL2/WinCore/EventHandler.cpp"
        "source/WinCore/Backends/SDL2/WinCore/Initializer.cpp"
        "source/WinCore/Backends/SDL2/WinCore/Window.cpp")
endif()

if (BACKEND_SDL3)
    file(GLOB BACKEND_SOURCES 
        "source/WinCore/Backends/SDL3/WinCore/EventHandler.cpp"
        "source/WinCore/Backends/SDL3/WinCore/Initializer.cpp"
        "source/WinCore/Backends/SDL3/WinCore/Window.cpp")
endif()

if (BACKEND_NULL)
    file(GLOB BACKEND_SOURCES 
        "source/WinCore/Backends/Null/WinCore/EventHandler.cpp"
        "source/WinCore/Backends/Null/WinCore/Initializer.cpp"
        "source/WinCore/Backends/Null/WinCore/Window.cpp")
endif()

add_library(WinCore ${WINCORE_SOURCES} ${BACKEND_SOURCES})

if (BACKEND_SDL2)
    target_link_libraries(WinCore ${SDL2_LIBRARIES})
endif()

if (BACKEND_SDL3)
    target_link_libraries(WinCore ${SDL3_LIBRARIES})
endif()

add_executable(test "test.c")
target_link_libraries(test WinCore opengl32)
