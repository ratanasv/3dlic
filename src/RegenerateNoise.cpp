#include "StdAfx.h"
#include "RegenerateNoise.h"
#include "3dlic_model.h"
#include <thread>
#include "FilteredNoise.h"

using namespace std;

mutex _mutex;
shared_ptr<GLTexture> _textureDelegatee;

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
			shared_ptr<TextureData> factory(new FilteredNoise(2, 3, 5, 256, 
				_lastSeenValue, _lastSeenSigma));
			{
				lock_guard<mutex> synchronized(_mutex);
				_textureDelegatee.reset(new GLTexture());
				_textureDelegatee->send_to_gpu(factory);
			}
		}
			
		this_thread::sleep_for(chrono::milliseconds(100));
	}

}

void RegenerateNoise::RunInMainThread(std::shared_ptr<GLTexture>& noise) {
	lock_guard<mutex> synchronized(_mutex);
	if (_textureDelegatee) {
		noise = _textureDelegatee;
	}
}
