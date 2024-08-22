cmake_minimum_required(VERSION 3.5)
set(CMAKE_CXX_STANDARD 11)
# Very important, otherwise your executable will be considered as a shared library by your OS and not as an executable
set(CMAKE_CXX_FLAGS "-no-pie")
set(PROJECT_NAME "$ENV{EXE_TITLE}")
project(${PROJECT_NAME})

# sudo apt install libqt5charts5-dev for Charts
# sudo apt install qttools5-dev for UiTools
find_package(Qt5 COMPONENTS Core UiTools Widgets Charts REQUIRED)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(PERMISSIONS OWNER_READ 
    OWNER_EXECUTE
    OWNER_READ
    OWNER_WRITE
    GROUP_EXECUTE 
    WORLD_EXECUTE
)


if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_definitions(DEBUG)
endif()

include_directories(${PROJECT_SOURCE_DIR}/../dev/include)
include_directories(${PROJECT_SOURCE_DIR}/../lib/NeuralNetwork/include)

file(GLOB_RECURSE SRCS ${PROJECT_SOURCE_DIR}/../dev/*.cpp)
file(GLOB_RECURSE HDRS ${PROJECT_SOURCE_DIR}/../dev/*.hpp)

add_executable(${PROJECT_NAME}
${SRCS}
${HDRS})

# $<$<CONFIG:Release>:release> => if CONFIG equal "Release" then replace with "release"
target_link_libraries(${PROJECT_NAME}
    Qt5::Widgets
    Qt5::UiTools
    Qt5::Charts
	sfml-graphics
	sfml-window
	sfml-system 
	sfml-audio
	sfml-network
	pthread
    ${PROJECT_SOURCE_DIR}/../lib/NeuralNetwork/bin/$<$<CONFIG:Debug>:debug>$<$<CONFIG:Release>:release>/NeuralNetwork.a
    
)
	
install(FILES ${PROJECT_NAME}  DESTINATION ${PROJECT_SOURCE_DIR}/.. PERMISSIONS ${PERMISSIONS} )
