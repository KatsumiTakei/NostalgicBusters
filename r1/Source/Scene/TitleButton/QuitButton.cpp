/**
 *	@file	QuitButton.cpp
 *	@date	2017 / 7 / 25
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#include "QuitButton.h"

QuitButton::QuitButton()
	:TitleButton()
{
	buttonColor_[TitleButton::Quit].w = 1.f;
	introduction_ = L"‚¢‚«‚È‚èƒQ[ƒ€‚ðI—¹‚µ‚Ü‚·";
}

QuitButton::~QuitButton() = default;

bool QuitButton::Execute()
{
	PostMessage(Window::Get()->WindowHundle(), WM_CLOSE, 0, 0);

	return true;
}

bool QuitButton::Enter()
{
	return TitleButton::Enter() ? Execute() : false;
}

void QuitButton::Draw()
{
	TitleButton::Draw();
}
