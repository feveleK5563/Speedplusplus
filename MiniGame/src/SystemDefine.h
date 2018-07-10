#pragma once
#include <random>
#include <memory>
#include <vector>
#include "Math.h"

class SystemDefine
{
private:
	static std::random_device	rd;
	static std::mt19937			mtrand;

public:
	static const int windowSizeX;	//ウィンドウサイズX
	static const int windowSizeY;	//ウィンドウサイズY

	//画面外判定(引数に矩形を指定、画面外に出ていたらtrue)
	static bool WindowOutBox(const Math::Box2D& hitBase);
	//画面内判定(引数に矩形を指定、画面内にいたらtrue)
	static bool WindowInBox(const Math::Box2D& hitBase);

	//生ポインタを安全にdeleteする
	template<class T> static void SafeDelete(T& t);
	//生ポインタを安全にdeleteする(配列版)
	template<class T> static void SafeDeleteArr(T& t);

	//範囲内でランダムな値を指定個数分取得する
	static std::shared_ptr<int[]> OutputRandomRange(int lowerLimit, int upperLimit, int size);
};