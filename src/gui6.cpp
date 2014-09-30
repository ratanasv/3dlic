#include "StdAfx.h"
#include "gui6.h"
#include "glui_utility.h"
#include "virtex.h"
#include "virmodel.h"
#include "texture6.h"
#include "3dlic_model.h"
#include "mfalk_reader_wrapper.h"
#include "NoiseTex3DFactory.h"
#include "FilteredNoise.h"
#include "LICFloatParam.h"
#include "RegenerateNoise.h"
#include "ProceduralTornado.h"

using namespace vir;
using std::string;
namespace fs = boost::filesystem;

extern float Xrot;
extern float Yrot;	

shared_ptr<GLSLProgram> VolumeTracingShader;
static shared_ptr<GLSLProgram> TwoDLICShader;
static shared_ptr<GLTexture> SparseNoise;
static shared_ptr<VirModel> Cube;
static shared_ptr<GLTexture> VectorDataTexture;
static shared_ptr<GLTexture> VirNoise;
static RegenerateNoise* RegenNoise = NULL;
static shared_ptr<GLTexture> KernelNoise;
static shared_ptr<GLTexture> Noise2D;
static shared_ptr<VirModel> Quad;

static void BindFloatUniform(const char* var, LICFloatParam param, 
	const shared_ptr<GLSLProgram>& program = VolumeTracingShader) 
{
	program->SetUniform(var, GetTDLPInstance()
		.GetFloatParameter(param).GetFloat());
}

static void BindBoolUniform(const char* var, LICBoolParam param,
	const shared_ptr<GLSLProgram>& program = VolumeTracingShader) 
{
	program->SetUniform(var, GetTDLPInstance()
		.GetBoolParameter(param).GetBool());
}


void draw6() {
	RegenNoise->RunInMainThread(VirNoise);

	VolumeTracingShader->Use();
	static shared_ptr<TextureVisitor> sparseNoiseVisitor(new GLSLTextureSamplerBinder(
		VolumeTracingShader, "uSparseNoiseSampler"));
	static shared_ptr<TextureVisitor> virNoiseVisitor(new GLSLTextureSamplerBinder(
		VolumeTracingShader, "uVirNoiseSampler"));
	static shared_ptr<TextureVisitor> vectorDataVisitor(new GLSLTextureSamplerBinder(
		VolumeTracingShader, "uVectorData"));
	VirNoise->pre_render(virNoiseVisitor);
	SparseNoise->pre_render(sparseNoiseVisitor);
	VectorDataTexture->pre_render(vectorDataVisitor);
	BindFloatUniform("uNumSteps", LICFloatParam::NUM_STEPS);
	BindFloatUniform("uBaseAlpha", LICFloatParam::BASE_ALPHA);
	BindFloatUniform("uRainbowValMin", LICFloatParam::RAINBOW_VAL_MIN);
	BindFloatUniform("uRainbowValMax", LICFloatParam::RAINBOW_VAL_MAX);
	BindFloatUniform("uNumStepsLIC", LICFloatParam::NUM_STEPS_LIC);
	BindFloatUniform("uVelocityScale", LICFloatParam::VELOCITY_SCALE);
	BindFloatUniform("uDT", LICFloatParam::DT);
	BindFloatUniform("uMinMagnitude", LICFloatParam::MAGNITUDE_MIN);
	BindFloatUniform("uMaxMagnitude", LICFloatParam::MAGNITUDE_MAX);
	BindFloatUniform("uColorIntensity", LICFloatParam::COLOR_INTENSITY);
	BindBoolUniform("uShowForwardVectorDiff", LICBoolParam::SHOW_FORWARD_VECTOR_DIFF);
	BindBoolUniform("uShowNumIterations", LICBoolParam::SHOW_NUM_ITERATIONS);
	Cube->render();
	VectorDataTexture->post_render();
	SparseNoise->post_render();
	VirNoise->post_render();

	if (GetTDLPInstance().GetBoolParameter(LICBoolParam::CUTTING_PLANE).GetBool()) {

	
	glClear(GL_DEPTH_BUFFER_BIT);
	TwoDLICShader->ClearProjection();
	TwoDLICShader->Perspective( 90., 1.,	0.1, 100. );
	TwoDLICShader->ClearModelView();
	TwoDLICShader->LookAt( vec3<>(0., 0., 15.), vec3<>(0., 0., 0.), vec3<>(0., 1., 0.));

	TwoDLICShader->Translate(
		GetTDLPInstance().GetFloatParameter(
		LICFloatParam::XTRANSLATE).GetFloat(), 
		GetTDLPInstance().GetFloatParameter(
		LICFloatParam::YTRANSLATE).GetFloat(),
		GetTDLPInstance().GetFloatParameter(
		LICFloatParam::ZTRANSLATE).GetFloat()
	);
	TwoDLICShader->Rotate(Xrot, Yrot, 0.0);
	TwoDLICShader->Use();
	static shared_ptr<TextureVisitor> noise2dVisitor(new GLSLTextureSamplerBinder(
		TwoDLICShader, "uNoiseSampler"));
	static shared_ptr<TextureVisitor> vectorData2DVisitor(new GLSLTextureSamplerBinder(
		TwoDLICShader, "uVectorData"));
	VectorDataTexture->pre_render(vectorData2DVisitor);
	Noise2D->pre_render(noise2dVisitor);
	BindFloatUniform("uRainbowValMin", LICFloatParam::RAINBOW_VAL_MIN, TwoDLICShader);
	BindFloatUniform("uRainbowValMax", LICFloatParam::RAINBOW_VAL_MAX, TwoDLICShader);
	BindFloatUniform("uNumStepsLIC", LICFloatParam::NUM_STEPS_LIC, TwoDLICShader);
	BindFloatUniform("uVelocityScale", LICFloatParam::VELOCITY_SCALE, TwoDLICShader);
	BindFloatUniform("uDT", LICFloatParam::DT, TwoDLICShader);
	BindFloatUniform("uMinMagnitude", LICFloatParam::MAGNITUDE_MIN, TwoDLICShader);
	BindFloatUniform("uMaxMagnitude", LICFloatParam::MAGNITUDE_MAX, TwoDLICShader);
	BindFloatUniform("uZ", LICFloatParam::Z_CUTTING_PLANE, TwoDLICShader);
	Quad->render();
	Noise2D->post_render();
	VectorDataTexture->post_render();
	
	}
}

void reset6() {
// 	for (auto it : BunchOfSliders) {
// 		it.second.reset();
// 	}
// 	for (auto it : BunchOfCheckboxes) {
// 		it.second.reset();
// 	}
}

void animate6(float t) {
	
}


void init6() {
	ExistingPath VERTEX_SHADER_PATH(GetStringProperty(Property::PATH_VERTEX_SHADER));
	ExistingPath FRAGMENT_SHADER_PATH(GetStringProperty(Property::PATH_FRAGMENT_SHADER));
	ExistingPath NOISE_PATH(GetStringProperty(Property::PATH_NOISE));
	ExistingPath DATA_PATH(GetStringProperty(Property::PATH_DATA));
	ExistingPath GAUSSIAN_COMPUTE_SHADER_PATH(GetStringProperty(
		Property::PATH_GAUSSIAN_COMPUTE_SHADER));
	ExistingPath KERNEL_PATH(GetStringProperty(
		Property::PATH_GAUSSIAN_COMPUTE_SHADER));

	VolumeTracingShader.reset(new GLSLProgram());
	VolumeTracingShader->Create(VERTEX_SHADER_PATH.c_str(),
		FRAGMENT_SHADER_PATH.c_str());
	VolumeTracingShader->Use();

	TwoDLICShader.reset(new GLSLProgram());
	TwoDLICShader->Create(GetStringProperty(Property::PATH_VS_2DLIC).c_str(),
		GetStringProperty(Property::PATH_FS_2DLIC).c_str());

	
	shared_ptr<TextureData> factory(new NoiseTex3DFactory(
		NOISE_PATH, 1));
	SparseNoise.reset(new GLTexture(factory));

	factory.reset(new ProceduralTornado(128, 128, 128, 0));
	VectorDataTexture.reset(new GLTexture(factory));

	shared_ptr<GeometryAbstractFactory> cubeFactory(new CubeGeometryFactory());
	shared_ptr<GeometryDelegatee> vaoFreeable(new VAODelegatee(cubeFactory, 
		VolumeTracingShader));
	Cube.reset(new VirModel(vaoFreeable));

	factory.reset(new FilteredNoise(2, 3, 5, 256, 
		GetTDLPInstance().GetFloatParameter(LICFloatParam::NOISE_DENSITY).GetFloat(), 
		0.5));
	VirNoise.reset(new GLTexture(factory));

	factory.reset(new Texture2DData(GetStringProperty(Property::PATH_NOISE2D)));
	Noise2D.reset(new GLTexture(factory));

	shared_ptr<GeometryAbstractFactory> planeFactory(new PlaneGeometryFactory());
	vaoFreeable.reset(new VAODelegatee(planeFactory, TwoDLICShader));
	Quad.reset(new VirModel(vaoFreeable));

	RegenNoise = new RegenerateNoise(LICFloatParam::NOISE_DENSITY);

}
