#include "GameModel.h"

USING_NS_CC;

GameModel::~GameModel() {
    CC_SAFE_RELEASE(_topCard);
    _playfieldCards.clear();
    _stackCards.clear();
}

GameModel* GameModel::create() {
    auto ret = new (std::nothrow) GameModel();
    if (ret && ret->init()) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool GameModel::init() {
    return true;
}

void GameModel::setTopCard(CardModel* card) {
    if (_topCard != card) {
        CC_SAFE_RELEASE(_topCard);
        _topCard = card;
        CC_SAFE_RETAIN(_topCard);
    }
}

void GameModel::addPlayfieldCard(CardModel* card) {
    if (card) _playfieldCards.pushBack(card);
}

void GameModel::removePlayfieldCard(CardModel* card) {
    if (card) _playfieldCards.eraseObject(card);
}

bool GameModel::isCardInPlayfield(CardModel* card) const {
    return _playfieldCards.contains(card);
}

void GameModel::addStackCard(CardModel* card) {
    if (card) _stackCards.pushBack(card);
}

CardModel* GameModel::getNextStackCard() const {
    if (_stackCards.empty()) return nullptr;
    return _stackCards.back(); // 最后添加的是最上面的
}

CardModel* GameModel::popStackCard() {
    if (_stackCards.empty()) return nullptr;
    auto card = _stackCards.back();
    // 增加引用计数防止从Vector移除时被释放，交给调用者管理
    card->retain();
    _stackCards.popBack();
    card->autorelease();
    return card;
}

bool GameModel::isStackEmpty() const {
    return _stackCards.empty();
}