/*
 * Copyright (C) 2021-2022 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#pragma once

#include "shared/test/common/fixtures/device_fixture.h"
#include "shared/test/common/mocks/mock_memory_manager.h"
#include "shared/test/common/test_macros/test.h"

namespace NEO {

class MemManagerFixture : public DeviceFixture {
  public:
    struct FrontWindowMemManagerMock : public MockMemoryManager {
        FrontWindowMemManagerMock(NEO::ExecutionEnvironment &executionEnvironment);
        void forceLimitedRangeAllocator(uint32_t rootDeviceIndex, uint64_t range);
    };

    void SetUp();
    void TearDown();

    std::unique_ptr<FrontWindowMemManagerMock> memManager;
};
} // namespace NEO
