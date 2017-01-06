cmake_minimum_required(VERSION 3.0)
set(CMAKE_CXX_STANDARD 11) # C++11...
set(CMAKE_CXX_STANDARD_REQUIRED ON) #...is required...
set(CMAKE_CXX_EXTENSIONS OFF) #...without compiler extensions like gnu++11
#set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "cmake")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

project(mysdlproject)

set (CMAKE_BUILD_TYPE Debug)


find_package(SDL2 REQUIRED)
find_package(SDL2_imgae REQUIRED)
#find_package(SDL_image REQUIRED)
message(STATUS SDL2 is in ${SDL2_INCLUDE_DIR})
message(STATUS SDL2_image is in ${SDL2_IMAGE_INCLUDE_DIR})

set (SOURCE 
src/director.cpp 
src/director.h 
src/utils/clock.h 
src/main.cpp 
src/node.cpp 
src/nodedrawable.cpp 
src/node.h 
src/texturemanager.cpp
src/texturemanager.h 

)

add_executable(${PROJECT_NAME} ${SOURCE})
include_directories(${SDL2_INCLUDE_DIR} ${SDL2_IMAGE_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARY} ${SDL2_IMAGE_LIBRARY})

add_custom_command(TARGET ${PROJECT_NAME} PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory
                       ${CMAKE_SOURCE_DIR}/assets $<TARGET_FILE_DIR:${PROJECT_NAME}>/assets)