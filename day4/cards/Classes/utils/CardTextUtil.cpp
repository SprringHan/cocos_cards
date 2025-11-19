#include "CardTextUtil.h"
bool isRedSuit(CardSuitType suit) {
    return suit == CST_HEARTS || suit == CST_DIAMONDS;
}

std::string faceToSymbol(CardFaceType face) {
    switch (face) {
    case CFT_ACE:   return "A";
    case CFT_TWO:   return "2";
    case CFT_THREE: return "3";
    case CFT_FOUR:  return "4";
    case CFT_FIVE:  return "5";
    case CFT_SIX:   return "6";
    case CFT_SEVEN: return "7";
    case CFT_EIGHT: return "8";
    case CFT_NINE:  return "9";
    case CFT_TEN:   return "10";
    case CFT_JACK:  return "J";
    case CFT_QUEEN: return "Q";
    case CFT_KING:  return "K";
    default:        return "A"; 
    }
}

std::string suitToTexturePath(CardSuitType suit) {
    switch (suit) {
    case CST_CLUBS:    return "res/suits/club.png";
    case CST_DIAMONDS: return "res/suits/diamond.png";
    case CST_HEARTS:   return "res/suits/heart.png";
    case CST_SPADES:   return "res/suits/spade.png";
    default:           return ""; 
    }
}

std::string bigFaceTexturePath(CardFaceType face, CardSuitType suit) {
    bool red = isRedSuit(suit);
    std::string color = red ? "red" : "black";
    std::string symbol = faceToSymbol(face);
    return "res/number/big_" + color + "_" + symbol + ".png";
}

std::string smallFaceTexturePath(CardFaceType face, CardSuitType suit) {
    bool red = isRedSuit(suit);
    std::string color = red ? "red" : "black";
    std::string symbol = faceToSymbol(face);
    return "res/number/small_" + color + "_" + symbol + ".png";
}