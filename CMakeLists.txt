
cmake_minimum_required(VERSION 3.15)

project(WinCore)

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

include_directories("include")
include_directories("source")
include_directories("source/WinCore/Backends/SDL2")

add_library(WinCore 
"source/WinCore/Application.cpp"
"source/WinCore/BaseWindow.cpp"
"source/WinCore/ClassRegistrator.cpp"
"source/WinCore/WindowManager.cpp"
"source/WinCore/WindowClassA.cpp"
"source/WinCore/WindowCreator.cpp"
"source/WinCore/User32.cpp"

"source/WinCore/Backends/SDL2/WinCore/EventHandler.cpp"
"source/WinCore/Backends/SDL2/WinCore/Initializer.cpp"
"source/WinCore/Backends/SDL2/WinCore/Window.cpp")

add_executable(test "test.c")
target_link_libraries(test WinCore ${SDL2_LIBRARIES})
