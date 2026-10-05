
cmake_minimum_required(VERSION 3.15)

function(add_wincore_example EXAMPLE_NAME)
    add_executable(${EXAMPLE_NAME} "${EXAMPLE_NAME}.c")
    
    target_link_libraries(${EXAMPLE_NAME} PRIVATE WinCore)
    
    if (WIN32)
        target_link_options(${EXAMPLE_NAME} PRIVATE "-Wl,--allow-multiple-definition")
        target_link_libraries(${EXAMPLE_NAME} PRIVATE opengl32)
    endif()
endfunction()

set(WINCORE_EXAMPLES
    GL1_Textured3DCube
    GL1_VertexArrays
)

foreach(EXAMPLE IN LISTS WINCORE_EXAMPLES)
    add_wincore_example(${EXAMPLE})
endforeach()
