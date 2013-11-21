#include "StdAfx.h"
#include "virmodel.h"

using namespace vir;
using std::transform;

template <class T> shared_ptr<vector<T>> initSmartArray(const int size = 0) {
	shared_ptr<vector<T>> returned(new vector<T>(size));
	return returned;
}


OBJFactory::OBJFactory( const char* fn ) {
	std::filebuf fb;
	if (fb.open (fn,std::ios::in)) {
		std::istream is(&fb);
		_model = obj::parseObjModel(is);
		fb.close();
	}

	initVertices();
	initNormals();
	initTexCoords();
	initIndices();
}




shared_ptr<vector<vir::vec3>> OBJFactory::get_vertices() {
	return vertices;
}

shared_ptr<vector<vir::vec3>> OBJFactory::get_normals() {
	return normals;
}

shared_ptr<vector<vir::vec3>> OBJFactory::get_tex_coord() {
	return texCoords;
}

shared_ptr<vector<unsigned>> OBJFactory::get_indices() {
	return indices;
}

void OBJFactory::initVertices() {
	vertices = shared_ptr<vector<vec3>>(new vector<vec3>(
		_model.vertex.size()/3, vec3() ));
	memcpy(vertices->data(), _model.vertex.data(), _model.vertex.size()*
		sizeof(float));
}

void OBJFactory::initNormals() {
	normals = shared_ptr<vector<vec3>>(new vector<vec3>(
		_model.normal.size()/3, vec3() ));
	memcpy(normals->data(), _model.normal.data(), _model.normal.size()*
		sizeof(float));
}

void OBJFactory::initTexCoords() {
	texCoords = initSmartArray<vir::vec3>();
	for (int i=0; i<_model.texCoord.size(); i=i+2) {
		texCoords->push_back(vir::vec3(_model.texCoord.at(i), 
			_model.texCoord.at(i+1),0.0f));
	}
}

void OBJFactory::initIndices() {
	auto raw_indices = _model.faces.begin()->second.first;
	indices = shared_ptr<vector<unsigned>>(new vector<unsigned>);
	for(int i=0; i<raw_indices.size(); i++){
		indices->push_back(raw_indices.at(i).v);
	}
}

void VAOFreeableDelegatee::freeCPUMemory() {
	_factory.reset();
}

VAODelegatee::VAODelegatee( const shared_ptr<GeometryAbstractFactory>& factory ) :
GeometryDelegatee(factory) {
	_num_indices = factory->get_indices()->size();
};

void VAODelegatee::send_to_gpu() {
	glGenVertexArrays( 1, &_vao );
	glBindVertexArray( _vao );

	GLuint v_buf, n_buf, vt_buf, in_buf;
	glGenBuffers(1, &v_buf);
	glBindBuffer(GL_ARRAY_BUFFER, v_buf);
	auto verts = _factory->get_vertices();
	glBufferData(GL_ARRAY_BUFFER, sizeof(vec3)*verts->size(), verts->data(), GL_STATIC_DRAW);
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, 0);

	glGenBuffers(1, &n_buf);
	glBindBuffer(GL_ARRAY_BUFFER, n_buf);
	auto norms = _factory->get_normals();
	glBufferData(GL_ARRAY_BUFFER, sizeof(vec3)*norms->size(), norms->data(), GL_STATIC_DRAW);
	glEnableClientState(GL_NORMAL_ARRAY);
	glNormalPointer(GL_FLOAT, 0, 0);

	glGenBuffers(1, &vt_buf);
	glBindBuffer(GL_ARRAY_BUFFER, vt_buf);
	auto vts = _factory->get_tex_coord();
	glBufferData(GL_ARRAY_BUFFER, sizeof(vec3)*vts->size(), vts->data(), GL_STATIC_DRAW);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glTexCoordPointer(3, GL_FLOAT, 0, 0);

	glGenBuffers(1, &in_buf);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, in_buf);
	auto ins = _factory->get_indices();
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned)*ins->size(), 
		ins->data(), GL_STATIC_DRAW);

	glBindVertexArray(0);
}

void VAODelegatee::pre_render()
{
	glBindVertexArray(_vao);
}

void VAODelegatee::post_render()
{
	glBindVertexArray(0);
}

void VAODelegatee::render()
{
	glDrawElements(GL_TRIANGLES, _num_indices, GL_UNSIGNED_INT, 0);
}

shared_ptr<vector<vir::vec3>> CubeGeometryFactory::get_vertices() {
	auto returned = initSmartArray<vir::vec3>();
	returned->push_back(_radius*vir::vec3(1.0f, -1.0f, -1.0f));
	returned->push_back(_radius*vir::vec3(1.0f, -1.0f, 1.0f));
	returned->push_back(_radius*vir::vec3(-1.0f, -1.0f, 1.0f));
	returned->push_back(_radius*vir::vec3(-1.0f, -1.0f, -1.0f));
	returned->push_back(_radius*vir::vec3(1.0f, 1.0f, -1.0f));
	returned->push_back(_radius*vir::vec3(1.0f, 1.0f, 1.0f));
	returned->push_back(_radius*vir::vec3(-1.0f, 1.0f, 1.0f));
	returned->push_back(_radius*vir::vec3(-1.0f, 1.0f, -1.0f));
	return returned;
}

shared_ptr<vector<vir::vec3>> CubeGeometryFactory::get_normals() {
	auto returned = initSmartArray<vir::vec3>();
	returned->push_back(vir::vec3(0.408246f, -0.816492f, -0.408246f));
	returned->push_back(vir::vec3(0.816492f, -0.408246f, 0.408246f));
	returned->push_back(vir::vec3(-0.577349f, -0.577349f, 0.577349f));
	returned->push_back(vir::vec3(-0.408246f, -0.408246f, -0.816492f));
	returned->push_back(vir::vec3(0.666646f, 0.333323f, -0.666646f));
	returned->push_back(vir::vec3(0.333323f, 0.666646f, 0.666646f));
	returned->push_back(vir::vec3(-0.577349f, 0.577349f, 0.577349f));
	returned->push_back(vir::vec3(-0.666646f, 0.666646f, -0.333323f));
	return returned;
}

shared_ptr<vector<vir::vec3>> CubeGeometryFactory::get_tex_coord() {
	auto returned = initSmartArray<vir::vec3>();
	returned->push_back(vir::vec3(1.0, 0.0, 0.0));
	returned->push_back(vir::vec3(1.0, 0.0, 1.0));
	returned->push_back(vir::vec3(0.0, 0.0, 1.0));
	returned->push_back(vir::vec3(0.0, 0.0, 0.0));
	returned->push_back(vir::vec3(1.0, 1.0, 0.0));
	returned->push_back(vir::vec3(1.0, 1.0, 1.0));
	returned->push_back(vir::vec3(0.0, 1.0, 1.0));
	returned->push_back(vir::vec3(0.0, 1.0, 0.0));
	return returned;
}

shared_ptr<vector<unsigned>> CubeGeometryFactory::get_indices() {
	unsigned indices[] = {
		4, 0, 3,
		4, 3, 7,
		2, 6, 7,
		2, 7, 3,
		1, 5, 2,
		5, 6, 2,
		0, 4, 1,
		4, 5, 1,
		4, 7, 5,
		7, 6, 5,
		0, 1, 2,
		0, 2, 3
	};
	auto returned = initSmartArray<unsigned>();
	returned->insert(returned->begin(), &indices[0], &indices[16]);
	return returned;
}
