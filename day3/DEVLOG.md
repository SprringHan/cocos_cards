## Day 3 - 2025-11-18

### 今日已完成

1. **完整卡片模型与显示**
   - 从“纯色矩形”升级为完整的卡牌视图：
     - 使用 CardModel 提供点数、花色等数据。
     - 使用 CardView 拼接底牌贴图、数字贴图和花色图标。
   - 调整卡牌大小：
     - 根据底牌原始贴图尺寸计算等比缩放系数。
     - 在 CardView 中同步更新 `ContentSize`，保证视觉尺寸和点击判定一致。
   - 理解并实践了 Cocos2d-x 的坐标系统：
     - `visibleSize`、`ContentSize`、AnchorPoint 的含义和区别。
     - 世界坐标（`touch->getLocation()`）与节点本地坐标（`convertToNodeSpace`）之间的转化关系。

2. **代码重构：提取花色/点数判断逻辑**
   - 将“根据点数/花色生成资源路径”的逻辑从 CardView 中抽离出来，放入 util 类（例如 `CardResUtil`）：
     - 封装 `faceToSymbol(CardFaceType)`：将枚举转换为 `"A" / "2" / ... / "10" / "J" / "Q" / "K"`。
     - 封装 `isRedSuit(CardSuitType)`：区分红牌和黑牌。
     - 封装 `suitToTexturePath(CardSuitType)`：统一管理花色贴图路径。
     - 封装 `bigFaceTexturePath` / `smallFaceTexturePath`：根据点数和花色生成对应数字贴图路径。
   - 这样 CardView 本身只关注“如何摆放组件”，而不再关心资源命名细节，为后期换皮肤或改资源结构打基础。

3. **学习 Cocos2d-x 点击事件处理流程**
   - 梳理了从“玩家点击屏幕”到“自己的回调函数被执行”的完整路径：
     - 操作系统 → Cocos 引擎内部事件 → `EventDispatcher` 分发 → `EventListenerTouchOneByOne` → `onTouchBegan/onTouchEnded`。
   - 在简单 demo 和 CardView 中尝试挂载触摸监听：
     - 使用 `EventListenerTouchOneByOne` 注册单点触摸。
     - 在 `onTouchBegan` 中转换坐标并做命中检测：
       - 通过 `touch->getLocation()` 获取世界坐标。
       - 通过 `convertToNodeSpace()` 转换为当前节点的本地坐标。
       - 使用矩形 `[0, width] x [0, height]` 判断是否点在卡牌内部。
   - 理解了监听器存在的意义：不需要像 EasyX 那样手动 `peekmessage`，而是由引擎统一轮询并回调。

### 未完成 / 仍需消化

- **JSON 文件的具体读法（LevelConfigLoader 内部实现）**
  - 当前的 `LevelConfigLoader::loadLevelConfig` 主要由 AI 辅助生成：
    - 能够正确读取 level1.json 中的 Playfield 与 Stack 牌面信息。
    - 可以把数据转换为 LevelConfig，用于在 GameView 中生成卡牌。
  - 但对 rapidjson 的细节仍不够熟悉（`Document`、`Value`、数组访问等具体 API 还未完全吃透）。
  - 由于时间紧张，今日以“能跑”为主，笔试结束后会专门花时间阅读 rapidjson 文档与源码，重新理解并注释这一块 loader 逻辑。

### 待完成 / 明日计划

1. **卡牌回调函数与 Undo 逻辑**
   - 在 CardView 中通过回调（例如 `std::function<void(CardView*)>`）将“点击事件”传递给 Controller。
   - 在 Controller / GameModel 层实现：
     - 选中/取消选中卡牌。
     - 基础出牌规则判断（能不能出、出到哪里）。
   - 设计并实现最小的 Undo 机制：
     - 记录每一步操作对 GameModel 的修改。
     - 支持撤销至少一到数步操作，满足需求 1–3 的基础要求。

2. **（如果时间允许）卡牌遮挡逻辑**
   - 设计合理的 Z-order 或层级管理方案，处理牌之间的遮挡关系。
   - 在视觉上体现“上层牌盖住下层牌”的效果，使场景更接近真实纸牌布局。
   - 可能需要调整 GameView 中各层的结构与卡牌添加顺序。

3. **后续：重读 JSON loader 实现**
   - 在笔试结束后，回头系统性整理：
     - LevelConfig 数据结构设计。
     - JSON 解析过程，每一步解析对应的 rapidjson 类型与 API。
   - 为后续扩展更多关卡或调整 JSON 结构打下基础。
