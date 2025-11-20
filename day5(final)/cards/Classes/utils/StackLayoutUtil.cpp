#include "StackLayoutUtil.h"
#include "cocos2d.h"

USING_NS_CC;

namespace StackLayoutUtil
{
    void applyStackLayout(std::vector<LevelCardConfig>& stackCards)
    {
        const int n = static_cast<int>(stackCards.size());
        if (n <= 0) return;

        // 设计分辨率下的屏幕尺寸
        auto visibleSize = Director::getInstance()->getVisibleSize();
        const float screenWidth = visibleSize.width;
        const float screenHeight = visibleSize.height;

        // 和 GameView 保持一致：主牌区高度 1500，其余是 stack 区
        const float playfieldHeight = 1500.0f;
        const float stackHeight = screenHeight - playfieldHeight;

        const float baseY = stackHeight * 0.5f;   // stack 区域的中线

        const float kTopGap = 250.0f;  // 顶牌相对其它牌多出来的右移
        const float kOverlapX = 100.0f;  // 其它牌之间的水平距离

        if (n == 1)
        {
            // 只有一张牌：直接放在 stack 区中间
            stackCards[0].position = Vec2(screenWidth * 0.5f, baseY);
            return;
        }

        const int   stackedCount = n - 1;
        const float stackedWidth = (stackedCount - 1) * kOverlapX; // 第一张到第 n-1 张之间的跨度
        const float totalWidth = stackedWidth + kTopGap;         // 最左牌到顶牌之间的总水平跨度

        // 整体水平居中：找到最左一张牌的中心 x
        const float centerX = screenWidth * 0.5f;
        const float firstX = centerX - totalWidth * 0.5f;

        // 先排前 n-1 张
        for (int i = 0; i < stackedCount; ++i)
        {
            const float x = firstX + i * kOverlapX;
            stackCards[i].position = Vec2(x, baseY);
        }

        // 顶牌在最右边，比上一张多一个 kTopGap
        const float topX = firstX + stackedWidth + kTopGap;
        stackCards[n - 1].position = Vec2(topX, baseY);
    }
}
