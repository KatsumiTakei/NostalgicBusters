#include "Input.h"

std::function<bool(eKey)> Input::IsPressedAll = [&](eKey argKeyCode)
{
	return Singleton<InputManager>::Get()->IsPressed(eStick::Left, argKeyCode);
};

std::function<bool(eKey)> Input::IsTouchedAll = [&](eKey argKeyCode)
{
	return Singleton<InputManager>::Get()->IsTouched(eStick::Left, argKeyCode);
};

std::function<bool(eKey, int)> Input::IsPressed = [&](eKey argKeyCode, int argPadIndex)
{
	return Singleton<InputManager>::Get()->IsPressed(eStick::Left, argKeyCode, argPadIndex);
};

std::function<bool(eKey, int)> Input::IsTouched = [&](eKey argKeyCode, int argPadIndex)
{
	return Singleton<InputManager>::Get()->IsTouched(eStick::Left, argKeyCode, argPadIndex);
};