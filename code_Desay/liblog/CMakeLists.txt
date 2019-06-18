
####################################################################################################

# Company       : Huizhou Desay SV Automotive Co., Ltd.

# Division      : Automotive Electronics, Desay Corporation

# Business Unit : Central Technology

# Department    : Advanced Development (Huizhou)

####################################################################################################

# 0. CMake Defs

cmake_minimum_required(VERSION 2.6 FATAL_ERROR)

cmake_policy(VERSION 2.6)

# 1. Project Name

project(liblog)

# 2. Project Env (Include/Lib Path, C/CXX/LD FLAGS)

add_definitions("-DDEV")

# 3. Project Build

set(LIB_NAME_LOG "svp_log")

set(LIB_SRC_FILES
        log.c)

add_library(${LIB_NAME_LOG} SHARED ${LIB_SRC_FILES})

set_target_properties(${LIB_NAME_LOG} PROPERTIES CLEAN_DIRECT_OUTPUT 1)

target_link_libraries(${LIB_NAME_LOG} pthread rt)

set(LIB_NAME_REC "svp_record")

set(LIB_SRC_FILES
        Recorder.h Recorder.cpp JsonMgr.h JsonMgr.cpp)

add_library(${LIB_NAME_REC} SHARED ${LIB_SRC_FILES})

set_target_properties(${LIB_NAME_REC} PROPERTIES CLEAN_DIRECT_OUTPUT 1)

target_link_libraries(${LIB_NAME_REC} PocoJSON svp_log)

# 4. Project Install

install(FILES $ENV{SVP_PLATFORM}.svp.log.conf.orig
        DESTINATION ${CMAKE_INSTALL_SYSCONFDIR}
        RENAME svp.log.conf.orig)

install(FILES $ENV{SVP_PLATFORM}.data.json
        DESTINATION ${CMAKE_INSTALL_SYSCONFDIR})

install(FILES log.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

install(FILES Recorder.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

install(TARGETS ${LIB_NAME_LOG} ${LIB_NAME_REC}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR})
