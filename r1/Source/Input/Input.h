
#pragma once

#include "../stdafx.h"

class Input : public GenericKeys
{
private:
	enum class eKeySource
	{
		Shot = GenericKeys::KeyCount,
		Slow,
		Bomb,
		Charge,
	};
public:
	static const eKey MoveUp = GenericKeys::Up;
	static const eKey MoveDown = GenericKeys::Down;
	static const eKey MoveRight = GenericKeys::Right;
	static const eKey MoveLeft = GenericKeys::Left;

	static const eKey Shot = static_cast<eKey>(eKeySource::Shot);
	static const eKey Slow = static_cast<eKey>(eKeySource::Slow);
	static const eKey Bomb = static_cast<eKey>(eKeySource::Bomb);
	static const eKey Charge = static_cast<eKey>(eKeySource::Charge);

public:
	static std::function<bool(eKey)> IsPressedAll;
	static std::function<bool(eKey)> IsTouchedAll;
	static std::function<bool(eKey, int)> IsPressed;
	static std::function<bool(eKey, int)> IsTouched;
};