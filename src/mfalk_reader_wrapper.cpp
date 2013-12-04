#include "StdAfx.h"
#include "mfalk_reader_wrapper.h"
#include <stdexcept>

using std::runtime_error;

MFalkDataTex3DFactory::MFalkDataTex3DFactory(const string& fileName) {
	bool isOk = _datFile.parseDatFile(const_cast<char*>(fileName.c_str()));
	if (!isOk) {
		MessageBox( NULL, "Cannot parse DatFile", "Error", MB_ICONSTOP);
		exit(EXIT_FAILURE);
	}
}


GLenum MFalkDataTex3DFactory::getInternalFormat() {
	if (_datFile.getDataDimension() != 3) {
		throw runtime_error("texture dat file dimension is not 3");
	}
	return GL_RGB;
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
	if (_datFile.getDataDimension() != 3) {
		throw runtime_error("texture dat file dimension is not 3");
	}
	return GL_RGB;
}

GLenum MFalkDataTex3DFactory::getType() {
	auto dataType = _datFile.getDataType();
	if (dataType == DATRAW_UCHAR)  {
		return GL_UNSIGNED_BYTE;
	} else if (dataType == DATRAW_FLOAT) {
		return GL_FLOAT;
	} else {
		throw runtime_error("texture datatype is not supported");
	}
}

void* MFalkDataTex3DFactory::get_data() {
	return _datFile.readRawData();
}

