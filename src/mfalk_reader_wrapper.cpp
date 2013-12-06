#include "StdAfx.h"
#include "mfalk_reader_wrapper.h"
#include <stdexcept>
#include <algorithm>

using std::runtime_error;
using Angel::vec4;
using std::transform;
using std::minmax_element;

MFalkDataTex3DFactory::MFalkDataTex3DFactory(const string& fileName, 
	GLenum internalFormat) : _internalFormat(internalFormat) 
{
	bool isOk = _datFile.parseDatFile(const_cast<char*>(fileName.c_str()));
	if (!isOk) {
		MessageBox( NULL, "Cannot parse DatFile", "Error", MB_ICONSTOP);
		exit(EXIT_FAILURE);
	}
}


GLenum MFalkDataTex3DFactory::getInternalFormat() {
	return _internalFormat;
}

int MFalkDataTex3DFactory::getWidth() {
	return _datFile.getDataSizes()[0];
}

int MFalkDataTex3DFactory::getHeight() {
	return _datFile.getDataSizes()[1];
}

int MFalkDataTex3DFactory::getDepth() {
	return _datFile.getDataSizes()[2];
}

GLenum MFalkDataTex3DFactory::getFormat() {
	return GL_RGBA;
}

GLenum MFalkDataTex3DFactory::getType() {
	return GL_FLOAT;
}


class Normalizer {
public:
	Normalizer(float center, float radius) : _center(center), _radius(radius) {};
	vec4<> operator() (const vec3<>& vec) {
		float vecLength = length(vec);
		return vec4<>(vec/vecLength*_radius + _center, vecLength);
	}
private:
	const float _center;
	const float _radius;
};



shared_ptr<void> MFalkDataTex3DFactory::get_data() {
	const unsigned numVectors = getWidth()*getHeight()*getDepth();
	shared_ptr<vec4<>> data;
	if (_internalFormat == GL_RGBA32F || _internalFormat == GL_RGBA16F) {
		data = getNormalizedData(Normalizer(0.0, 1.0));
	} else if (_internalFormat == GL_RGBA8 || _internalFormat == GL_RGBA) {
		data = getNormalizedData(Normalizer(0.5, 0.5));
	} else {
		throw runtime_error("texture datatype is not supported");
	}

	auto minMax = minmax_element(data.get(), data.get() + numVectors, 
		[](const vec4<>& a, const vec4<>& b) {
			return a.w < b.w;
		}
	);
	float maxLength = minMax.second->w;
	transform(data.get(), data.get() + numVectors, data.get(),
		[&](const vec4<>& vec) {
			vec4<> scaled = vec;
			scaled[3] = scaled[3]/maxLength;
			return scaled;
		}
	);
	return data;

}

shared_ptr<vec4<>> MFalkDataTex3DFactory::getNormalizedData(
	const Normalizer& normalizer) 
{
	const unsigned numVectors = getWidth()*getHeight()*getDepth();
	auto scaledData = initCStyleArray(new vec4<>[numVectors]);
	auto floatData = getFloatVecData();
	transform(floatData.get(), floatData.get() + numVectors, scaledData.get(), 
		normalizer);


	return scaledData;
}

shared_ptr<vec3<>> MFalkDataTex3DFactory::getFloatVecData() {
	const unsigned numVectors = getWidth()*getHeight()*getDepth();
	if (_datFile.getDataType() == DATRAW_UCHAR) {
		auto rawData = initCStyleArray((vec3<unsigned char>*)_datFile.readRawData(
			_datFile.getTimeStepBegin()));
		auto floatData = initCStyleArray(new vec3<>[numVectors]);
		transform(rawData.get(), rawData.get() + numVectors, floatData.get(),
			[](const vec3<unsigned char>& vec) {
				return vec3<>(vec.x, vec.y, vec.z);	
			}
		);
		return floatData;
	} else if (_datFile.getDataType() == DATRAW_FLOAT) {
		return initCStyleArray(static_cast<vec3<>*>(_datFile.readRawData(
			_datFile.getTimeStepEnd())));
	} else {
		throw runtime_error("texture datatype is not supported");
	}
}

