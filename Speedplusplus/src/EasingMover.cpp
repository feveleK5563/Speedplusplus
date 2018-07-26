#include "EasingMover.h"

EasingMover::EasingMover(	const Math::Vec2& pos, float scale, float degAngle):
	pos(pos),				startPos(pos),				diffPos(Math::Vec2(0, 0)),
	scale(scale),			startScale(scale),			diffScale(0.f),
	degAngle(degAngle),		startDegAngle(degAngle),	diffDegAngle(0.f),
	angle(Math::ToRadian(degAngle)) {}

EasingMover::EasingMover(	const Math::Vec2& startPos, const Math::Vec2& endPos,
							float startScale,			float endScale,
							float startDegAngle,		float endDegAngle):
	pos(startPos),				startPos(startPos),				diffPos(endPos - startPos),
	scale(startScale),			startScale(startScale),			diffScale(endScale - startScale),
	degAngle(startDegAngle),	startDegAngle(startDegAngle),	diffDegAngle(endDegAngle - startDegAngle),
	angle(Math::ToRadian(degAngle)) {}


bool EasingMover::Update(float speed)
{
	easing.Run(Ease_Cubic::Out, speed);

	pos.x		= easing.GetVolume(startPos.x, diffPos.x);
	pos.y		= easing.GetVolume(startPos.y, diffPos.y);
	scale		= easing.GetVolume(startScale, diffScale);
	degAngle	= easing.GetVolume(startDegAngle, diffDegAngle);
	angle		= Math::ToRadian(degAngle);

	return easing.IsEaseEnd();
}

const Math::Vec2& EasingMover::GetPos() const
{
	return pos;
}

const float& EasingMover::GetScale() const
{
	return scale;
}

const float& EasingMover::GetAngle() const
{
	return angle;
}

void EasingMover::SetEndMove(const Math::Vec2& endPos, float endScale, float endDegAngle)
{
	startPos = pos;
	diffPos = endPos - pos;

	startScale = scale;
	diffScale = endScale - scale;

	startDegAngle = degAngle;
	diffDegAngle = endDegAngle - degAngle;

	easing.Reset();
}