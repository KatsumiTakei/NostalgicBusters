/**
 *	@file	UiDrawer.cpp
 *	@date	2017 / 10 / 20
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#include "UiDrawer.h"
#include "../Player/Player.h"
#include "../Player/Graze.h"
#include "../ScoreManager/ScoreManager.h"
#include "../Config.h"
#include "../Stage/Stage.h"

class UiDrawer::Impl
{
public:
	/**
	 *	@constructor	Impl
	 *	@brief			インプリメント
	 *	@param[in]		argUiDrawer	!<	UIの描画
	 *	@param[in]		argPlayer	!<	プレイヤー
	 */
	Impl(UiDrawer *argUiDrawer, Player *argPlayer)
		: uiDrawer_(argUiDrawer), player_(argPlayer), graphManager_(Singleton<GraphManager>::Get()),
		scaleCnt_(0.f)
	{
		uiTexture_[eImage::Sides] = graphManager_->FindIndexTexture(L"Ui");
		uiTexture_[eImage::Number] = graphManager_->FindIndexTexture(L"Number");
		uiTexture_[eImage::Ready] = graphManager_->FindIndexTexture(L"Ready");
		uiTexture_[eImage::StageTitle] = graphManager_->FindIndexTexture(L"StageCall");
		uiTexture_[eImage::Result] = graphManager_->FindIndexTexture(L"Result");

		for (int lStageIndex = 0; lStageIndex < static_cast<int>(ImageColor::Ready); ++lStageIndex)
		{
			stageNameColor_[lStageIndex] = 0.f;
		}
	}
	~Impl() = default;

private:
	/**
	 *	@fn		DrawReadyGraph
	 *	@brief	ステージタイトルの描画
	 */
	void DrawReadyGraph()
	{
		if (uiDrawer_->count_ > 0)
			return;

		//	ステージタイトル
		//DrawRectGraph(Math::Vector2(PlayScreenRight * 0.5f, PlayScreenBottom * 0.5f), Math::Vector2(0.f, 0.f), Math::Vector2(100.f, 200.f), uiTexture_[eImage::StageTitle], Math::Vector4(1, 1, 1, stageNameColor_[static_cast<int>(ImageColor::Title)]));
		
		//	Ready
		float sf = sinf(scaleCnt_ * Math::PI2 + FlameTime);
		sf = Math::Clamp(sf, 0.f, 1.f);
		scaleCnt_ += 0.002f;
		const Math::Vector2 Pos = Math::Vector2(Window::Get()->WindowRect().right, Window::Get()->WindowRect().bottom) * 0.5f;
		DrawRotaGraph(Pos, Math::Vector2(240.f, 80.f), Math::Vector2(120.f, 40.f), Math::Vector2(1.f, sf), 0.f, uiTexture_[eImage::Ready]);
	}

public:
	void Update()
	{
		const float NumAlphaCnt = (-0.5f) * FlameTime;
		const float NumAdjust = 1.f / -NumAlphaCnt;
		//	ステージ番号のアルファ
		stageNameColor_[static_cast<int>(ImageColor::Ready)] += (uiDrawer_->count_ < NumAlphaCnt) ? NumAdjust : -NumAdjust;
		
		const float TitleAlphaCnt = (-2.5f) * FlameTime;
		const float TitleAdjust = 1.f / -TitleAlphaCnt;
		//	ステージタイトルのアルファ
		stageNameColor_[static_cast<int>(ImageColor::Title)] += (uiDrawer_->count_ < TitleAlphaCnt) ? TitleAdjust : -TitleAdjust;
		
	}

	void Draw()
	{
		// 画面背景
		DrawGraph(Math::Vector2(0, 0), uiTexture_[eImage::Flame]);

		DrawReadyGraph();

		const Math::Vector2 HiScoreTextPos = Math::Vector2(PlayScreenRight + 6.f, PlayScreenTop + 18.f);
		const Math::Vector2 CurrentScoreTextPos = Math::Vector2(HiScoreTextPos.x, HiScoreTextPos.y + 30);
		const Math::Vector4 Black = Math::Vector4(0.5f, 0.5f, 0.5f, 1.f);
		const Math::Vector4 White = Math::Vector4(1.f, 1.f, 1.f, 1.f);

		//{// スコア
		//	const int DigitNum = 9;
		//	//	HACK	:	Text to eImage
		//	/*graphManager_->DrawAndWriteText(L"HiScore", HiScoreTextPos);
		//	graphManager_->DrawAndWriteText(L"Score", CurrentScoreTextPos);*/

		//	//	ハイスコア
		//	//DrawNum(Math::Vector2(HiScoreTextPos.x + 190, HiScoreTextPos.y), Math::Vector2(12, 24), 1.2, static_cast<float>(Singleton<ScoreManager>::Get()->HiScore()), uiTexture_[eImage::Number], DigitNum, White);
		//	//	スコア
		//	DrawNum(Math::Vector2(PlayScreenRight + 200.f, PlayScreenTop + 6.f), Math::Vector2(12, 24), 1.2, static_cast<float>(Singleton<ScoreManager>::Get()->Score()), uiTexture_[eImage::Number], DigitNum, White);
		//}

		const Math::Vector2 PlayerLifeMarkPos(PlayScreenRight * 0.5f, PlayScreenTop * 0.5f);

		{// ライフ
			const Math::Vector2 LifeMarkSize = Math::Vector2(uiTexture_[eImage::Sides]->ImageSize().y);
			//	HACK	:	Text to eImage
			//graphManager_->DrawAndWriteText(L"Player", Math::Vector2(PlayerLifeMarkPos.x - 70.f, PlayerLifeMarkPos.y - 5.f));

			for (int lLifeIndex = 0, lLifeCount = player_->MaxLife(); lLifeIndex < lLifeCount; lLifeIndex++)
			{// 最大ライフ
				DrawRectGraph(Math::Vector2(PlayerLifeMarkPos.x + (LifeMarkSize.x * lLifeIndex) + 2.f, PlayerLifeMarkPos.y), Math::Vector2(0, 0), LifeMarkSize, uiTexture_[eImage::Sides], Black);

				if (lLifeIndex < player_->Life())
				{// 現在のライフ
					DrawRectGraph(Math::Vector2(PlayerLifeMarkPos.x + (LifeMarkSize.x * lLifeIndex) + 2.f, PlayerLifeMarkPos.y), Math::Vector2(0, 0), LifeMarkSize, uiTexture_[eImage::Sides], White);
				}
			}
		}
#if 0

		const Math::Vector2 playerBombsMarkPos(PlayerLifeMarkPos.x, PlayerLifeMarkPos.y + 25.f);

		{// ボム

		 //	HACK	:	Text to eImage
			graphManager_->DrawAndWriteText(L"Bomb", Math::Vector2(playerBombsMarkPos.x - 70.f, playerBombsMarkPos.y - 5.f));
			const Math::Vector2 BombMarkSize = Math::Vector2(21.f, 21.f);
			for (int i = 0; i < 5; i++)
			{// 最大ボム
				DrawRectGraph(Math::Vector2(playerBombsMarkPos.x + (BombMarkSize.x * i) + 22.f, playerBombsMarkPos.y), Math::Vector2(BombMarkSize.x, 0), BombMarkSize, uiTexture_[eImage::Sides], Black);

				if (i < player_->BombsNum())
				{// 現在のボム
					DrawRectGraph(Math::Vector2(playerBombsMarkPos.x + (BombMarkSize.x * i) + 22.f, playerBombsMarkPos.y), Math::Vector2(BombMarkSize.x, 0), BombMarkSize, uiTexture_[eImage::Sides], White);
				}
			}
		}

		const Math::Vector2 playerPowerTextPos = Math::Vector2(playerBombsMarkPos.x - 70.f, playerBombsMarkPos.y + 45.f);
		{// パワー
			wchar_t powerStr[20] = {};
			swprintf_s(powerStr, 20, L"Power 　　%.1f / 5.0", player_->Power());
			//	HACK	:	Text to eImage
			graphManager_->DrawAndWriteText(powerStr, playerPowerTextPos);
		}

		const Math::Vector2 playerGrazePos = Math::Vector2(playerPowerTextPos.x, playerPowerTextPos.y + 45.f);
		{// グレイズ
			graphManager_->ConfigTextColor(D2D1::ColorF::Green);
			//	HACK	:	Text to eImage
			graphManager_->DrawAndWriteText(L"ビリビリ", playerGrazePos);
			graphManager_->ConfigTextColor(D2D1::ColorF::Black);
			const Math::Vector2 GrazeMarkSize = Math::Vector2(21.f, 21.f);
			for (int i = 0; i < 5; i++)
			{// 最大グレイズ
				DrawRectGraph(Math::Vector2(playerGrazePos.x + (GrazeMarkSize.x * i) + 92.f, playerGrazePos.y), Math::Vector2(GrazeMarkSize.x * 2, 0), GrazeMarkSize, uiTexture_[eImage::Sides], Black);

				if (i < player_->GrazePoint() / SpecifiedValue)
				{//	現在のグレイズ
					DrawRectGraph(Math::Vector2(playerGrazePos.x + (GrazeMarkSize.x * i) + 92.f, playerGrazePos.y), Math::Vector2(GrazeMarkSize.x * 2, 0), GrazeMarkSize, uiTexture_[eImage::Sides], White);
				}
			}
		}
#endif
		//	HACK	:	仮
		if (Stage::IsFinished())
		{
			Math::Vector2 size = (player_->IsLiving()) ? Math::Vector2(0, 0) : Math::Vector2(640, 0);
			DrawRectGraph(Math::Vector2(0, 0), size, uiTexture_[eImage::Result]->ImageSize(), uiTexture_[eImage::Result], Math::Vector4(1, 1, 1, 1));
		}
	}

public:
	/**
	 *	@enum	eImage
	 *	@brief	画像の描画用
	 */
	enum eImage
	{
		Sides,
		Flame,
		Number,
		Ready,
		StageTitle,
		Result,

		Max,
	};
	/**
	 *	@enum	ImageColor
	 *	@brief	画像の透明度設定用
	 */
	enum class ImageColor
	{
		Title,
		Ready,
		Max,
	};

private:
	Player						*player_;
	UiDrawer					*uiDrawer_;
	std::shared_ptr<Texture>	uiTexture_[eImage::Max];
	GraphManager				*graphManager_;
	float						stageNameColor_[static_cast<int>(ImageColor::Max)];
	float						scaleCnt_;
};
/**
 *	@constructor	UiDrawer
 *	@brief			UIの表示
 *	@param[in]		argPlayer	!<	プレイヤー
 */
UiDrawer::UiDrawer(Player * argPlayer)
	:pImpl(std::make_unique<Impl>(this, argPlayer))
{
	count_ = (-5) * FlameTime;
	drawMode_ = eDrawMode::Alpha;
	priority_ = ePriority::Ui;
}

UiDrawer::~UiDrawer() = default;

bool UiDrawer::Update()
{
	pImpl->Update();
	count_++;

	return true;
}

void UiDrawer::Draw()
{
	pImpl->Draw();
}