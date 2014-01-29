#pragma once
#include "vec.h"
#include "model_controller.h"
#include <cstring>

using std::string;
using Angel::vec2;
using Angel::vec3;

class GLUI;
class GLUI_HSlider;
class GLUI_StaticText;
class GLUI_Panel;


class SliderBundle : public Observer {
private:
	const vec2<> _defVals;
	const vec2<> _bounds;
	const bool _twoSided;
	/** glui directly writes to this value. data binding between this and the 
	 * value in 3dlicmodel is established via gluipresentationlayer::OnGLUICallback.
	 */
	vec2<> _vals;
	const int _callbackID;
	const string _format;
	GLUI_HSlider* const _slider;
	GLUI_StaticText* const _label;
public:
	SliderBundle(GLUI* main_glui, GLUI_Panel* panel, vec2<> def_vals,
		vec2<> bounds, int id, const string format, void (*callback)(int id),
		bool is_two_sided = false);
	void Refresh();
	void Reset();
	bool IsDual() const;
	vec2<> GetValue() const;
	virtual void Update(const Observable* const observable);
private: //non-copyable, non-assignable
	SliderBundle(const SliderBundle& other);
	SliderBundle& operator=(const SliderBundle& other);
};
