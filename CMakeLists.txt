cmake_minimum_required(VERSION 2.8)
project(InsTux)

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS 5.2)
        message(FATAL_ERROR "Your GCC seems outdated. Go to https://github.com/AimTuxOfficial/AimTux/wiki/Updating-your-compiler for further instructions. If you have done so and still see this message, run 'git clean -fdx' and try again")
    endif()
else()
    message(FATAL_ERROR "You must have GCC >= 5.2 installed in order to compile AimTux. Go to https://github.com/AimTuxOfficial/AimTux/wiki/Updating-your-compiler for further instructions.")
endif()

FUNCTION (EXCLUDE_FILES_FROM_DIR_IN_LIST _InFileList _excludeDirName _verbose)
    foreach (ITR ${_InFileList})
        if ("${_verbose}")
            message(STATUS "ITR=${ITR}")
        endif ("${_verbose}")

        if ("${ITR}" MATCHES "(.*)${_excludeDirName}(.*)") # Check if the item matches the directory name in _excludeDirName
            message(STATUS "Remove Item from List:${ITR}")
            list (REMOVE_ITEM _InFileList ${ITR}) # Remove the item from the list
        endif ("${ITR}" MATCHES "(.*)${_excludeDirName}(.*)")

    endforeach(ITR)
    set(SOURCE_FILES ${_InFileList} PARENT_SCOPE) # Return the SOURCE_FILES variable to the calling parent
ENDFUNCTION (EXCLUDE_FILES_FROM_DIR_IN_LIST)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Wall -Wno-maybe-uninitialized -Wno-unused-result -O3 --no-gnu-unique") #force 32bit compilation
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -static-libstdc++ --no-gnu-unique")
set(CMAKE_BUILD_TYPE Release)

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

file(GLOB_RECURSE SOURCE_FILES src/*.cpp)

option(WITH_GUI "Compile with ImGui." OFF)
if(WITH_GUI)
    message(STATUS "Generating Makefile with ImGui (to disable GUI, run 'cmake . -DWITH_GUI=OFF')")
    add_definitions(-DUSE_IMGUI=1)
    EXCLUDE_FILES_FROM_DIR_IN_LIST("${SOURCE_FILES}" "${CMAKE_SOURCE_DIR}/src/imgui/examples/" FALSE)
    EXCLUDE_FILES_FROM_DIR_IN_LIST("${SOURCE_FILES}" "${CMAKE_SOURCE_DIR}/src/imgui/extra_fonts" FALSE)

    find_package(SDL2)
    include_directories(${SDL2_INCLUDE_DIR})
    message(STATUS "SDL2 include directory: " ${SDL2_INCLUDE_DIR})

    add_library(InsTux SHARED ${SOURCE_FILES})
    set_target_properties(InsTux PROPERTIES COMPILE_FLAGS "-m32 --no-gnu-unique" LINK_FLAGS "-m32 --no-gnu-unique")
    target_link_libraries(InsTux SDL2 dl)
else()
    message(STATUS "Generating Makefile without ImGui (to enable GUI, run 'cmake . -DWITH_GUI=ON')")
    EXCLUDE_FILES_FROM_DIR_IN_LIST("${SOURCE_FILES}" "${CMAKE_SOURCE_DIR}/src/imgui" FALSE)
    EXCLUDE_FILES_FROM_DIR_IN_LIST("${SOURCE_FILES}" "${CMAKE_SOURCE_DIR}/src/gui" FALSE)
    add_library(InsTux SHARED ${SOURCE_FILES})
    set_target_properties(InsTux PROPERTIES COMPILE_FLAGS "-m32" LINK_FLAGS "-m32")
    target_link_libraries(InsTux dl)
endif()
