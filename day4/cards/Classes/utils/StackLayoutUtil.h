// utils/StackLayoutUtil.h
#pragma once

#include <vector>
#include "configs/models/LevelConfig.h"

// 手牌区排布相关的小工具
namespace StackLayoutUtil
{
    // 根据关卡中的 _stackCards 数量，计算它们在屏幕上的位置
    // 要求：
    // 顶部牌在最右边，比其它牌右移约 250 像素
    // 其它牌之间水平间距约 135 像素（堆叠效果）
    // 整体牌堆在 stack 区域中间
    void applyStackLayout(std::vector<LevelCardConfig>& stackCards);
}
