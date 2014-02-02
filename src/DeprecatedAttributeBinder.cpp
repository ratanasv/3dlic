#include "StdAfx.h"
#include "DeprecatedAttributeBinder.h"

void DeprecatedAttributeBinder::EnablePositionAttribute(const int vecLength /*= 4 */) {
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(vecLength, GL_FLOAT, 0, 0);
}

void DeprecatedAttributeBinder::EnableNormalAttribute(const int vecLength /*= 4 */) {
	glEnableClientState(GL_NORMAL_ARRAY);
	glVertexPointer(vecLength, GL_FLOAT, 0, 0);
}

void DeprecatedAttributeBinder::EnableColorAttribute(const int vecLength /*= 4 */) {
	glEnableClientState(GL_COLOR_ARRAY);
	glVertexPointer(vecLength, GL_FLOAT, 0, 0);
}

void DeprecatedAttributeBinder::EnableTexCoordAttribute(const int vecLength /*= 4 */) {
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glTexCoordPointer(vecLength, GL_FLOAT, 0, 0);
}