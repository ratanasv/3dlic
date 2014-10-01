#include "StdAfx.h"
#include "RegenerateNoise.h"
#include "3dlic_model.h"
#include <thread>
#include "FilteredNoise.h"

using namespace std;

mutex _mutex;
shared_ptr<TextureData> _lowFreqNoisefactory;
shared_ptr<TextureData> _highFreqNoisefactory;

RegenerateNoise::RegenerateNoise(const LICFloatParam& param) : 
	_param(param)
{
	_lastSeenValue = GetTDLPInstance().GetFloatParameter(_param).GetFloat();
	_lastSeenSigma = GetTDLPInstance().GetFloatParameter(LICFloatParam::NOISE_SIGMA)
		.GetFloat();
	thread work(*this);
	work.detach();
}

void RegenerateNoise::operator()() {
	while (true) {
		float newValue = GetTDLPInstance().GetFloatParameter(_param).GetFloat();
		float newSigma = GetTDLPInstance().GetFloatParameter(LICFloatParam::NOISE_SIGMA)
			.GetFloat();
		if (_lastSeenValue != newValue || _lastSeenSigma != newSigma) {
			_lastSeenValue = newValue;
			_lastSeenSigma = newSigma;
			shared_ptr<TextureData> newLowFreqNoiseFactory(new FilteredNoise(2, 3, 5, 256, 
				_lastSeenValue, _lastSeenSigma));
			shared_ptr<TextureData> newHighFreqNoiseFactory(new FilteredNoise(2, 3, 5, 256, 
				_lastSeenValue * 3.0, _lastSeenSigma));
			{
				lock_guard<mutex> synchronized(_mutex);
				_lowFreqNoisefactory = newLowFreqNoiseFactory;
				_highFreqNoisefactory = newHighFreqNoiseFactory;
			}
		}
			
		this_thread::sleep_for(chrono::milliseconds(100));
	}

}

void RegenerateNoise::RunInMainThread(std::shared_ptr<GLTexture>& lowFreqNoise,
	std::shared_ptr<GLTexture>& highFreqNoise) 
{
	lock_guard<mutex> synchronized(_mutex);
	if (_lowFreqNoisefactory) {
		lowFreqNoise->send_to_gpu(_lowFreqNoisefactory);
		highFreqNoise->send_to_gpu(_highFreqNoisefactory);
		_lowFreqNoisefactory.reset();
		_highFreqNoisefactory.reset();
	}
}
