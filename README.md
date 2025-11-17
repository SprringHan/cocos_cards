# Cocos Cards – 纸牌小游戏

一个基于 **Cocos2d-x** 的纸牌小游戏，用来完成课程给定的「纸牌程序设计」需求，并练习 **MVC 架构** 和 C++ 工程实践。

当前进度：✅ 能在场景中创建基础的 `GameScene` / `GameView` / `CardView`，并在屏幕中正确显示一张牌。

---

## 功能目标（对应课程需求）

> 具体需求参考课程提供的 PDF《纸牌程序设计【需求一】》

- **基础玩法**
  - 主牌区（Playfield）：根据关卡配置布局多张牌
  - 底牌区 / 手牌区（Stack / Tray）：作为玩家可操作的牌堆
  - 规则：玩家可以根据点数 ±1 的规则从桌面或牌堆出牌
- **交互与动画**
  - 点击牌堆翻牌到手牌区
  - 点击符合规则的桌面牌，使其飞到手牌区顶部
  - 牌移动使用 Cocos2d-x 动作系统（`MoveTo` 等）
- **回退（Undo）系统**
  - 记录每一步操作
  - 支持多步撤销：牌按相反动画飞回原位

---

## 技术栈

- 语言：C++
- 引擎：Cocos2d-x 3.17
- 架构：MVC（`GameModel / GameView / GameController`）
- 开发环境：
  - Windows + Visual Studio（工程使用 Cocos 官方模板创建）
  - 设计分辨率：1080 × 2080，`FIXED_WIDTH`

---

## 当前进度

### Day 1 – 环境与架构准备

- 完成 Cocos2d-x 环境安装与配置
- 阅读引擎模板代码，理解基本启动流程（`AppDelegate` / `Director` / `Scene`）
- 初步设计项目的 MVC 结构：
  - `GameScene` 作为入口场景
  - `GameView` 负责场景内所有 UI/牌面展示
  - `GameModel` / `CardModel` 用于抽象游戏与牌的数据

### Day 2 – 显示第一张牌

- 新建 `GameScene`，并在 `AppDelegate` 中切换到该场景作为游戏入口
- 搭建基础 `GameView`：
  - 获取屏幕 `visibleSize`，将 `GameView` 填满屏幕
  - 使用 `LayerColor` 分割上方主牌区 / 下方底牌区
  - 学习并实践了 `ContentSize`、`AnchorPoint`、`Position` 的关系
- 实现最小可用版 `CardModel` + `CardView`：
  - `CardModel`：使用课程 PDF 里的 `CardFaceType` / `CardSuitType` 枚举存储牌面信息
  - `CardView`：继承 `Node`，在内部用 `DrawNode` 画出一张矩形牌作为占位
  - 在 `GameView` 中创建一张牌并成功显示在屏幕中（验证坐标与视图结构正确）
- 理解了 Cocos2d-x 中 `Ref` / `autorelease` 的基本使用方式

更详细的过程记录见：[DEVLOG.md](./DEVLOG.md)

---

## 目录结构（进行中）

> 根据课程要求和 MVC 思路逐步整理中

```text
.
├─ Classes/
│  ├─ GameScene.h / GameScene.cpp     # 主场景（入口）
│  ├─ GameView.h / GameView.cpp       # 场景视图，负责布局与牌面展示
│  ├─ CardModel.h / CardModel.cpp     # 单张牌的数据模型
│  ├─ CardView.h / CardView.cpp       # 单张牌的视图
│  ├─ GameConfig.h                    # 花色、点数等枚举和配置
│  └─ （后续：GameModel / GameController / UndoManager 等）
├─ Resources/
│  └─ （牌面图片、关卡 JSON 等资源）
├─ proj.win32/                        # Visual Studio 工程目录（自动生成）
└─ DEVLOG.md
