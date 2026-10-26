# Sal Interface
add_library(
  Sal_Backend

  src/GLFW/GLFWWindow.cpp
  src/GLFW/GLFWInput.cpp

  src/OpenGL/OpenGLMesh.cpp
  src/OpenGL/OpenGLShaders.cpp
  src/OpenGL/OpenGLTexture.cpp
  src/OpenGL/OpenGLRenderer.cpp

  src/MiniAudio/MiniAudioAudio.cpp
  src/MiniAudio/MiniAudioAudioManager.cpp

  src/BackendFactory.cpp

  src/stb_image_imp.cpp
)

# Include
target_include_directories(
  Sal_Backend
  PUBLIC
  "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

# Linking
target_link_libraries(
  Sal_Backend
  PUBLIC
  Sal_Core
  Sal_Interface
)

# External Libraries

# Define macro for importing GitHub repos
include(FetchContent)

macro(ImportRepo NAME REPO COMMIT)
  message(
        STATUS
        "Module: ${NAME} @ ${COMMIT}"
    )

  FetchContent_Declare(
        ${NAME}
        GIT_REPOSITORY ${REPO}
        GIT_TAG ${COMMIT} # exact commit from lockfile
        GIT_SHALLOW TRUE
        UPDATE_DISCONNECTED FALSE
    )

  FetchContent_MakeAvailable(${NAME})
endmacro()

# GLAD
ImportRepo(glad https://github.com/Dav1dde/glad.git 5bf3eda)
add_library(glad STATIC ${glad_SOURCE_DIR}/src/glad.c)
target_include_directories(glad PUBLIC ${glad_SOURCE_DIR}/include)

# GLFW
ImportRepo(GLFW https://github.com/glfw/glfw.git 3.4)

# STB
ImportRepo(stb https://github.com/nothings/stb.git master)
add_library(stb_image INTERFACE)
target_include_directories(stb_image INTERFACE ${stb_SOURCE_DIR})

# Miniaudio
ImportRepo(miniaudio https://github.com/mackron/miniaudio.git 350784a)

if(UNIX AND NOT APPLE)
  target_link_libraries(Sal_Backend PRIVATE pthread m dl)
endif()

# Linking
target_link_libraries(
    Sal_Backend
    PRIVATE
    glad
    glfw
    stb_image
    miniaudio
)
