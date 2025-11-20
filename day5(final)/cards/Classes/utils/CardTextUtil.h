#pragma once

#include<string>
#include "../configs/models/GameConfig.h"

//判断花色是否为红色
bool isRed(CardSuitType suit);

//点数枚举->字符串
std::string faceToSymble(CardFaceType face);

//花色枚举->资源路径
std::string suitToTexturePath(CardSuitType suit);

//点数枚举->大数字路径
std::string bigFaceTexturePath(CardFaceType face, CardSuitType suit);

//点数枚举->小数字路径
std::string smallFaceTexturePath(CardFaceType face, CardSuitType suit);