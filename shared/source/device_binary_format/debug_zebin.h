/*
 * Copyright (C) 2021-2022 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#pragma once

#include "shared/source/device_binary_format/elf/zebin_elf.h"
#include "shared/source/utilities/arrayref.h"
#include "shared/source/utilities/stackvec.h"

#include <limits>
#include <string>
#include <unordered_map>
#include <vector>

namespace NEO {
class GraphicsAllocation;
namespace Debug {
struct Segments {
    struct Segment {
        uintptr_t address = std::numeric_limits<uintptr_t>::max();
        size_t size = 0;
    };
    using CPUSegment = Segment;
    using GPUSegment = Segment;
    using KernelNameIsaPairT = std::pair<std::string_view, GraphicsAllocation *>;
    using KernelNameToSegmentMap = std::unordered_map<std::string, GPUSegment>;

    GPUSegment varData;
    GPUSegment constData;
    CPUSegment stringData;
    KernelNameToSegmentMap nameToSegMap;
    Segments();
    Segments(const GraphicsAllocation *globalVarAlloc, const GraphicsAllocation *globalConstAlloc, ArrayRef<const uint8_t> &globalStrings, std::vector<KernelNameIsaPairT> &kernels);
};

class DebugZebinCreator {
  public:
    using Elf = NEO::Elf::Elf<NEO::Elf::EI_CLASS_64>;
    DebugZebinCreator() = delete;
    DebugZebinCreator(Elf &zebin, const Segments &segments) : segments(segments), zebin(zebin) {}

    void applyRelocations();
    void createDebugZebin();
    inline std::vector<uint8_t> getDebugZebin() { return debugZebin; }

  protected:
    void applyRelocation(uint64_t addr, uint64_t value, NEO::Elf::RELOC_TYPE_ZEBIN type);
    const Segments::Segment *getSegmentByName(ConstStringRef sectionName);
    const Segments::Segment *getTextSegmentByName(ConstStringRef sectionName);
    bool isRelocTypeSupported(NEO::Elf::RELOC_TYPE_ZEBIN type);

    std::vector<uint8_t> debugZebin;
    const Segments &segments;
    Elf &zebin;
};

std::vector<uint8_t> createDebugZebin(ArrayRef<const uint8_t> zebin, const Segments &segmentData);
} // namespace Debug
} // namespace NEO
