#
# Copyright (C) 2019-2022 Intel Corporation
#
# SPDX-License-Identifier: MIT
#

set(NEO_CORE_OS_INTERFACE_TESTS_WINDOWS
    ${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt
    ${CMAKE_CURRENT_SOURCE_DIR}/adapter_info_tests.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/adapter_info_tests.h
    ${CMAKE_CURRENT_SOURCE_DIR}/gdi_interface_tests.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/gmm_helper_tests_win.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/hw_info_config_win_tests.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/hw_info_config_win_tests.h
    ${CMAKE_CURRENT_SOURCE_DIR}/um_km_data_translator_tests.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/wddm_preemption_tests.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/wddm_special_heap_test.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/wddm_tests.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/wddm_shared_allocations_test.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/wddm_command_stream_l0_tests.cpp
)

set_property(GLOBAL PROPERTY NEO_CORE_OS_INTERFACE_TESTS_WINDOWS ${NEO_CORE_OS_INTERFACE_TESTS_WINDOWS})

if(WIN32)
  target_sources(${TARGET_NAME} PRIVATE
                 ${NEO_CORE_OS_INTERFACE_TESTS_WINDOWS}
  )
endif()

add_subdirectories()
