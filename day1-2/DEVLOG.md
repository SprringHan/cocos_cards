# DEVLOG – Cocos Cards Day1-2


## Day 1 – 环境搭建与架构初步设计

> 主要是打地基的一天，没有写实际游戏逻辑代码。

**完成内容：**

- 安装并配置 Cocos2d-x 开发环境
  - 搭建 Cocos2d-x 3.x
  - 配置 Visual Studio 工程，确认官方模板项目可以编译运行
- 阅读模板工程代码，了解基础启动流程：
  - `AppDelegate::applicationDidFinishLaunching`
  - `Director`、`Scene`、`Layer` 的基本概念
- 粗略浏览课程 PDF《纸牌程序设计【需求一】》，理解需求：
  - 主牌区（Playfield）、底牌区（Stack / Tray）
  - 牌面点数 ±1 的核心规则
  - 需要实现的动画与 Undo 功能
- 初步设计项目的 MVC 架构：
  - `GameScene`：作为游戏的入口场景
  - `GameView`：负责场景内布局和所有可见元素（牌区、牌、按钮等）
  - `GameModel` / `CardModel`：负责用数据描述“当前局面”
  - 后续再引入 `GameController` 负责调度 Model 与 View

**小结：**

- 今天主要在熟悉 Cocos2d-x 的项目结构和课程需求，为后面真正写逻辑打好基础。
- 心里大概有了一个整体结构：从 `AppDelegate` → `GameScene` → `GameView` → 具体牌的 MVC。

---

## Day 2 – GameScene / GameView / CardView 打通 & 显示第一张牌

> 今天开始真正写和游戏相关的代码，目标是：**让屏幕中出现一张牌**。

**完成内容：**

1. **搭建 `GameScene` 入口**

   - 新建 `GameScene` 继承 `cocos2d::Scene`
   - 实现 `static Scene* createScene()` 并在 `AppDelegate` 中使用 `GameScene::createScene()` 作为启动场景
   - 确认游戏能从模板场景切换到自己的 `GameScene`

2. **实现基础版 `GameView`**

   - 在 `GameScene::init()` 中创建 `GameView` 并作为子节点添加
   - 在 `GameView::init()` 中：
     - 通过 `Director::getInstance()->getVisibleSize()` 获取屏幕尺寸
     - 设置 `GameView` 的 `ContentSize`、`AnchorPoint`、`Position`，让其覆盖整个可见区域
     - 使用 `LayerColor` 分成上下两块区域：
       - 上方：主牌区（Playfield）灰色背景
       - 下方：底牌区（Stack / 手牌区）更深灰背景
     - 学习并实践：
       - `ContentSize` 决定节点尺寸
       - `AnchorPoint` + `setPosition` 决定节点如何对齐到屏幕
       - `setIgnoreAnchorPointForPosition(false)` 对 `LayerColor` 的影响

3. **实现最小可用版 `CardModel` 与 `CardView`**

   - 根据课程 PDF 使用现有枚举：
     - `CardFaceType`（点数）
     - `CardSuitType`（花色）
   - 编写 `CardModel`（继承 `cocos2d::Ref`）：
     - 通过 `create(id, face, suit)` 创建一张牌的数据
     - 内部保存牌的 id、点数、花色
   - 编写 `CardView`（继承 `cocos2d::Node`）：
     - 持有一个 `CardModel*`，并在 `init` 中使用 `CC_SAFE_RETAIN` 管理引用计数
     - 使用 `DrawNode` 画出一个矩形当作临时牌面（后续会替换为真正贴图）
     - 将 `CardView` 的 `ContentSize` 设置为牌的大小，并以中心为锚点

4. **在 GameView 中显示第一张牌**

   - 在主牌区中创建一张测试牌：
     - 使用 `CardModel::create(0, CFT_FIVE, CST_HEARTS);`
     - 使用 `CardView::create(model)` 生成视图
     - 将 `CardView` 添加到 `_playfieldLayer`，并设置位置为主牌区中心
   - 运行后，成功在画面中看到：
     - 上下两个不同背景色区域
     - 主牌区中心的一张白色矩形牌块（代表一张测试牌）

5. **对 Cocos 内存管理有了直观认识**

   - 理解了：
     - `Ref` / `retain()` / `release()` / `autorelease()` 的基本关系
     - 初步了解了 `Node` 的作用，会对其进行使用
     - 了解了cocos2d对内存智能管理的机制，知道了使用 `addchild()` 保护对象
     - 持有 `Ref*` 成员（如 `CardView` 持有 `CardModel*`）时，需要在 `init` 中 `retain()`，在析构中 `release()`

**小结：**

- 今天的目标「看到第一张牌」已经完成 ✅
- GameScene → GameView → CardView 的最小链路已经打通：
  - 后续可以在此基础上：
    - 批量从 JSON 配置生成牌（Step 4）
    - 加入点击、移动、匹配逻辑和 Undo（后续步骤）

---

> 下一步计划：
> - 从关卡 JSON 读取主牌区和底牌区的牌面配置
> - 用 `CardModel` / `CardView` 批量生成整局牌的初始布局