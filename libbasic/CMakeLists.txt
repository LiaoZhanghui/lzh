
####################################################################################################

# Company       : Huizhou Desay SV Automotive Co., Ltd.

# Division      : Automotive Electronics, Desay Corporation

# Business Unit : Central Technology

# Department    : Advanced Development (Huizhou)

####################################################################################################

# 1. Project Name

project(libbasic)

# 2. Project Env (Include/Lib Path, C/CXX/LD FLAGS)

include_directories("include")

include_directories("${libutf_SOURCE_DIR}")

# 3. Project Build

set(LIB_NAME "svp_basic")

set(LIB_SRC_FILES
        src/SVPFile.cpp
        src/SVPString.cpp
        src/SVPTime.cpp
	src/SVPThread.cpp
        src/SVPMessage.cpp
        src/SVPSomeIP.cpp
        src/SVPSystem.cpp
        src/SVPUnicode.cpp
        src/SVPUtils_Signal.cpp
        src/SVPUtils_BackTrace.cpp)

add_library(${LIB_NAME} SHARED ${LIB_SRC_FILES})

set_target_properties(${LIB_NAME} PROPERTIES CLEAN_DIRECT_OUTPUT 1)

target_link_libraries(${LIB_NAME}
        utf PocoFoundation
        pthread m rt dl svp_log vsomeip)

# 4. Project Install

install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

install(TARGETS ${LIB_NAME}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR})
