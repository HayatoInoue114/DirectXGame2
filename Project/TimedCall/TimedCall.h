#pragma once
#include <iostream>
#include <functional>

/// <summary>
/// 時限発動
/// </summary>
class TimedCall {
public:
	TimedCall(std::function <void(void)> f, uint32_t time);
	~TimedCall();

	//更新
	void Update();

	//完了ならtrueを返す
	bool IsFinished() { return isComplete; }

private:
	//コールバック
	std::function<void(void)> f_;

	//残り時間
	uint32_t time_;

	//完了フラグ
	bool isComplete = false;
};

