#include "UndoManager.h"
#include "../models/GameModel.h"
#include "../models/CardModel.h"
#include "../views/GameView.h"
#include "../views/CardView.h"

USING_NS_CC;

UndoManager::UndoManager() = default;

UndoManager::~UndoManager()
{
    CC_SAFE_RELEASE(_undoModel);
}

void UndoManager::init(GameModel* model, GameView* view)
{
    _gameModel = model;
    _gameView = view;

    CC_SAFE_RELEASE(_undoModel);
    _undoModel = UndoModel::create();
    CC_SAFE_RETAIN(_undoModel);
}

bool UndoManager::canUndo() const
{
    return _undoModel && !_undoModel->isEmpty();
}

void UndoManager::pushStackToTop(CardView* sourceView)
{
    if (!_gameModel || !_undoModel || !sourceView) return;

    auto model = const_cast<CardModel*>(sourceView->getModel());
    if (!model) return;

    UndoRecord rec;
    rec.type = UndoOpType::StackToTop;
    rec.movedCard = model;
    rec.prevTopCard = _gameModel->getTopCard(); // 可能为 nullptr
    rec.fromPos = sourceView->getPosition();
    rec.fromStack = true;

    _undoModel->pushRecord(rec);
}

void UndoManager::pushPlayfieldToTop(CardView* sourceView)
{
    if (!_gameModel || !_undoModel || !sourceView) return;

    auto model = const_cast<CardModel*>(sourceView->getModel());
    if (!model) return;

    UndoRecord rec;
    rec.type = UndoOpType::PlayfieldToTop;
    rec.movedCard = model;
    rec.prevTopCard = _gameModel->getTopCard();
    rec.fromPos = sourceView->getPosition();
    rec.fromStack = false;

    _undoModel->pushRecord(rec);
}

void UndoManager::undo()
{
    if (!_gameModel || !_gameView || !_undoModel) return;

    UndoRecord rec;
    if (!_undoModel->popRecord(rec))
    {
        CCLOG("UndoManager: no record to undo.");
        return;
    }

    switch (rec.type)
    {
    case UndoOpType::StackToTop:
        undoStackToTop(rec);
        break;
    case UndoOpType::PlayfieldToTop:
        undoPlayfieldToTop(rec);
        break;
    default:
        break;
    }
}

CardView* UndoManager::findCardViewForModel(CardModel* model) const
{
    if (!_gameView || !model) return nullptr;

    auto searchLayer = [model](Node* layer) -> CardView*
        {
            if (!layer) return nullptr;
            const auto& children = layer->getChildren();
            for (auto child : children)
            {
                auto view = dynamic_cast<CardView*>(child);
                if (!view) continue;

                auto m = const_cast<CardModel*>(view->getModel());
                if (m == model) return view;
            }
            return nullptr;
        };

    // 先在 StackLayer 里找，再在 PlayfieldLayer 里找
    if (auto v = searchLayer(_gameView->getStackLayer()))     return v;
    if (auto v = searchLayer(_gameView->getPlayFieldLayer())) return v;

    return nullptr;
}

void UndoManager::undoStackToTop(const UndoRecord& rec)
{
    CardModel* moved = rec.movedCard;
    CardModel* prevTop = rec.prevTopCard;
    if (!moved) return;

    CardModel* currentTop = _gameModel->getTopCard();
    if (currentTop != moved)
    {
        CCLOG("UndoManager::undoStackToTop: topCard mismatch.");
    }

    // 1. 逻辑还原：把 moved 放回 stack，恢复顶部牌
    _gameModel->addStackCard(moved);
    _gameModel->setTopCard(prevTop);

    // 2. 视图：现在堆牌区本来就没有把牌删掉，这里只是给它一个 MoveTo 回原位置（fromPos）的动画
    CardView* view = findCardViewForModel(moved);
    if (!view) return;

    auto move = MoveTo::create(0.3f, rec.fromPos);
    view->runAction(move);
}

void UndoManager::undoPlayfieldToTop(const UndoRecord& rec)
{
    CardModel* moved = rec.movedCard;
    CardModel* prevTop = rec.prevTopCard;
    if (!moved) return;

    CardModel* currentTop = _gameModel->getTopCard();
    if (currentTop != moved)
    {
        CCLOG("UndoManager::undoPlayfieldToTop: topCard mismatch.");
    }

    // 1. 逻辑还原：顶部牌恢复为 prevTop，moved 回到主牌区
    _gameModel->setTopCard(prevTop);
    _gameModel->addPlayfieldCard(moved);

    // 2. 视图还原：把 moved 的 CardView 从顶部区移回主牌区原位置
    CardView* view = findCardViewForModel(moved);
    if (!view) return;

    Node* playfieldLayer = _gameView->getPlayFieldLayer();
    if (!playfieldLayer) return;

    view->retain();

    // 保持当前世界坐标，换父节点后再转回局部
    Vec2 worldPos = view->getParent()->convertToWorldSpace(view->getPosition());
    view->removeFromParent();
    playfieldLayer->addChild(view);
    Vec2 localPos = playfieldLayer->convertToNodeSpace(worldPos);
    view->setPosition(localPos);

    auto move = MoveTo::create(0.3f, rec.fromPos);
    auto seq = Sequence::create(
        move,
        CallFunc::create([this, view, prevTop]()
            {
                // 动画结束后，把 GameView 里记录的顶部 View 改回 prevTop 对应的那张
                CardView* topView = nullptr;
                if (prevTop)
                {
                    topView = findCardViewForModel(prevTop);
                }
                _gameView->updateStackTopView(topView);
                view->release();
            }),
        nullptr
    );
    view->runAction(seq);
}
