#pragma once

#include <cstdint>

enum class TileID : uint64_t {  // NOLINT
  kVoid = 0,
  kInvisibleBarrier,
  kDirtTopLeft,
  kDirtTopCenter,
  kDirtTopRight,
  kDirtMiddleLeft,
  kDirtMiddleCenter,
  kDirtMiddleRight,
  kDirtBottomLeft,
  kDirtBottomCenter,
  kDirtBottomRight,
  kStoneHorizontalLeft,
  kStoneHorizontalCenter,
  kStoneHorizontalRight,
  kStoneVerticalTop,
  kStoneVerticalMiddle,
  kStoneVerticalBottom,
  kPlasticBlock,
};
