cmake_minimum_required(VERSION 3.22)
project(libTGParserCpp VERSION 1.0.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)

option(BUILD_SHARED "Whether to build the shared library" ON)
option(BUILD_STATIC "Whether to build the static library" ON)
option(COMPILE_PROJECTS "Build test" OFF)

#include_directories(src)
#include_directories(tests)
FILE(GLOB THINKGEAR_SOURCES src/*.cpp)
set (THINKGEAR_HEADERS
     src/ThinkGearStreamParser.h
    )
set (LIBNAME tgparser)
set (HEADERS_DIR tgparser)
if(BUILD_STATIC)
    message("Build static library")
    add_library (${LIBNAME} STATIC ${THINKGEAR_SOURCES})    
    target_include_directories(${LIBNAME} PRIVATE include PUBLIC $<INSTALL_INTERFACE:include>)
    set_target_properties(${LIBNAME} PROPERTIES PUBLIC_HEADER "${THINKGEAR_HEADERS}")
    list(APPEND THINKGEAR_TARGETS ${LIBNAME})
endif()
if(BUILD_SHARED)
    message("Build shared library")
    add_compile_definitions(EXPORT)
    add_library (${LIBNAME}_SHARED SHARED ${THINKGEAR_SOURCES})
    set_target_properties(${LIBNAME}_SHARED PROPERTIES OUTPUT_NAME ${LIBNAME})
    target_include_directories(${LIBNAME}_SHARED PRIVATE include PUBLIC $<INSTALL_INTERFACE:include>)
    set_target_properties(${LIBNAME}_SHARED PROPERTIES PUBLIC_HEADER "${THINKGEAR_HEADERS}")
    list(APPEND THINKGEAR_TARGETS ${LIBNAME}_SHARED)
endif()

install(FILES ${THINKGEAR_HEADERS} DESTINATION include/${HEADERS_DIR})
install(TARGETS ${THINKGEAR_TARGETS} EXPORT ${LIBNAME}-config
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
	RUNTIME DESTINATION bin
	PUBLIC_HEADER DESTINATION include/${HEADERS_DIR})

install(EXPORT ${LIBNAME}-config DESTINATION lib/cmake/${HEADERS_DIR})

#if (COMPILE_PROJECTS)
#    message("COMPILE PROJECTS!")
#    add_subdirectory(projects/parse_test)
#    add_subdirectory(projects/device_test)
#endif()
