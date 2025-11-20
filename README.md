# Card Matching Game （纸牌匹配小游戏）

基于 **cocos2d-x 3.17** 实现的纸牌匹配小游戏，完成《纸牌程序设计【需求一】》的手牌翻牌、桌面匹配与回退（Undo）功能。

> 提示：本 README 仅描述核心思路，详细设计请参考交付文档。

---

## 功能概览

- ✅ **主牌区匹配**
  - 底牌为点数 N 时，主牌区中点数为 N-1 或 N+1 的翻开牌可被点击匹配。
  - 无花色限制。

- ✅ **手牌区翻牌**
  - 点击手牌区，从牌堆翻出新牌，以动画形式覆盖到底牌区顶部，成为新的底牌。

- ✅ **回退功能**
  - 记录玩家所有翻牌与匹配操作。
  - 多次点击回退按钮，可按相反顺序逐步撤销之前的所有操作，直至撤销栈为空。

- ✅ **关卡配置**
  - 使用 JSON 文件描述主牌区与牌堆的初始布局。
  - 可通过修改配置文件快速新增或调整关卡。

- ✅ **MVC 架构**
  - 严格区分 Model / View / Controller，并引入 Managers 与 Services 层，便于扩展。

---

## 目录结构

（如有出入请按自己项目实际情况调整）

```text
Classes/
├── configs/
│   ├── models/
│   │   └── LevelConfig.h
│   ├── loaders/
│   │   └── LevelConfigLoader.h
│   └── CardResConfig.h
├── models/
│   ├── CardModel.h / .cpp
│   ├── GameModel.h / .cpp
│   └── UndoModel.h / .cpp
├── views/
│   ├── CardView.h / .cpp
│   ├── GameView.h / .cpp
│   ├── PlayFieldView.h / .cpp
│   └── StackView.h / .cpp
├── controllers/
│   ├── GameController.h / .cpp
│   ├── PlayFieldController.h / .cpp
│   └── StackController.h / .cpp
├── managers/
│   └── UndoManager.h / .cpp
├── services/
│   ├── GameModelFromLevelGenerator.h / .cpp
│   └── UndoService.h / .cpp
└── utils/
    ├── LayoutUtils.h / .cpp
    └── JsonUtils.h / .cpp