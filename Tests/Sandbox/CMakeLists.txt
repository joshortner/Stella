cmake_minimum_required(VERSION 3.7.1)
set(CMAKE_CXX_STANDARD 20)

project(sandbox)

##### SET VARIABLES
set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

##### STELLA
set(STELLA_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../Stella)
add_subdirectory(${STELLA_DIR} ${STELLA_DIR}/build/lib)

##### INCLUDE
include_directories(
	${SRC_DIR}
	${STELLA_DIR}/src ##### STELLA INCLUDE
	${IMGUI_DIR}
)

##### SOURCE FILES 
file(
	GLOB SOURCES
	${SRC_DIR}/main.cpp
	${SRC_DIR}/Sandbox/Startup/*cpp
	${SRC_DIR}/Sandbox/Utils/*cpp

	##### Native Scripts #####
	${SRC_DIR}/Sandbox/NativeScripts/PointSourceScript/*cpp
)

##### EXECUTABLE
add_executable(
	${PROJECT_NAME}
	${SOURCES}
)

##### LINK
target_link_libraries(
	${PROJECT_NAME}
	stella
)

add_compile_definitions(PROJECT_SRC_DIR="${CMAKE_CURRENT_SOURCE_DIR}" PROJECT_SRC_DIR_SET)
