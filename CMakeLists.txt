cmake_minimum_required(VERSION 3.14)
project(Name-Mangling)

set(CMAKE_CXX_STANDARD 17)

# Step 1: Create an object library
add_library(objlib OBJECT main.cpp)

# Step 2: Link the object library into an executable
add_executable(main $<TARGET_OBJECTS:objlib>)

# Step 3: Create symbol inspection as separate custom target
if (MSVC)
    # For MSVC, use dumpbin on object file
    add_custom_target(inspect_symbols ALL
        COMMAND ${CMAKE_COMMAND} -E echo "Using dumpbin to inspect symbols"
        COMMAND dumpbin /SYMBOLS $<TARGET_OBJECTS:objlib> | findstr print || cmd /c exit 0
        COMMENT "Inspecting symbols using dumpbin on object file"
        VERBATIM
    )
elseif(WIN32)
    # For MinGW or other Windows compilers, use nm on object file
    add_custom_target(inspect_symbols ALL
        COMMAND ${CMAKE_COMMAND} -E echo "Using nm to inspect symbols"
        COMMAND nm $<TARGET_OBJECTS:objlib> | findstr print || cmd /c exit 0
        COMMENT "Inspecting symbols using nm on object file"
        VERBATIM
    )
else()
    # On Linux/macOS, use nm on the final binary
    add_custom_target(inspect_symbols ALL
        COMMAND ${CMAKE_COMMAND} -E echo "Using nm to inspect symbols"
        COMMAND nm $<TARGET_FILE:main> | grep print || true
        COMMENT "Inspecting symbols using nm on executable"
        VERBATIM
    )
endif()

# Ensure inspection runs after build
add_dependencies(inspect_symbols main)
