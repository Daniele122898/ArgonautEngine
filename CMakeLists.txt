cmake_minimum_required(VERSION 3.19)
project(Engine)

set(CMAKE_CXX_STANDARD 17)
#set(CMAKE_C_COMPILER "C:/Program Files (x86)/Microsoft Visual Studio/2019/Community/VC/Tools/MSVC/14.29.30133/bin/Hostx86/x86/cl.exe")
#set(CMAKE_CXX_COMPILER "C:/Program Files (x86)/Microsoft Visual Studio/2019/Community/VC/Tools/MSVC/14.29.30133/bin/Hostx86/x86/cl.exe")

# Glad configuration
set(GLAD_PROFILE "core" CACHE STRING "OpenGL profile")
set(GLAD_API "gl=4.6" CACHE STRING "API type/version pairs, like \"gl=3.2,gles=\", no version means latest")
add_subdirectory(vendor/glad)

# glfw configuration
add_subdirectory(vendor/glfw)

# glm configuration
add_subdirectory(vendor/glm)

# fmt configuration
#add_subdirectory(vendor/fmt)

# spdlog configuration
add_subdirectory(vendor/spdlog)

# Get recursively all .cpp and .hpp files in src subdir
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "src/*.cpp" "src/*.h")
# Variable Engine is executable from SOURCES defined above
add_executable(Engine ${SOURCES})

if(MSVC)
    target_compile_options(Engine PRIVATE /W4 /WX /wd4201)
else()
    target_compile_options(Engine PRIVATE -Wall -Wextra -pedantic -Werror -fdiagnostics-show-template-tree)
endif()

# Says that Engine wants to link against glfw and glad and then does magic and makes headers work.
target_link_libraries(Engine PRIVATE glfw glad glm spdlog)