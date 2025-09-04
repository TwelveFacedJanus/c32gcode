cmake_minimum_required(VERSION 3.15)

# Set project name and version
project(C32GCode VERSION 1.0.0 LANGUAGES CXX C)  # Add C language support

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Set output directories
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Find required packages
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(GLEW REQUIRED)

# Add ImGui as a library
add_library(imgui STATIC
        external/imgui/imgui.cpp
        external/imgui/imgui_demo.cpp
        external/imgui/imgui_draw.cpp
        external/imgui/imgui_tables.cpp
        external/imgui/imgui_widgets.cpp
        # GLFW backend
        external/imgui/backends/imgui_impl_glfw.cpp
        # OpenGL backend
        external/imgui/backends/imgui_impl_opengl3.cpp
)

# Set include directories for ImGui
target_include_directories(imgui PUBLIC
        external/imgui
        external/imgui/backends
)

# Link ImGui dependencies
target_link_libraries(imgui PRIVATE
        OpenGL::GL
        glfw
)
add_subdirectory(external/spdlog)


# Add Native File Dialog library with platform-specific sources
add_library(nfd STATIC)

target_include_directories(nfd PUBLIC
        external/nativefiledialog/src/include
        external/nativefiledialog/src
)

# Platform-specific source files for nfd
if(WIN32)
    target_sources(nfd PRIVATE
            external/nativefiledialog/src/nfd_win.cpp
            external/nativefiledialog/src/nfd_common.c
    )
elseif(APPLE)
    # For macOS, we need to handle Objective-C files separately
    target_sources(nfd PRIVATE
            external/nativefiledialog/src/nfd_cocoa.m
            external/nativefiledialog/src/nfd_common.c
    )

    # Set language standards for Objective-C files
    set_source_files_properties(external/nativefiledialog/src/nfd_cocoa.m
            PROPERTIES
            COMPILE_FLAGS "-x objective-c"  # Explicitly set as Objective-C
    )

    set_source_files_properties(external/nativefiledialog/src/nfd_common.c
            PROPERTIES
            COMPILE_FLAGS "-std=c99"  # Set C standard for C files
    )

    # Find and link Cocoa framework for macOS
    find_library(COCOA_LIBRARY Cocoa)
    target_link_libraries(nfd PRIVATE ${COCOA_LIBRARY})

else() # Linux/Unix
    target_sources(nfd PRIVATE
            external/nativefiledialog/src/nfd_gtk.c
            external/nativefiledialog/src/nfd_common.c
    )

    # Set C standard for C files on Linux
    set_source_files_properties(
            external/nativefiledialog/src/nfd_gtk.c
            external/nativefiledialog/src/nfd_common.c
            PROPERTIES
            COMPILE_FLAGS "-std=c99"
    )

    # For GTK on Linux, we need to find the required packages
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(GTK3 REQUIRED gtk+-3.0)
    target_include_directories(nfd PRIVATE ${GTK3_INCLUDE_DIRS})
    target_link_libraries(nfd PRIVATE ${GTK3_LIBRARIES})
    target_compile_options(nfd PRIVATE ${GTK3_CFLAGS_OTHER})
endif()

# Create executable target
add_executable(C32GCode
        source/main.cpp
        source/wm/window_manager.cpp
        source/wm/window_manager.h
        source/ui/ui.cpp
        source/ui/ui.h)

# Target include directories
target_include_directories(C32GCode PRIVATE
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}/source
        ${GLEW_INCLUDE_DIRS}
        ${GLFW_INCLUDE_DIRS}
        ${CMAKE_SOURCE_DIR}/external
        # Add ImGui includes
        external/imgui
        external/imgui/backends
        # Add nfd includes
        external/nativefiledialog/src
        # Add spdlog includes
        external/spdlog/include
)

# Find and add source files
file(GLOB_RECURSE SOURCES
        "source/*.cpp"
)

# Add sources to target
target_sources(C32GCode PRIVATE ${SOURCES})

# Link libraries
target_link_libraries(C32GCode PRIVATE
        OpenGL::GL
        GLEW::GLEW
        glfw
        imgui
        nfd
        spdlog::spdlog
)

# Set target properties
set_target_properties(C32GCode PROPERTIES
        OUTPUT_NAME "C32GCode"
        VERSION ${PROJECT_VERSION}
)

# Compiler options - only apply to C++ files
target_compile_options(C32GCode PRIVATE
        $<$<COMPILE_LANGUAGE:CXX>:$<$<CXX_COMPILER_ID:GNU,Clang,AppleClang>:-Wall -Wextra -Wpedantic -O2>>
        $<$<COMPILE_LANGUAGE:CXX>:$<$<CXX_COMPILER_ID:MSVC>:/W4 /O2>>
)

# Copy resources to build directory
if(EXISTS ${CMAKE_SOURCE_DIR}/resources)
    add_custom_command(TARGET C32GCode POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/resources
            $<TARGET_FILE_DIR:C32GCode>/resources
            COMMENT "Copying resources to build directory"
    )
endif()

# Optional: Install instructions
install(TARGETS C32GCode
        RUNTIME DESTINATION bin
)

# Optional: Install headers
if(EXISTS ${CMAKE_SOURCE_DIR}/include)
    install(DIRECTORY include/ DESTINATION include)
endif()

# Optional: Install resources
if(EXISTS ${CMAKE_SOURCE_DIR}/resources)
    install(DIRECTORY resources/ DESTINATION resources)
endif()