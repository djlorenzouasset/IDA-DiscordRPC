cmake_minimum_required(VERSION 3.27)

if(NOT DEFINED ENV{IDASDK} OR "$ENV{IDASDK}" STREQUAL "")
  set(ENV{IDASDK} "${CMAKE_CURRENT_LIST_DIR}/IDA-DiscordRPC/ida-sdk")
endif()

set(_idasdk_bootstrap "$ENV{IDASDK}/cmake/idasdk_init.cmake")
if(NOT EXISTS "${_idasdk_bootstrap}")
  set(_idasdk_bootstrap "$ENV{IDASDK}/cmake/bootstrap.cmake")
endif()
if(NOT EXISTS "${_idasdk_bootstrap}")
  set(_idasdk_bootstrap "$ENV{IDASDK}/src/cmake/bootstrap.cmake")
endif()
if(NOT EXISTS "${_idasdk_bootstrap}")
  message(FATAL_ERROR "Could not locate bootstrap.cmake or idasdk_init.cmake under '$ENV{IDASDK}'. Set IDASDK to a valid IDA SDK root.")
endif()

include("${_idasdk_bootstrap}")

project(IDA-DiscordRPC)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(idasdk REQUIRED)

ida_add_plugin("IDA-DiscordRPC64"
  SOURCES
    IDA-DiscordRPC/main.cpp
    IDA-DiscordRPC/settings/form_manager.cpp
    IDA-DiscordRPC/settings/plugin_settings.cpp
    IDA-DiscordRPC/utils/discord.cpp
    IDA-DiscordRPC/utils/ida.cpp
  INCLUDES
    "${CMAKE_CURRENT_LIST_DIR}/IDA-DiscordRPC/discord-sdk"
  LIBRARIES
    "${CMAKE_CURRENT_LIST_DIR}/IDA-DiscordRPC/discord-sdk/discord-rpc.lib"
  OUTPUT_NAME IDA-DiscordRPC64
)

set_target_properties(IDA-DiscordRPC64 PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)
