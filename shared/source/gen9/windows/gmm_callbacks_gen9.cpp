/*
 * Copyright (C) 2018-2022 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include "shared/source/gen9/hw_cmds_base.h"
#include "shared/source/helpers/windows/gmm_callbacks.h"
#include "shared/source/helpers/windows/gmm_callbacks.inl"

namespace NEO {
template struct DeviceCallbacks<SKLFamily>;
template struct TTCallbacks<SKLFamily>;
} // namespace NEO
