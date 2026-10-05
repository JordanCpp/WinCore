
cmake_minimum_required(VERSION 3.15)

function(add_wincore_example EXAMPLE_NAME)
    add_executable(${EXAMPLE_NAME} "${EXAMPLE_NAME}.c")

     set_target_properties(${EXAMPLE_NAME} PROPERTIES
        C_STANDARD 90
        C_STANDARD_REQUIRED ON
        C_EXTENSIONS OFF)
    
    target_link_libraries(${EXAMPLE_NAME} PRIVATE WinCore)
    
    if (UNIX)
        target_link_libraries(${EXAMPLE_NAME} PRIVATE m)
    endif()
    
    if (WIN32)
        target_link_options(${EXAMPLE_NAME} PRIVATE "-Wl,--allow-multiple-definition")
        target_link_libraries(${EXAMPLE_NAME} PRIVATE opengl32)
    endif()
    
endfunction()

set(WINCORE_EXAMPLES
    GL1_DynamicSpotlight
    GL1_GouraudVsFlat
    GL1_MultitextureBlend
    GL1_Textured3DCube
    GL1_VertexArrays
)

foreach(EXAMPLE IN LISTS WINCORE_EXAMPLES)
    add_wincore_example(${EXAMPLE})
endforeach()
