#include "StdAfx.h"
#include "virmodel.h"

using namespace std;
using namespace vir;

OBJFactoryImpl::OBJFactoryImpl( const char* fn )
{
	using namespace obj;
	std::filebuf fb;
	if (fb.open (fn,std::ios::in)){
		std::istream is(&fb);
		_model = parseObjModel(is);
		fb.close();
	}
}




std::shared_ptr<std::vector<vir::vec3>> OBJFactoryImpl::get_vertices()
{
	static shared_ptr<vector<vec3>> vertices(NULL);
	if(vertices.get() == NULL){
		vertices = shared_ptr<vector<vec3>>(new vector<vec3>(
			_model.vertex.size()/3, vec3() ));
		memcpy(vertices->data(), _model.vertex.data(), _model.vertex.size()*
			sizeof(float));

	}
	return vertices;
}

std::shared_ptr<std::vector<vir::vec3>> OBJFactoryImpl::get_normals()
{
	static shared_ptr<vector<vec3>> normals(NULL);
	if(normals.get() == NULL){
		normals = shared_ptr<vector<vec3>>(new vector<vec3>(
			_model.normal.size()/3, vec3() ));
		memcpy(normals->data(), _model.normal.data(), _model.normal.size()*
			sizeof(float));
	}
	return normals;
}

std::shared_ptr<std::vector<vir::vec2>> OBJFactoryImpl::get_tex_coord()
{
	static shared_ptr<vector<vec2>> tex_coord(NULL);
	if(tex_coord.get() == NULL){
		tex_coord = shared_ptr<vector<vec2>>(new vector<vec2>(
			_model.texCoord.size()/2, vec2() ));
		memcpy(tex_coord->data(), _model.texCoord.data(), _model.texCoord.size()*
			sizeof(float));
// 		tex_coord = shared_ptr<vector<vec2>>(new vector<vec2>());
// 		for(int i=0; i<_model.texCoord.size()/2; i++){
// 			tex_coord->push_back(vec2(_model.texCoord.at(2*i+0), 
// 				_model.texCoord.at(2*i+1)));
// 		}
	}
	return tex_coord;
}

std::shared_ptr<std::vector<unsigned>> OBJFactoryImpl::get_indices()
{
	static shared_ptr<vector<unsigned>> indices(NULL);
	if(indices.get() == NULL){
		auto raw_indices = _model.faces.begin()->second.first;
		indices = shared_ptr<vector<unsigned>>(new vector<unsigned>);
		for(int i=0; i<raw_indices.size(); i++){
			indices->push_back(raw_indices.at(i).v);
		}
	}
	return indices;
}

VAODelegateeImpl::VAODelegateeImpl( const GeometryAbstractFactory& factory ):
	GeometryDelegateeImpl(factory){
		_num_indices = factory->get_indices()->size();
	};

void VAODelegateeImpl::send_to_gpu()
{
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
	glBufferData(GL_ARRAY_BUFFER, sizeof(vec2)*vts->size(), vts->data(), GL_STATIC_DRAW);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glTexCoordPointer(2, GL_FLOAT, 0, 0);

	glGenBuffers(1, &in_buf);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, in_buf);
	auto ins = _factory->get_indices();
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned)*ins->size(), 
		ins->data(), GL_STATIC_DRAW);

	glBindVertexArray(0);
}

void VAODelegateeImpl::pre_render()
{
	glBindVertexArray(_vao);
}

void VAODelegateeImpl::post_render()
{
	glBindVertexArray(0);
}

void VAODelegateeImpl::render()
{
	glDrawElements(GL_TRIANGLES, _num_indices, GL_UNSIGNED_INT, 0);
}
