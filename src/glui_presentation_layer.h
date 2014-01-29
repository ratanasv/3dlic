#pragma once
#include <map>
#include <memory>
#include <cstring>


using std::map;
using std::shared_ptr;
using std::string;

enum class LICFloatParam : int;
enum class LICBoolParam : int;
class SliderBundle;
class GLUI;
class GLUI_Panel;

void CallbackMemFunWrapper(int id);

// class for dealing with GLUI since it writes directly to memory.
class GLUIPresentationLayer {
private:
	map<LICFloatParam, shared_ptr<SliderBundle>> _sliders;
	map<LICFloatParam, LICFloatParam> _dualSliderIDs;
private: //enforces a singleton property.
	GLUIPresentationLayer() {};
	GLUIPresentationLayer(const GLUIPresentationLayer& other);
	GLUIPresentationLayer& operator=(const GLUIPresentationLayer& other);
public:
	void InsertSlider(GLUI* main_glui, GLUI_Panel* panel, LICFloatParam param, 
		const string& label);
	void InsertDualSlider(GLUI* main_glui, GLUI_Panel* panel, 
		LICFloatParam paramOne, LICFloatParam paramTwo, const string& label);
	void InsertCheckBox(GLUI* main_glui, GLUI_Panel* panel, LICBoolParam param);
	friend void ::CallbackMemFunWrapper(int id);
private:
	int GetCallbackID(LICFloatParam pone, LICFloatParam ptwo);
	/**
	 * Establishes data binding between the slider and the model.
	 */
	void OnGLUICallback(const int callbackID);
public:
	static GLUIPresentationLayer* INSTANCE;
};