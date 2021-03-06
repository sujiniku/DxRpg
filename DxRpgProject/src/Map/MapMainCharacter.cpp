#include "Map/MapMainCharacter.h"
#include "DxLib.h"   // GetRand()
#include "Util/GameController.h"

namespace Map
{
	MapMainCharacter::MapMainCharacter()
		: prevX_(0), prevY_(0), encCnt_(0)
		//,x_(10 * BlockLen), y_(7 * BlockLen), dir_(Down), kind_(0)
		// note: 部分的な基底のメンバ初期化はできない
	{
		// 基底メンバの一部初期化
		x_ = 10 * BlockLen;
		y_ = 7 * BlockLen;
		dir_ = Down;
		kind_ = 0;
	}

	MapMainCharacter::~MapMainCharacter()
	{
	}

	void MapMainCharacter::initMapState(int tx, int ty, int dir, int kind)
	{
		encCnt_ = 0;
		prevX_ = tx;
		prevY_ = ty;
		x_ = tx;
		y_ = ty;
		flag_ = true;
		dir_ = dir;  // キャラのむいている向きを下にセット D:0,L:1,R:2,U:3
		imgFlag_ = true; kind_ = kind;
		walkPixel_ = 0; walkFlag_ = false; animePicPos_ = dir * 4;
		stop();            // 歩数０
	}

	bool MapMainCharacter::move(int *isPassable)
	{
		bool enc = false;
		// 下、左、右、上
		const int InputKeyDirection[4] =
		{
			KEY_INPUT_DOWN,
			KEY_INPUT_LEFT,
			KEY_INPUT_RIGHT,
			KEY_INPUT_UP,
		};
		Util::GameController* gc = Util::GameController::getInstance();

		setPosition();        // キャラクタ位置の決定
		// xもyも座標が１区画のサイズの倍数のとき
		if (x_%BlockLen == 0 && y_%BlockLen == 0)
		{
			if (gc->getKey(KEY_INPUT_B) == Util::GameController::Pressed)
			{
				walkSpeed_ = 2;
			}
			else if (gc->getKey(KEY_INPUT_F) == Util::GameController::Pressed)
			{
				walkSpeed_ = 4;
			}
			else
			{
				walkSpeed_ = 1;
			}

			// Eが押されていないとき
			if (gc->getKey(KEY_INPUT_E) == Util::GameController::NotPressed)
			{
				int cy = y_ / BlockLen;
				int cx = x_ / BlockLen;
				// バグ回避のため、
				// 四隅は移動マップ切り替えを伴うのでここではエンカウントしないようにする
				const bool Condition
					= cy == 0 || cy == YBlock - 1 || cx == 0 || cx == XBlock - 1;
				if (!Condition) {
					// エンカウント判定
					enc = encountersEnemy();
				}
			}

			// キーが押されてない時 (キー入力受付条件)
			if (gc->getKey(KEY_INPUT_LEFT) == Util::GameController::NotPressed
				&& gc->getKey(KEY_INPUT_UP) == Util::GameController::NotPressed
				&& gc->getKey(KEY_INPUT_RIGHT) == Util::GameController::NotPressed
				&& gc->getKey(KEY_INPUT_DOWN) == Util::GameController::NotPressed)
			{
				stop();                                // 歩くカウンタを戻す
			}
			else                                    // キーが何か押されている時
			{
				for (int i = 0; i < DirNum; i++)                // 向きの方向チェック
				{
					// 入力キーの判定
					if (gc->getKey(InputKeyDirection[i]) == Util::GameController::Pressed)
					{
						dir_ = i;    // 入力されているキーの方向にdirをセットする
						int cy = y_ / BlockLen;
						int cx = x_ / BlockLen;
						int ny = y_ / BlockLen + OffsetY[dir_];
						int nx = x_ / BlockLen + OffsetX[dir_];
						// 向いてる方向が通れる場所なら
						if (canMove(isPassable[ny * XBlock + nx]))
						{
							walkFlag_ = true;
							// 今いた場所の処理
							// 四隅+その一つ内周なら(これが移動時に万が一重なるバグを回避する手段) 
							const bool ConditionCur
								= cy == 0 || cy == 1 || cy == YBlock - 2 || cy == YBlock - 1
								|| cx == 0 || cx == 1 || cx == XBlock - 2 || cx == XBlock - 1;
							if (ConditionCur)
							{
								isPassable[cy * XBlock + cx] = MainCharOnly;    // ADD
							}
							else
							{
								isPassable[cy * XBlock + cx] = Through;
							}

							// 次の場所の処理
							// 次のマスが四隅なら
							const bool ConditionOuter
								= ny == 0 || ny == 1 || ny == YBlock - 2 || ny == YBlock - 1
								|| nx == 0 || nx == 1 || nx == XBlock - 2 || nx == XBlock - 1;
							if (ConditionOuter)
							{
								isPassable[ny * XBlock + nx] = MainCharOnly;
							}
							else
							{
								isPassable[ny * XBlock + nx] = NoThrough;
							}
						}
						else                    // 向いてる方向が通れない場所なら
						{
							walkFlag_ = false;    // 歩いていない状態に
						}
						break;    // いずれかの方向の処理が終わればループを抜ける
					}
				}
				walk();           // 常にdirの方向へ歩く
			}
		}
		else                      // xもyも座標が１区画のサイズの倍数でないとき
		{
			walk();               // 常にdirの方向へ歩く
		}
		// 次へ描画する画像をセット
		animePicPos_ = decideAnimePic(walkPixel_, dir_);
		return enc;
	}

	bool MapMainCharacter::encountersEnemy()
	{
		bool enc = false;
		// 最低歩数歩かないと戦闘にならない
		if (encCnt_ >= EncMinWalkCnt)
		{    // 一定確率で前と同じ座標でないなら戦闘に
			if (DxLib::GetRand(EncRand) == 0
				&& (x_ != prevX_ || y_ != prevY_))
			{
				encCnt_ = 0;
				enc = true;
			}
		}
		// 前と同じ座標にいないなら敵遭遇カウントを増やして前回位置を更新する。
		if (x_ != prevX_ || y_ != prevY_)
		{
			encCnt_++;
			prevX_ = x_;
			prevY_ = y_;
		}
		return enc;
	}
}
