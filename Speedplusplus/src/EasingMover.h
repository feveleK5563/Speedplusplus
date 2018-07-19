#pragma once
#include "Math.h"
#include "Easing.h"

class EasingMover
{
private:
	Easing		easing;			//イージング

	Math::Vec2	pos;			//座標
	Math::Vec2	startPos;		//開始時の位置
	Math::Vec2	diffPos;		//目標の位置 - 開始時の位置

	float		scale;			//拡大率(1.fで標準)
	float		startScale;		//開始時の拡大率
	float		diffScale;		//目標の拡大率 - 開始時の拡大率

	float		degAngle;		//角度(degree)
	float		startDegAngle;	//開始時の角度
	float		diffDegAngle;	//目標の角度 - 開始時の角度
	float		angle;			//角度

public:
	EasingMover(const Math::Vec2& pos, float scale, float degAngle);

	EasingMover(const Math::Vec2& startPos, const Math::Vec2& endPos,
				float startScale,			float endScale,
				float startDegAngle,		float endDegAngle);

	bool Update(float speed);

	const Math::Vec2&	GetPos() const;
	const float&		GetScale() const;
	const float&		GetAngle() const;

	void SetEndMove(const Math::Vec2& endPos, float endScale, float endDegAngle);
};