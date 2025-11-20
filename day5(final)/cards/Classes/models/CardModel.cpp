#include "CardModel.h"

USING_NS_CC;

//由于构造方法中含有参数，这里手动实现静态create方法
CardModel* CardModel::create(int id, CardFaceType face,CardSuitType suit) {
	auto ret = new (std::nothrow) CardModel;
	if (ret && ret->init(id, face, suit)) {
		ret->autorelease();
		return ret;
	}
	CC_SAFE_DELETE(ret);
	return nullptr;
}

bool CardModel::init(int id, CardFaceType face, CardSuitType suit) {
	_cardId = id;
	_face = face;
	_suit = suit;
	return true;
}
