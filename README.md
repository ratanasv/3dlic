3dlic
=====

## What is this?
My project for implementing [Output-sensitive 3D Line Integral Convolutions](http://www.vis.uni-stuttgart.de/texflowvis/3dlic/). An improvement is being introduced to compute [sparse noise](http://en.wikipedia.org/wiki/Halton_sequence) on the GPU. 

## Guideline for adding another GLSL uniform variable.

- Add your enum to `THREEDLICParameters`.
- Add default values and register it in THREEDLICParameters's default ctor.
- (optional) Create a GLUI element in `glui_utility::create_proj6_panel`.
- Send it to the GPU before the draw call in `gui6::draw6`.

## Uniform variables data binding
In general we need to sync between `SliderBundle::_vals` & `THREEDLICParameters::_floatParams[id]` & `SliderBundle::_label`. 
### The Mechanism

 1. User interacts with a GLUI element
 1. GLUI Internally calls its callback func 
 1. In this case THE callback is `GLUIPresentationLayer::OnGLUICallback`
 1. `OnGLUICallback` then updates the model `(THREEDLICParameters)`.
 1. `THREEDLICParameters` then notify all observers of state changes. 
