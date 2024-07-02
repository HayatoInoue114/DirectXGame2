#include "TimedCall.h"

TimedCall::TimedCall(std::function<void(void)> f, uint32_t time) {
	f_ = f;
	time_ = time;
}

TimedCall::~TimedCall() {}

void TimedCall::Update() {
	if (isComplete) {
		return;
	}
	time_--;
	if (time_ <= 0) {
		isComplete = true;
		// コールバック関数呼び出し
		f_();
	}
}