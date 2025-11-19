# Kards - Cocos2d-x 纸牌小游戏（需求一）

本项目是基于 Cocos2d-x 的纸牌游戏课程作业，目标是在限定时间内，从零搭建一个结构清晰、功能可用的纸牌游戏 Demo，并尽量按照 MVC 思路组织代码结构。

## 项目概述

- 引擎：Cocos2d-x
- 语言：C++
- 架构：简化版 MVC（GameScene / GameView / GameModel / Controller）
- 核心要素：
  - 卡牌模型：点数、花色、在场景中的位置
  - 卡牌视图：底牌贴图 + 大号数字 + 小号数字 + 花色图标
  - 关卡配置：从 JSON 读取牌面布局
  - 基本交互：点击牌、翻牌、匹配规则与后续的撤销（Undo）

## 目录结构（简要）

> 实际目录以工程为准，这里只列出与本次需求密切相关的部分。

- `Classes/`
  - `scenes/GameScene.*`  
    场景入口，负责创建 GameView / GameModel / GameController 并关联起来。
  - `views/GameView.*`  
    负责整块游戏画面的布局（主牌区 / Playfield + 手牌区 / Stack）。
  - `views/CardView.*`  
    单张卡牌的视图，用一个底牌 Sprite + 数字/花色 Sprite 组合而成，内部处理点击命中检测并通过回调对外发送“被点击”事件。
  - `models/CardModel.*`  
    卡牌数据模型，包含 cardId、点数、花色等信息，后续会作为 GameModel 内部状态的一部分。
  - `models/GameModel.*`  
    游戏整体数据模型（当前仍是最小实现，后续承载运行时状态与 Undo 记录）。
  - `configs/models/LevelConfig.*`  
    关卡静态配置结构（Playfield/Stack 里有哪些牌、初始点数/花色/位置）。
  - `configs/loaders/LevelConfigLoader.*`  
    使用 rapidjson 读取 JSON 文件，生成 `LevelConfig`，加载 Playfield / Stack 的初始布局。
  - `utils/CardResUtil.*`  
    根据点数/花色生成数字贴图、花色贴图路径，封装红黑判断等资源相关逻辑。
  - `utils/StackLayoutUtil.*`  
    在加载 LevelConfig 时，为手牌区（Stack）自动计算合理的摆放坐标。
  - `controllers/GameController.*`  
    控制层入口，接受来自 GameView 的点击事件，后续将调用 Playfield / Stack 相关逻辑并驱动 GameModel。
  - （预留）`controllers/PlayfieldController.*` / `controllers/StackController.*`  
    分别负责主牌区与手牌区的详细规则逻辑（尚在规划/迁移中）。

- `Resources/`
  - `res/card_bg.png`：空白底牌贴图
  - `res/number/`：大/小数字贴图（例如 `big_red_5.png` / `small_black_A.png`）
  - `res/suits/`：花色贴图（例如 `heart.png` / `spade.png`）
  - `configs/levels/level1.json`：示例关卡配置

## 开发进度

### Day 1 - 环境搭建 & 框架初建

- 完成 Cocos2d-x 开发环境配置，能够成功编译运行空工程。
- 了解基本引擎结构（`Director` / `Scene` / `Layer` / `Node`）。
- 搭建基础 MVC 思路：
  - `GameScene` 作为场景入口。
  - 预留 `GameView`、`GameModel`、`Controller` 的职责划分。

### Day 2 - GameScene & 基础 CardView

- 创建 `GameScene` 和 `GameView`，建立主牌区和手牌区两个基础布局。
- 实现最简单的 `CardModel` / `CardView`：
  - 通过 `DrawNode` 或简单 Sprite，在屏幕上画出一张“调试用”的卡牌。
  - 初步理解 `visibleSize`、`ContentSize`、锚点（AnchorPoint）等概念。
- 确认从 `GameScene` 挂载 `GameView`，并在 `GameView` 中创建并显示单张卡牌。

### Day 3 - 完整卡牌 & 事件监听（2025-11-18）

**1. 完整卡片模型与显示**

- 将卡牌从“纯色矩形”升级为完整的纸牌视图：
  - 使用 `CardModel + CardView` 组合：
    - 底牌背景：`card_bg.png`
    - 大号数字贴图：`res/number/big_*.png`
    - 小号数字 + 花色贴图：`res/number/small_*.png` + `res/suits/*.png`
- 处理卡牌大小与缩放：
  - 基于底牌原始贴图大小，按目标高度计算 `scale`，等比放大。
  - 同步更新 `CardView::ContentSize`，保证点击区域与显示一致。
- 理解 Cocos2d-x 的多种坐标系并实际使用：
  - 世界坐标（`touch->getLocation()`）
  - 节点本地坐标（`convertToNodeSpace`）
  - 不同 Anchor 下位置的含义与影响。

**2. 资源路径逻辑重构**

- 将“点数/花色 → 资源路径”的分支逻辑从 `CardView` 中抽离，封装到 `CardResUtil` 中：
  - 统一封装数字字符（`A/2/.../10/J/Q/K`）、红黑花色判断、各类贴图路径生成。
- 减少视图层硬编码，方便美术资源或命名规则变化时集中修改。

**3. Cocos2d-x 事件系统与点击监听**

- 学习触摸事件的整体流程：
  - 操作系统输入 → Cocos 内部事件 → `EventDispatcher` → `EventListenerTouchOneByOne` → `onTouchBegan/onTouchEnded`。
- 在 `CardView` 中挂载触摸监听：
  - 使用 `EventListenerTouchOneByOne` 注册单点触摸。
  - 利用 `touch->getLocation()` + `convertToNodeSpace()` + `Rect.containsPoint()` 做命中检测。
- 为后续“点击牌 → 通知 Controller”打通了视图层的输入通路。

**4. JSON Loader 现状**

- 借助 AI 辅助生成了 `LevelConfigLoader::loadLevelConfig` 的基础实现：
  - 能从 JSON 中读取 Playfield / Stack 的牌面数据并在场景中展示。
- 暂未完全吃透 rapidjson 的细节（`Document` / `Value` / 数组访问等），计划在笔试结束后对 loader 部分进行二次阅读和重构。

### Day 4 - 控制器接入 & 手牌区布局 / 规则设计（2025-11-19）

**1. GameModel 最小实现 & 架构完善**

- 新增 `GameModel` 类，作为后续承载游戏运行状态与 Undo 记录的模型层：
  - 解决了 `init()` 写成全局函数、析构函数声明未实现导致的链接错误。
  - 当前版本为最小实现：可以通过 `CREATE_FUNC(GameModel)` 正常创建实例。
- 明确了未来架构分层：
  - `GameScene`：负责创建并关联 GameView / GameModel / GameController。
  - `GameView`：只负责视图展示与输入转发，不直接包含规则逻辑。
  - `GameController`（以及后续的 `PlayfieldController` / `StackController`）：负责规则判断与状态流转。

**2. 点击事件完整通路：CardView → GameView → GameController**

- 在 `CardView` 中：
  - 保留触摸监听，仅负责命中检测和触发 `_clickCallback(this)`。
- 在 `GameView` 中：
  - 为每张牌设置点击回调：
    - 主牌区牌：`view->setClickCallback([this](CardView* v) { _controller->onPlayfieldCardClicked(v); });`
    - 手牌区牌：`view->setClickCallback([this](CardView* v) { _controller->onStackCardClicked(v); });`
  - 不再在视图层决定“能不能出牌”，而是把事件交给控制器。
- 在 `GameController` 中：
  - 设计了 `onPlayfieldCardClicked(CardView*)` 与 `onStackCardClicked(CardView*)` 接口，准备承接需求一、二的完整逻辑。

**3. 手牌区布局工具：StackLayoutUtil**

- 新增 `StackLayoutUtil::applyStackLayout(std::vector<LevelCardConfig>& stackCards)`：
  - 针对 JSON 中 hand stack 的坐标为 0 的情况，在关卡加载时自动计算手牌区布局：
    - 除顶牌外的手牌以约 `35.0f` 的水平间距从左向右堆叠。
    - 顶牌位于最右侧，比前一张多偏移约 `50.0f`，视觉上作为“真正的顶部牌”。
    - 整个手牌堆在 stack 区域内部水平居中，垂直放在 stack 区高度中线处。
  - 在 `LevelConfigLoader::loadLevelConfig` 中，解析完 `Stack` 数组后对 `_stackCards` 调用该工具，确保开局手牌布局合理、与 JSON 完全解耦。

**4. 需求一、二的规则设计与 bug 分析**

- 针对需求一和需求二，开始设计运行时数据结构与规则：
  - 引入：
    - `_playfieldCardViews`：当前主牌区的牌视图列表。
    - `_stackCardViews`：手牌堆中的牌视图列表。
    - `_stackTopView`：当前顶部牌（可以来自手牌堆，也可以来自主牌区）。
- 发现并分析一个潜在 bug：
  - 一开始尝试通过“用 `_stackTopView` 到 `_stackCardViews` 中查 index”来判断下一张手牌；
  - 但当顶牌被主牌区牌替换后，它不再属于 `_stackCardViews`，此时通过顶牌去数组枚举 index 会失败。
- 对策（设计层面）：
  - 将“手牌堆状态”与“顶部牌”解耦：
    - 使用 `_stackCardViews + _stackDrawIndex` 表示“手牌堆还剩多少、下一张要翻谁”；
    - 使用 `_stackTopView` 单独表示当前顶部牌，而不假设它一定属于 `_stackCardViews`。
  - 这样：
    - 点击手牌区抽牌（需求一）只依赖 `_stackDrawIndex`，不会因为顶部牌被替换而出错；
    - 点击主牌区匹配顶部牌（需求二）只更新 `_stackTopView`，不影响手牌堆索引。

**5. C++ 语法与 Cocos 调试能力的提升**

- 通过这一天的实现顺便补课了：
  - `this` 指针在成员函数中的含义，本质上是“当前对象指针”的隐式参数。
  - lambda 捕获列表（`[this]`、`[=]`、`[&]` 等）的语义，理解了回调中如何正确捕获当前对象。
  - 明确了点击回调参数 `v` 是由 CardView 在 `_clickCallback(this)` 时传入的，不与外部 `view` 变量“自动绑定”。
- 学会在 Visual Studio 的“输出(Output)”窗口中查看 `CCLOG` 输出，用于调试点击事件、坐标、状态变更等。

## TODO / 下一步计划

2. **Undo（需求三）设计与实现**
   - 为每一步操作记录必要的状态变化（例如：从哪个区域移到哪个区域、旧顶牌是谁、对应的 CardModel 状态）。
   - 设计一个操作栈结构，用于支持一层或多层撤销。
   - 在控制器中实现 Undo 接口，并预留 UI 的触发入口。