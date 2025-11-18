# Cocos2d-x 纸牌小游戏（需求一）

本项目是基于 Cocos2d-x 的纸牌游戏课程作业，目标是在限定时间内，从零搭建一个简单但结构清晰的卡牌游戏 Demo，并尽量按照 MVC 思路组织代码结构。

## 项目概述

- 引擎：Cocos2d-x
- 语言：C++
- 架构：简单的 MVC（GameScene / GameView / GameModel / Controller）
- 核心要素：
  - 卡牌模型：点数、花色、在场景中的位置
  - 卡牌视图：底牌贴图 + 大号数字 + 小号数字 + 花色图标
  - 关卡配置：从 JSON 读取牌面布局
  - 基本交互：点击牌、后续将接入出牌逻辑与撤销（Undo）

## 目录结构（简要）

> 实际目录以工程为准，这里只列出和本次需求密切相关的部分。

- `Classes/`
  - `scenes/GameScene.*`：入口场景，挂载 GameView
  - `views/GameView.*`：负责整块游戏画面的布局（主牌区 + 底牌区）
  - `views/CardView.*`：一张可视化的卡牌（Sprite 组合）
  - `models/CardModel.*`：卡牌数据模型（点数、花色、ID 等）
  - `configs/models/LevelConfig.*`：关卡静态配置（从 JSON 解析后的数据结构）
  - `configs/loaders/LevelConfigLoader.*`：读取 JSON 并生成 LevelConfig
  - `utils/CardResUtil.*`：根据点数/花色生成资源路径、辅助判断红黑等
- `Resources/`
  - `res/card_bg.png`：空白底牌
  - `res/number/`：大/小数字贴图（big_red_5.png / small_black_A.png 等）
  - `res/suits/`：花色贴图（heart.png / spade.png 等）
  - `configs/levels/level1.json`：示例关卡配置

## 开发进度

### Day 1 - 环境搭建 & 框架初建

- 完成 Cocos2d-x 开发环境配置，能够成功编译运行空工程。
- 了解基本引擎结构（`Director` / `Scene` / `Layer` / `Node`）。
- 搭建基础 MVC 思路：
  - `GameScene` 作为场景入口。
  - 预留 `GameView`、`GameModel`、`Controller` 的职责划分。

### Day 2 - GameScene & 基础 CardView

- 创建 `GameScene` 和 `GameView`，建立主牌区和底牌区两个基础布局。
- 实现最简单的 `CardModel` / `CardView`：
  - 通过 `DrawNode` 或简单 Sprite，在屏幕上画出一张“调试用”的卡牌。
  - 初步理解 `visibleSize`、`ContentSize`、锚点（AnchorPoint）等概念。
- 确认从 `GameScene` 挂载 `GameView`，并在 `GameView` 中创建并显示单张卡牌。

### Day 3 - 完整卡牌 & 事件监听（2025-11-18）

**1. 完整卡牌模型与显示**

- 将卡牌从“纯色矩形”升级为“真正的纸牌”：
  - 使用 `CardModel + CardView` 组合：
    - 底牌背景：`card_bg.png`
    - 大号数字贴图：`res/number/big_*.png`
    - 小号数字 + 花色贴图：`res/number/small_*.png` + `res/suits/*.png`
- 处理卡牌大小与缩放：
  - 基于底牌原始贴图大小，按目标高度计算 `scale`，等比放大。
  - 同步更新 `CardView::ContentSize`，保证点击区域和布局计算与实际显示一致。
- 系统性理解各类坐标：
  - 世界坐标（`touch->getLocation()`）
  - 节点本地坐标（`convertToNodeSpace`）
  - 不同 Anchor 下的位置含义及影响。

**2. 资源路径逻辑重构**

- 将“根据点数/花色 → 资源文件名”的多处 `switch` 抽离到工具类中：
  - 新增 `CardResUtil`（命名可根据工程实际为 `CardResUtil.*` 或类似）：
    - `faceToSymbol(CardFaceType)`：`A/2/.../10/J/Q/K`
    - `isRedSuit(CardSuitType)`：判断红牌/黑牌
    - `suitToTexturePath(CardSuitType)`：返回花色贴图路径
    - `bigFaceTexturePath(face, suit)` / `smallFaceTexturePath(face, suit)`
- 好处：
  - 避免在 `CardView` 中硬编码复杂逻辑。
  - 资源命名或美术资源变更时，只需修改 util 一处。

**3. Cocos2d-x 事件系统与点击监听**

- 学习 Cocos2d-x 中触摸/点击事件的整体流程：
  - 引擎内部从操作系统接收输入 → 封装为 `Touch` & `Event` → 使用 `EventDispatcher` 分发。
  - 通过 `EventListenerTouchOneByOne` 注册单点触摸监听。
- 在 View 中实际接入：
  - 在 `CardView::init` 中创建监听器并绑定：
    - `onTouchBegan(Touch*, Event*)`
    - `onTouchEnded(Touch*, Event*)`
  - 通过：
    - `touch->getLocation()` 获取点击世界坐标。
    - `convertToNodeSpace()` 转换到当前卡牌的本地坐标。
    - 使用 `Rect(0,0,width,height).containsPoint(localPos)` 做命中检测。
  - 为后续通过回调将“点击事件”传递给 Controller 做好准备（`std::function<void(CardView*)>` 形式的回调）。

**4. 当前尚未完全理解的部分**

- `LevelConfigLoader` 中 JSON 解析逻辑：
  - 使用 AI 辅助生成了 `LevelConfigLoader::loadLevelConfig` 的基本实现。
  - 能够正确从 `level1.json` 读取牌面数据，并在场景中展示。
  - 但对 rapidjson 的细节（`Document` / `Value` / 数组访问等）尚未完全吃透，计划在笔试结束后进行二次阅读与重构。

## TODO / 下一步计划

1. **卡牌点击回调与 Controller 逻辑**
   - 将 `CardView` 中的点击事件，通过回调传递至控制层（例如 `GameController`）。
   - 完成基础规则判断：哪些牌是可选的、点击后如何更新模型和视图。
   - 建立最小的选中/取消选中、高亮反馈。

2. **撤销（Undo）逻辑**
   - 为每一步操作记录操作栈（从 GameModel 的角度记录状态变化）。
   - 支持至少一层/多层撤销，满足需求 1–3 的基本要求。

3. **（可选）遮挡关系与层级**
   - 设计卡牌的遮挡关系（Z-order 或按照布局规则控制绘制次序）。
   - 优化重叠区域视觉效果，使牌堆、布局更接近真实游戏体验。
