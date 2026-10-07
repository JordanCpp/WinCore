
cmake_minimum_required(VERSION 3.15)

function(add_wincore_example EXAMPLE_NAME)
    add_executable(${EXAMPLE_NAME} "${EXAMPLE_NAME}.c")

     set_target_properties(${EXAMPLE_NAME} PROPERTIES
        C_STANDARD 90
        C_STANDARD_REQUIRED ON
        C_EXTENSIONS OFF)
    
    target_link_libraries(${EXAMPLE_NAME} PRIVATE WinCore)
    
    if (WIN32)
        target_link_options(${EXAMPLE_NAME} PRIVATE "-Wl,--allow-multiple-definition")
    endif()
    
endfunction()

set(WINCORE_EXAMPLES
    GL3_Cube
)

foreach(EXAMPLE IN LISTS WINCORE_EXAMPLES)
    add_wincore_example(${EXAMPLE})
endforeach()
