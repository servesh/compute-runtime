/*
 * Copyright (C) 2021-2022 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include "level_zero/core/source/helpers/l0_populate_factory.h"
#include "level_zero/core/source/hw_helpers/l0_hw_helper_base.inl"
#include "level_zero/core/source/hw_helpers/l0_hw_helper_pvc_and_later.inl"

#include "hw_cmds.h"

namespace L0 {

using Family = NEO::XE_HPC_COREFamily;
static auto gfxCore = IGFX_XE_HPC_CORE;

template <>
void populateFactoryTable<L0HwHelperHw<Family>>() {
    extern L0HwHelper *l0HwHelperFactory[IGFX_MAX_CORE];
    l0HwHelperFactory[gfxCore] = &L0HwHelperHw<Family>::get();
}

template <>
bool L0HwHelperHw<Family>::isIpSamplingSupported(const NEO::HardwareInfo &hwInfo) const {
    if (hwInfo.platform.usDeviceID == NEO::XE_HPC_CORE::pvcXtDeviceIds[0] ||
        hwInfo.platform.usDeviceID == NEO::XE_HPC_CORE::pvcXtDeviceIds[1] ||
        hwInfo.platform.usDeviceID == NEO::XE_HPC_CORE::pvcXtDeviceIds[2] ||
        hwInfo.platform.usDeviceID == NEO::XE_HPC_CORE::pvcXtDeviceIds[3] ||
        hwInfo.platform.usDeviceID == NEO::XE_HPC_CORE::pvcXtDeviceIds[4]) {
        return true;
    }
    return false;
}

template class L0HwHelperHw<Family>;

} // namespace L0
