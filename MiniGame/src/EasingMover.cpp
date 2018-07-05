#include "EasingMover.h"

EasingMover::EasingMover(	const Math::Vec2& pos, float scale, float degAngle):
	pos(pos),				startPos(pos),				diffPos(Math::Vec2(0, 0)),
	scale(scale),			startScale(scale),			diffScale(0.f),
	degAngle(degAngle),		startDegAngle(degAngle),	diffDegAngle(0.f) {}

EasingMover::EasingMover(	const Math::Vec2& startPos, const Math::Vec2& endPos,
							float startScale,			float endScale,
							float startDegAngle,		float endDegAngle):
	pos(startPos),				startPos(startPos),				diffPos(endPos - startPos),
	scale(startScale),			startScale(startScale),			diffScale(endScale - startScale),
	degAngle(startDegAngle),	startDegAngle(startDegAngle),	diffDegAngle(endDegAngle - startDegAngle) {}


void EasingMover::Update()
{
	float nEase = easing.cubic.Out(easing.Time(30), 0.f, 1.f);

	pos.x		= startPos.x + (diffPos.x * nEase);
	pos.y		= startPos.y + (diffPos.y * nEase);
	scale		= startScale + (diffScale * nEase);
	degAngle	= startDegAngle + (diffDegAngle * nEase);
}

const Math::Vec2& EasingMover::GetPos()
{
	return pos;
}

float EasingMover::GetScale()
{
	return scale;
}

float EasingMover::GetAngle()
{
	return Math::ToRadian(degAngle);
}