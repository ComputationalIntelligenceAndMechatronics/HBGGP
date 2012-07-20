cmake_minimum_required(VERSION 2.4)
if(COMMAND cmake_policy)
		   cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)


project(PACC C CXX)
set(PACC_VERSION 1.2.1)

set( CMAKE_VERBOSE_MAKEFILE ON )

option( WITH_GSL "Build the project using GNU Scientific Library" ON )
if( WITH_GSL )
	set(GSL_DIR "$ENV{HOME}/local")
	set(GSL_INCLUDE_DIR "${GSL_DIR}/include")
	set(GSL_LIB_DIR "${GSL_DIR}/lib")
	add_definitions(-DHAVE_GSL)
	set( PACC_LIBS  gsl gslcblas)
	link_directories( ${GSL_LIB_DIR} )
	include_directories( ${GSL_INCLUDE_DIR} )
endif( WITH_GSL )




file(GLOB_RECURSE PACC_SRC  PACC/*.cpp)
file(REMOVE_RECURSE PACC_SRC  PACC/SVG/*.cpp)

file(GLOB PACC_HEADERS     	PACC/*.hpp)
file(REMOVE_RECURSE PACC_HEADERS     	PACC/SVG/*.hpp)
file(REMOVE PACC_HEADERS     	PACC/SVG.hpp)

file(GLOB PACC_MATH_HEADERS PACC/Math/*.hpp)
file(GLOB PACC_UTIL_HEADERS PACC/Util/*.hpp)
file(GLOB PACC_XML_HEADERS   PACC/XML/*.hpp)
file(GLOB PACC_SOCKET_HEADERS PACC/Socket/*.hpp)
file(GLOB PACC_THREADING_HEADERS PACC/Threading/*.hpp)

# Include headers required
include_directories(${CMAKE_HOME_DIRECTORY})

add_library(pacc SHARED ${PACC_SRC})
add_dependencies(pacc pthread ${PACC_LIBS} )
target_link_libraries(pacc pthread ${PACC_LIBS})
set_target_properties(pacc PROPERTIES VERSION ${PACC_VERSION})
set_target_properties(pacc PROPERTIES LINKER_LANGUAGE CXX)


# Install dependencies
install(FILES ${PACC_HEADERS} DESTINATION include/PACC)
install(FILES ${PACC_MATH_HEADERS} DESTINATION include/PACC/Math)
install(FILES ${PACC_UTIL_HEADERS} DESTINATION include/PACC/Util)
install(FILES ${PACC_XML_HEADERS} DESTINATION include/PACC/XML)
install(FILES ${PACC_SOCKET_HEADERS} DESTINATION include/PACC/Socket)
install(FILES ${PACC_THREADING_HEADERS} DESTINATION include/PACC/Threading)

# Install targets
install(TARGETS pacc    DESTINATION lib)

