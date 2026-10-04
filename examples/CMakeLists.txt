
cmake_minimum_required(VERSION 3.15)

add_executable(GL1_Textured3DCube "GL1_Textured3DCube.c")
target_link_options(GL1_Textured3DCube PRIVATE "-Wl,--allow-multiple-definition")
target_link_libraries(GL1_Textured3DCube WinCore)

add_executable(GL1_VertexArrays "GL1_VertexArrays.c")
target_link_options(GL1_VertexArrays PRIVATE "-Wl,--allow-multiple-definition")
target_link_libraries(GL1_VertexArrays WinCore)
