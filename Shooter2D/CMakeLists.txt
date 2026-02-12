cmake_minimum_required(VERSION 3.10)
project(SpaceShooter)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add source files
set(SOURCES
    main.cpp
    Game.cpp
)

# Create executable
add_executable(SpaceShooter ${SOURCES})

# Find SDL2
if(WIN32)
    # Windows: Look for SDL2 in the project directory
    set(SDL2_INCLUDE_DIRS "${CMAKE_SOURCE_DIR}/SDL2/include")
    set(SDL2_LIBRARIES "${CMAKE_SOURCE_DIR}/SDL2/lib/x64/SDL2.lib;${CMAKE_SOURCE_DIR}/SDL2/lib/x64/SDL2main.lib")
    
    # Copy SDL2.dll to output directory
    add_custom_command(TARGET SpaceShooter POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_SOURCE_DIR}/SDL2/lib/x64/SDL2.dll"
        $<TARGET_FILE_DIR:SpaceShooter>)
else()
    # Linux: Use pkg-config
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(SDL2 REQUIRED sdl2)
endif()

# Include directories
target_include_directories(SpaceShooter PRIVATE ${SDL2_INCLUDE_DIRS})

# Link libraries
target_link_libraries(SpaceShooter ${SDL2_LIBRARIES})

# Windows specific settings
if(WIN32)
    set_target_properties(SpaceShooter PROPERTIES
        WIN32_EXECUTABLE TRUE
    )
endif()
