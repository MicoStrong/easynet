# ----------------------------------------------------------------------------
#  Root CMake file for easynet
#
#    From the off-tree build directory, invoke:
#      $ cmake <PATH_TO_EASYNET_ROOT>
#
# ----------------------------------------------------------------------------

# 禁用 in-source 构建,防止破坏源码树
if(" ${CMAKE_SOURCE_DIR}" STREQUAL " ${CMAKE_BINARY_DIR}")
    message(FATAL_ERROR "
FATAL: In-source builds are not allowed.
       You should create a separate directory for build files.
")
endif()

# 设置cmake最低版本号
cmake_minimum_required(VERSION 3.16)

# 设置项目名,语言为C++
project (easynet LANGUAGES CXX)

# 向用户提供选项,是否编译easynet库,默认不编译成库
option(USE_EASYNET_LIBRARY "Compile sources into a library" OFF)
message(STATUS "Compile easynet sources into a library? ${USE_EASYNET_LIBRARY}")

# 为了使用cmake_dependent_option
include(CMakeDependentOption)

# USE_EASYNET_LIBRARY为ON时,默认不生成静态库
cmake_dependent_option(
        MAKE_EASYNET_STATIC_LIBRARY "Compile sources into a static library" OFF
        "USE_EASYNET_LIBRARY" ON
)

# USE_EASYNET_LIBRARY为ON时,默认生成动态库
cmake_dependent_option(
        MAKE_EASYNET_SHARED_LIBRARY "Compile sources into a shared library" ON
        "USE_EASYNET_LIBRARY" ON
)

# 生成easynet库所依赖的文件
list(APPEND _sources
        include/socket_options.hpp
        include/socket_family.hpp
        include/socket.hpp
        src/socket.cpp
        )

message(STATUS "Compile sources into a STATIC library? ${MAKE_EASYNET_STATIC_LIBRARY}")
message(STATUS "Compile sources into a SHARED library? ${MAKE_EASYNET_SHARED_LIBRARY}")
if(USE_LIBRARY)
    if(MAKE_EASYNET_SHARED_LIBRARY)
        add_library(easynet SHARED ${_sources})
        add_executable(srv tcp_server.cpp)
        target_link_libraries(srv easynet)
    endif()

    if(MAKE_EASYNET_STATIC_LIBRARY)
        add_library(easynet STATIC ${_sources})
        add_executable(srv tcp_server.cpp)
        target_link_libraries(srv easynet)
    endif()

    set_target_properties(srv
            PROPERTIES
            CXX_STANDARD 14
            CXX_EXTENSIONS OFF
            CXX_STANDARD_REQUIRED ON
            POSITION_INDEPENDENT_CODE 1
            )
else()
    add_executable(srv tcp_server.cpp ${_sources})
    set_target_properties(srv
            PROPERTIES
            CXX_STANDARD 14
            CXX_EXTENSIONS OFF
            CXX_STANDARD_REQUIRED ON
            POSITION_INDEPENDENT_CODE 1
            )
endif()

