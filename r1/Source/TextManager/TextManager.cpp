/**
 *	@file	TextManager.cpp
 *	@date	2017 / 10 / 20
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#include "TextManager.h"
#include "../Config.h"
#include "../Input/Input.h"

class TextManager::Impl 
{
public:
	/**
	 *	@constructor	Impl
	 *	@brief			インプリメント
	 */
	Impl() : drawPointX_(50.f), drawPointY_(350.f), stringP_(0), charP_(0),
		count_(0), isWaiting_(false), isFinished_(false), isChanged_(false),
		textScreen_(std::make_unique<RectGauge>()), textPath_("Resources/Data/")
	{
		textScreen_->Color(D2D1::ColorF(0.f, 0.f, 0.f, 0.5f));
		textScreen_->pos_ = Math::Vector2(drawPointX_, drawPointY_);
		textScreen_->Wight(75.f);
		textScreen_->property_.MaxValue = 350.f;
		textScreen_->property_.MinValue = 50.f;

		textScreen_->property_.MaxPos = 350.f;
		textScreen_->property_.MinPos = 50.f;
		textScreen_->property_.Value = 350.f;
		graph_[Leika] = Singleton<GraphManager>::Get()->FindIndexTexture(L"Leika");
		graph_[LeikaDetail] = Singleton<GraphManager>::Get()->FindIndexTexture(L"LeikaDetail");
		const std::string Filename = "stage01boss";
		const std::string Path = (textPath_ + Filename);

#ifndef _USE_ARCHIVE_
		File *file = nullptr;
		Singleton<Loader>::Get()->SetFile(&file, (Path + ".txt").c_str());
		Singleton<Loader>::Get()->LoadAtThreadUTF();

		for (int i = 0; i < 256; ++i)
		{
			for (int j = 0; j < 1; ++j)
			{
				text_[j][i] = 0;
			}
		}
		
		lstrcpyW(text_[0], file->DataW());
		Resource::ExportBinary(Path + ".dat", file->DataW());
#else
		const char* text = Singleton<Archive>::Get()->LoadText((Path + ".dat").c_str());
		auto str = String::DeleteSubStr(text, "E");
		lstrcpyW(text_[0], String::ToWstring(str).c_str());
		
#endif // !_USE_ARCHIVE_

	}
	~Impl() = default;


private:

	void UpdateText(const wchar_t str)
	{
		// １文字分一時記憶配列
		wchar_t OneBuf[3];
		switch (str)
		{// 文字の設定

		case '@':
		{// 改行文字
			charP_++;
			break;
		}
		case 'B':
		{// ボタン押し待ち文字
			isWaiting_ = true;
			charP_++;
			break;
		}
		case 'E':
		{// 終了文字
			isFinished_ = true;
			charP_++;
			break;
		}
		case 'G':
		{// 表情変更文字
			isChanged_ = true;
			charP_++;
			break;
		}
		case 'C':
		{// クリア文字
			drawtext_.clear();
			charP_++;
			break;
		}
		default:
		{// その他の文字
			if (count_ > 600)
			{
				isWaiting_ = false;
			}
			if (!isWaiting_)
			{// １文字分抜き出す
				OneBuf[0] = text_[stringP_][charP_];
				OneBuf[1] = text_[stringP_][charP_ + 1];
				OneBuf[2] = '\0';

				// 参照文字位置を２バイト勧める
				charP_ += 2;

				drawtext_ += OneBuf;

				count_ = 0;
			}
			break;
		}

		if (text_[stringP_][charP_] == '\0')
		{// 参照文字列の終端まで行っていたら参照文字列を進める
			stringP_++;
			charP_ = 0;
		}

		}// ----switch終わり----
	}
public:

	void Initialize()
	{
		drawtext_.clear();

		isFinished_ = true;
		isChanged_ = false;
		isWaiting_ = false;
		count_ = 0;
		stringP_ = 0;
		charP_ = 0;

	}

	bool Update()
	{
		if (isFinished_) 
			return false;

		if (Singleton<InputManager>::Get()->IsTouched(Input::Decide))
		{
			isWaiting_ = false;
		}

		const auto OneStr = text_[stringP_][charP_];
		UpdateText(OneStr);

		count_++;
		return true;
	}

	void Draw()
	{
		if (isFinished_)
			return;

		const Math::Vector2 Size = Math::Vector2(300.f, 360.f);
		
		std::weak_ptr<Texture> tex = (isChanged_) ? graph_[LeikaDetail] : graph_[Leika];
		DrawRotaGraph(Math::Vector2(static_cast<float>(PlayScreenLeft) * 4.f, drawPointY_ * 0.85f), Size, 0.5f, 0.f, tex);

		Singleton<GraphManager>::Get()->ChangeRt2dDepth(eRtDepth::Back);
		Singleton<GraphManager>::Get()->RenderTarget()->BeginDraw();
		
		textScreen_->Draw(Singleton<GraphManager>::Get()->RenderTarget());

		Singleton<GraphManager>::Get()->ConfigTextColor(D2D1::ColorF::White);
		Singleton<GraphManager>::Get()->DrawAndWriteText(drawtext_.c_str(), Math::Vector2(drawPointX_, drawPointY_));
		Singleton<GraphManager>::Get()->ConfigTextColor(D2D1::ColorF::Black);

		Singleton<GraphManager>::Get()->RenderTarget()->EndDraw();
		Singleton<GraphManager>::Get()->ChangeRt2dDepth(eRtDepth::Front);


	}

private:
	enum
	{
		Under,
		Leika,
		LeikaDetail,

		Num,
	};

private:
	float drawPointX_, drawPointY_;
	int stringP_, charP_;
	int count_;
	bool isWaiting_;
	bool isChanged_;
	wchar_t text_[1][256];
	std::wstring drawtext_;
	std::shared_ptr<Texture> graph_[Num];
	std::unique_ptr<RectGauge> textScreen_;

public:
	bool isFinished_;
	std::string textPath_;
};

TextManager::TextManager()
	:pImpl(std::make_unique<Impl>())
{
}

TextManager::~TextManager() = default;

void TextManager::Initialize()
{
	pImpl->Initialize();
}

bool TextManager::Update()
{
	return pImpl->Update();
}

void TextManager::Draw()
{
	pImpl->Draw();
}
void TextManager::IsFinished(bool argFlag)
{ 
	pImpl->isFinished_ = argFlag;
}
bool TextManager::IsFinished()const 
{ 
	return pImpl->isFinished_; 
}

void TextManager::TextPath(std::string argPath)
{
	pImpl->textPath_ = argPath;
}

std::string TextManager::TextPath()const
{
	return pImpl->textPath_;
}
