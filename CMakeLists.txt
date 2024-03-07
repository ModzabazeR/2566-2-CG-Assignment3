cmake_minimum_required(VERSION 3.3)
project(CG-Assignment3)

set(CMAKE_CXX_STANDARD 17)

# Add the source files
set(SOURCE_FILES
        main.cpp
        Libs/Mesh.cpp
        Libs/Shader.cpp
        Libs/Window.cpp
        Libs/stb_image.cpp
)

# Add the executable
add_executable(CG-Assignment3 ${SOURCE_FILES})

# Find the necessary libraries
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(glfw3 REQUIRED)

# Include the necessary libraries
include_directories(${OPENGL_INCLUDE_DIR} ${GLEW_INCLUDE_DIRS})

# Link the necessary libraries
target_link_libraries(CG-Assignment3 ${OPENGL_LIBRARIES} ${GLEW_LIBRARIES} glfw)

# Copy shaders to build directory
file(GLOB SHADERS "Shaders/*")
foreach(SHADER ${SHADERS})
    get_filename_component(SHADER_NAME ${SHADER} NAME)
    add_custom_command(TARGET CG-Assignment3 POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy
            ${CMAKE_SOURCE_DIR}/Shaders/${SHADER_NAME}
            $<TARGET_FILE_DIR:CG-Assignment3>/Shaders/${SHADER_NAME})
endforeach()

# Copy textures to build directory
file(GLOB TEXTURES "Textures/*")
foreach(TEXTURE ${TEXTURES})
    get_filename_component(TEXTURE_NAME ${TEXTURE} NAME)
    add_custom_command(TARGET CG-Assignment3 POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy
            ${CMAKE_SOURCE_DIR}/Textures/${TEXTURE_NAME}
            $<TARGET_FILE_DIR:CG-Assignment3>/Textures/${TEXTURE_NAME})
endforeach()

# Copy models to build directory
file(GLOB MODELS "Models/*")
foreach(MODEL ${MODELS})
    get_filename_component(MODEL_NAME ${MODEL} NAME)
    add_custom_command(TARGET CG-Assignment3 POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy
            ${CMAKE_SOURCE_DIR}/Models/${MODEL_NAME}
            $<TARGET_FILE_DIR:CG-Assignment3>/Models/${MODEL_NAME})
endforeach()