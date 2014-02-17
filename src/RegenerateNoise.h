#pragma once
#include "LICFloatParam.h"
#include <memory>
#include <mutex>

class GLTexture;

class RegenerateNoise {
private:
	const LICFloatParam _param;
	float _lastSeenValue;
	float _lastSeenSigma;

public:
	RegenerateNoise(const LICFloatParam& param);
	void operator()();
	void RunInMainThread(std::shared_ptr<GLTexture>& noise);
};