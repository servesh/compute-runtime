/*
 * Copyright (C) 2020-2022 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#pragma once

#include "shared/test/common/helpers/debug_manager_state_restore.h"
#include "shared/test/common/mocks/mock_compilers.h"
#include "shared/test/common/mocks/mock_device.h"
#include "shared/test/common/mocks/mock_memory_manager.h"

#include "level_zero/core/source/context/context_imp.h"
#include "level_zero/core/source/driver/driver_handle_imp.h"
#include "level_zero/core/test/unit_tests/mocks/mock_device.h"

class MockPageFaultManager;
namespace NEO {
struct UltDeviceFactory;
} // namespace NEO

namespace L0 {
struct Context;
struct Device;
struct ContextImp;

namespace ult {
class MockBuiltins;

struct MockDriverModel : NEO::DriverModel {
    size_t maxAllocSize;

    MockDriverModel(size_t maxAllocSize) : NEO::DriverModel(NEO::DriverModelType::UNKNOWN), maxAllocSize(maxAllocSize) {}

    void setGmmInputArgs(void *args) override {}
    uint32_t getDeviceHandle() const override { return {}; }
    PhysicalDevicePciBusInfo getPciBusInfo() const override { return {}; }
    size_t getMaxMemAllocSize() const override {
        return maxAllocSize;
    }

    bool isGpuHangDetected(NEO::OsContext &osContext) override {
        return false;
    }

    PhyicalDevicePciSpeedInfo getPciSpeedInfo() const override { return {}; }
};

struct MockDriverModelWDDM : NEO::DriverModel {
    size_t maxAllocSize;

    MockDriverModelWDDM(size_t maxAllocSize) : NEO::DriverModel(NEO::DriverModelType::WDDM), maxAllocSize(maxAllocSize) {}

    void setGmmInputArgs(void *args) override {}
    uint32_t getDeviceHandle() const override { return {}; }
    PhysicalDevicePciBusInfo getPciBusInfo() const override { return {}; }
    size_t getMaxMemAllocSize() const override {
        return maxAllocSize;
    }
    PhyicalDevicePciSpeedInfo getPciSpeedInfo() const override { return {}; }

    bool isGpuHangDetected(NEO::OsContext &osContext) override {
        return false;
    }
};

struct MockDriverModelDRM : NEO::DriverModel {
    size_t maxAllocSize;

    MockDriverModelDRM(size_t maxAllocSize) : NEO::DriverModel(NEO::DriverModelType::DRM), maxAllocSize(maxAllocSize) {}

    void setGmmInputArgs(void *args) override {}
    uint32_t getDeviceHandle() const override { return {}; }
    PhysicalDevicePciBusInfo getPciBusInfo() const override { return {}; }
    size_t getMaxMemAllocSize() const override {
        return maxAllocSize;
    }
    PhyicalDevicePciSpeedInfo getPciSpeedInfo() const override { return {}; }

    bool isGpuHangDetected(NEO::OsContext &osContext) override {
        return false;
    }
};

struct ContextShareableMock : public L0::ContextImp {
    ContextShareableMock(L0::DriverHandleImp *driverHandle) : L0::ContextImp(driverHandle) {}
    bool isShareableMemory(const void *pNext, bool exportableMemory, NEO::Device *neoDevice) override {
        return true;
    }
};

struct DeviceFixture {
    NEO::MockCompilerEnableGuard compilerMock = NEO::MockCompilerEnableGuard(true);
    void SetUp();    // NOLINT(readability-identifier-naming)
    void TearDown(); // NOLINT(readability-identifier-naming)
    void setupWithExecutionEnvironment(NEO::ExecutionEnvironment &executionEnvironment);

    std::unique_ptr<Mock<L0::DriverHandleImp>> driverHandle;
    NEO::MockDevice *neoDevice = nullptr;
    L0::Device *device = nullptr;
    L0::ContextImp *context = nullptr;
    MockBuiltins *mockBuiltIns = nullptr;
    NEO::ExecutionEnvironment *execEnv = nullptr;
};

struct DriverHandleGetMemHandlePtrMock : public L0::DriverHandleImp {
    void *importFdHandle(ze_device_handle_t hDevice, ze_ipc_memory_flags_t flags, uint64_t handle, NEO::GraphicsAllocation **pAloc) override {
        if (failHandleLookup) {
            return nullptr;
        }
        return &mockFd;
    }

    void *importNTHandle(ze_device_handle_t hDevice, void *handle) override {
        if (failHandleLookup) {
            return nullptr;
        }
        return &mockHandle;
    }

    uint64_t mockHandle = 57;
    int mockFd = 57;
    bool failHandleLookup = false;
};

class MemoryManagerMemHandleMock : public MockMemoryManager {
  public:
    bool isNTHandle(osHandle handle, uint32_t rootDeviceIndex) override {
        return NTHandle;
    };

    bool NTHandle = false;
};

struct GetMemHandlePtrTestFixture {
    NEO::MockCompilerEnableGuard compilerMock = NEO::MockCompilerEnableGuard(true);
    void SetUp();    // NOLINT(readability-identifier-naming)
    void TearDown(); // NOLINT(readability-identifier-naming)
    NEO::MemoryManager *prevMemoryManager = nullptr;
    MemoryManagerMemHandleMock *currMemoryManager = nullptr;
    std::unique_ptr<DriverHandleGetMemHandlePtrMock> driverHandle;
    NEO::MockDevice *neoDevice = nullptr;
    L0::Device *device = nullptr;
    std::unique_ptr<L0::ContextImp> context;
};

struct PageFaultDeviceFixture {
    NEO::MockCompilerEnableGuard compilerMock = NEO::MockCompilerEnableGuard(true);
    void SetUp();    // NOLINT(readability-identifier-naming)
    void TearDown(); // NOLINT(readability-identifier-naming)

    std::unique_ptr<Mock<L0::DriverHandleImp>> driverHandle;
    std::unique_ptr<MockMemoryManager> mockMemoryManager;
    NEO::MockDevice *neoDevice = nullptr;
    L0::Device *device = nullptr;
    L0::ContextImp *context = nullptr;
    MockPageFaultManager *mockPageFaultManager = nullptr;
    NEO::MemoryManager *memoryManager = nullptr;
};

struct MultiDeviceFixture {
    NEO::MockCompilerEnableGuard compilerMock = NEO::MockCompilerEnableGuard(true);
    void SetUp();    // NOLINT(readability-identifier-naming)
    void TearDown(); // NOLINT(readability-identifier-naming)

    DebugManagerStateRestore restorer;
    std::unique_ptr<Mock<L0::DriverHandleImp>> driverHandle;
    std::vector<NEO::Device *> devices;
    uint32_t numRootDevices = 4u;
    uint32_t numSubDevices = 2u;
    L0::ContextImp *context = nullptr;
};

struct SingleRootMultiSubDeviceFixture : public MultiDeviceFixture {
    void SetUp();

    L0::Device *device = nullptr;
    NEO::Device *neoDevice = nullptr;
};

struct ImplicitScalingRootDevice : public SingleRootMultiSubDeviceFixture {
    void SetUp() {
        DebugManager.flags.EnableImplicitScaling.set(1);
        SingleRootMultiSubDeviceFixture::SetUp();
    }
};

struct ContextFixture : DeviceFixture {
    void SetUp();
    void TearDown();
};

struct MultipleDevicesWithCustomHwInfo {
    void SetUp();      // NOLINT(readability-identifier-naming)
    void TearDown() {} // NOLINT(readability-identifier-naming)
    NEO::HardwareInfo hwInfo;
    const uint32_t numSubslicesPerSlice = 4;
    const uint32_t numEuPerSubslice = 8;
    const uint32_t numThreadsPerEu = 7;
    const uint32_t sliceCount = 2;
    const uint32_t subsliceCount = 8;

    std::unique_ptr<Mock<L0::DriverHandleImp>> driverHandle;
    NEO::OsAgnosticMemoryManager *memoryManager = nullptr;
    std::unique_ptr<UltDeviceFactory> deviceFactory;

    const uint32_t numRootDevices = 1u;
    const uint32_t numSubDevices = 2u;
};

template <uint32_t copyEngineCount, uint32_t implicitScaling>
struct SingleRootMultiSubDeviceFixtureWithImplicitScaling : public MultiDeviceFixture {
    NEO::MockCompilerEnableGuard compilerMock = NEO::MockCompilerEnableGuard(true);

    DebugManagerStateRestore restorer;
    std::unique_ptr<Mock<L0::DriverHandleImp>> driverHandle;
    std::vector<NEO::Device *> devices;
    uint32_t numRootDevices = 1u;
    uint32_t numSubDevices = 2u;
    L0::ContextImp *context = nullptr;

    L0::Device *device = nullptr;
    NEO::Device *neoDevice = nullptr;
    L0::DeviceImp *deviceImp = nullptr;

    NEO::HardwareInfo hwInfo;
    uint32_t expectedCopyEngineCount = copyEngineCount;
    uint32_t expectedComputeEngineCount = 0;

    uint32_t numEngineGroups = 0;
    uint32_t subDeviceNumEngineGroups = 0;

    void SetUp() {
        DebugManagerStateRestore restorer;
        DebugManager.flags.EnableImplicitScaling.set(implicitScaling);
        DebugManager.flags.CreateMultipleRootDevices.set(numRootDevices);
        DebugManager.flags.CreateMultipleSubDevices.set(numSubDevices);

        NEO::HardwareInfo hwInfo = *NEO::defaultHwInfo.get();
        hwInfo.featureTable.flags.ftrRcsNode = false;
        hwInfo.featureTable.flags.ftrCCSNode = true;
        // hwInfo.gtSystemInfo.CCSInfo.NumberOfCCSEnabled = 4;
        if (expectedCopyEngineCount != 0) {
            hwInfo.capabilityTable.blitterOperationsSupported = true;
            hwInfo.featureTable.ftrBcsInfo = maxNBitValue(expectedCopyEngineCount);
        } else {
            hwInfo.capabilityTable.blitterOperationsSupported = false;
        }

        if (implicitScaling) {
            expectedComputeEngineCount = 1u;
        } else {
            expectedComputeEngineCount = hwInfo.gtSystemInfo.CCSInfo.NumberOfCCSEnabled;
        }

        MockDevice *mockDevice = MockDevice::createWithNewExecutionEnvironment<MockDevice>(&hwInfo, 0);

        NEO::DeviceVector devices;
        devices.push_back(std::unique_ptr<NEO::Device>(mockDevice));

        driverHandle = std::make_unique<Mock<L0::DriverHandleImp>>();
        ze_result_t res = driverHandle->initialize(std::move(devices));
        EXPECT_EQ(ZE_RESULT_SUCCESS, res);

        ze_context_handle_t hContext;
        ze_context_desc_t desc = {ZE_STRUCTURE_TYPE_CONTEXT_DESC, nullptr, 0};
        res = driverHandle->createContext(&desc, 0u, nullptr, &hContext);
        EXPECT_EQ(ZE_RESULT_SUCCESS, res);
        context = static_cast<ContextImp *>(Context::fromHandle(hContext));

        device = driverHandle->devices[0];
        neoDevice = device->getNEODevice();
        deviceImp = static_cast<L0::DeviceImp *>(device);

        NEO::Device *activeDevice = deviceImp->getActiveDevice();
        auto &engineGroups = activeDevice->getRegularEngineGroups();
        numEngineGroups = static_cast<uint32_t>(engineGroups.size());

        if (activeDevice->getSubDevices().size() > 0) {
            NEO::Device *activeSubDevice = activeDevice->getSubDevice(0u);
            (void)activeSubDevice;
            auto &subDeviceEngineGroups = activeSubDevice->getRegularEngineGroups();
            (void)subDeviceEngineGroups;

            for (uint32_t i = 0; i < subDeviceEngineGroups.size(); i++) {
                if (subDeviceEngineGroups[i].engineGroupType == NEO::EngineGroupType::Copy ||
                    subDeviceEngineGroups[i].engineGroupType == NEO::EngineGroupType::LinkedCopy) {
                    subDeviceNumEngineGroups += 1;
                }
            }
        }
    }

    void TearDown() {
        context->destroy();
    }
};

} // namespace ult
} // namespace L0
