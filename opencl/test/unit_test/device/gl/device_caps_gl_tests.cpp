/*
 * Copyright (C) 2018-2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include "shared/source/debug_settings/debug_settings_manager.h"
#include "shared/source/helpers/basic_math.h"
#include "shared/source/helpers/hw_info.h"
#include "shared/source/memory_manager/os_agnostic_memory_manager.h"
#include "shared/test/common/helpers/debug_manager_state_restore.h"

#include "opencl/test/unit_test/fixtures/cl_device_fixture.h"
#include "test.h"

#include "gmock/gmock.h"

#include <memory>

using namespace NEO;

TEST(Device_GetCaps, givenForceClGlSharingWhenCapsAreCreatedThenDeviceReportsClGlSharingExtension) {
    DebugManagerStateRestore dbgRestorer;
    {
        DebugManager.flags.AddClGlSharing.set(true);
        auto device = std::make_unique<MockClDevice>(MockDevice::createWithNewExecutionEnvironment<MockDevice>(defaultHwInfo.get()));
        const auto &caps = device->getDeviceInfo();

        EXPECT_THAT(caps.deviceExtensions, testing::HasSubstr(std::string("cl_khr_gl_sharing ")));
        DebugManager.flags.AddClGlSharing.set(false);
    }
}
