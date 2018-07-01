#pragma once
#include <vector>
#include "Math.h"
#include "ImageLoader.h"

//色指定
struct Color
{
	int r, g, b, alpha;

	Color();
	Color(int r, int g, int b, int alpha);
	Color(const Color& color);
};

//画像データの描画とアニメーションの管理
class ImageDrawer
{
private:
	ImageData	imageData;		//画像データ
	Math::Vec2	criterionPos;	//描画の基準位置
	int			nowAnimPattern;	//現在のアニメーションパターン番号
	float		nowAnimImage;	//現在のアニメーション画像

public:
	//コンストラクタ(描画したい画像データを指定)
	ImageDrawer(const ImageData& imageData, const Math::Vec2& criterionPos);
	//コンストラクタ(描画したい画像データと描画輝度を指定)
	ImageDrawer(const ImageData& imageData, const Math::Vec2& criterionPos, const Color& color);

	bool Run();	//アニメーションさせる(アニメーションが一周したらtrueが返る)
	void ChangeAnimPattern(int pattern, bool isResetTime);	//アニメーションパターン番号の変更

	//描画する
	void Draw(const Math::Vec2& pos, float scale, float angle, bool isTurn, const Color& color);
	//指定番号の画像を描画する
	void DrawOne(const Math::Vec2& pos, float scale, float angle, bool isTurnint, int imageSheet, const Color& color);

	const ImageData& GetImageData() const;	//画像データを取得
};