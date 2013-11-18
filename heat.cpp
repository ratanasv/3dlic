#include "StdAfx.h"
#include "heat.h"
using namespace vir;

static const float TEMP_MAX = 100.0;
static const float TEMP_MIN = 0.0;

static void hsv_to_rgb(float hsv[3], float rgb[3])
{
	float tmp1 = hsv[2] * (1-hsv[1]);
	float tmp2 = hsv[2] * (1-hsv[1] * (hsv[0] / 60.0f - (int) (hsv[0]/60.0f) ));
	float tmp3 = hsv[2] * (1-hsv[1] * (1 - (hsv[0] / 60.0f - (int) (hsv[0]/60.0f) )));
	switch((int)(hsv[0] / 60)) {
	case 0:
		rgb[0] = hsv[2] ;
		rgb[1] = tmp3 ;
		rgb[2] = tmp1 ;
		break;
	case 1:
		rgb[0] = tmp2 ;
		rgb[1] = hsv[2] ;
		rgb[2] = tmp1 ;
		break;
	case 2:
		rgb[0] = tmp1 ;
		rgb[1] = hsv[2] ;
		rgb[2] = tmp3 ;
		break;
	case 3:
		rgb[0] = tmp1 ;
		rgb[1] = tmp2 ;
		rgb[2] = hsv[2] ;
		break;
	case 4:
		rgb[0] = tmp3 ;
		rgb[1] = tmp1 ;
		rgb[2] = hsv[2] ;
		break;
	case 5:
		rgb[0] = hsv[2] ;
		rgb[1] = tmp1 ;
		rgb[2] = tmp2 ;
		break;
	default:
		fprintf(stderr,  "What!? Inconceivable!\n");
	}
}

static float get_heat( float x, float y, float z )
{
	struct centers
	{
		float xc, yc, zc;       // center location
		float a;                // amplitude
	};
	static struct centers Centers[] =
	{
		{	 1.00f,	 0.00f,	 0.00f,	 90.00f	},
		{	-1.00f,	 0.30f,	 0.00f,	120.00f	},
		{	 0.00f,	 1.00f,	 0.00f,	120.00f	},
		{	 0.00f,	 0.40f,	 1.00f,	170.00f	},
	};
	float t = 0.0;
	for( int i = 0; i < 4; i++ )
	{
		float dx = x - Centers[i].xc;
		float dy = y - Centers[i].yc;
		float dz = z - Centers[i].zc;
		float rsqd = SQR(dx) + SQR(dy) + SQR(dz);
		t += Centers[i].a * exp( -5.*rsqd );
	}

	if( t > TEMP_MAX )
		t = TEMP_MAX;
	return t;
}

float NodesImpl::get_max_gradient()
{
	static bool has_been_init = false;
	static float grad_max = FLT_MIN;
	if(!has_been_init){
		for(int i=0; i<NX; i++){
			for(int j=0; j<NY; j++){
				for(int k=0; k<NZ; k++){
					float grad = my_nodes[i][j][k].grad;
					if(grad > grad_max)
						grad_max = grad;
				}
			}
		}
		has_been_init = true;
	}
	return grad_max;
}

struct node& NodesImpl::get_node( int i, int j, int k )
{
	return my_nodes[i][j][k];
}

NodesImpl::NodesImpl()
{
	for(int i=0; i<NX; i++)
		for(int j=0; j<NY; j++)
			for(int k=0; k<NZ; k++)
				fill_one_node(i,j,k);
	for(int i=0; i<NX; i++)
		for(int j=0; j<NY; j++)
			for(int k=0; k<NZ; k++)
				init_node_derivative(i,j,k);
	for(int i=0; i<NX; i++)
		for(int j=0; j<NY; j++)
			for(int k=0; k<NZ; k++)
				init_node_gradient_color(i,j,k);
}

void NodesImpl::init_node_gradient_color( int i, int j, int k )
{
	struct node& one_node = my_nodes[i][j][k];
	float rgb[3];
	float hue = (one_node.grad-0.0)/(get_max_gradient()-0.0);
	hue = 240.0*(1.0-hue);
	float hsv[3] = {hue, 1.0, 1.0};
	hsv_to_rgb(hsv, rgb);
	one_node.gr = rgb[0];
	one_node.gg = rgb[1];
	one_node.gb = rgb[2];
}

void NodesImpl::init_node_derivative( int i, int j, int k )
{
	struct node& current = my_nodes[i][j][k];
	if(i == 0){
		struct node& next = my_nodes[i+1][j][k];
		current.dTdx = (next.T-current.T)/(next.x-current.x);
	}
	else if(i == NX-1){
		struct node& previous = my_nodes[i-1][j][k];
		current.dTdx = (current.T-previous.T)/(current.x-previous.x);
	}
	else{
		struct node& next = my_nodes[i+1][j][k];
		struct node& previous = my_nodes[i-1][j][k];
		current.dTdx = (next.T-previous.T)/(next.x-previous.x);
	}
	if(j == 0){
		struct node& next = my_nodes[i][j+1][k];
		current.dTdy = (next.T-current.T)/(next.y-current.y);
	}
	else if(j == NY-1){
		struct node& previous = my_nodes[i][j-1][k];
		current.dTdy = (current.T-previous.T)/(current.y-previous.y);
	}
	else{
		struct node& next = my_nodes[i][j+1][k];
		struct node& previous = my_nodes[i][j-1][k];
		current.dTdy = (next.T-previous.T)/(next.y-previous.y);
	}
	if(k == 0){
		struct node& next = my_nodes[i][j][k+1];
		current.dTdz = (next.T-current.T)/(next.z-current.z);
	}
	else if(k == NZ-1){
		struct node& previous = my_nodes[i][j][k-1];
		current.dTdz = (current.T-previous.T)/(current.z-previous.z);
	}
	else{
		struct node& next = my_nodes[i][j][k+1];
		struct node& previous = my_nodes[i][j][k-1];
		current.dTdz = (next.T-previous.T)/(next.z-previous.z);
	}
	current.grad = sqrt(SQR(current.dTdx)+SQR(current.dTdy)+SQR(current.dTdz));
}

void NodesImpl::fill_one_node( int i, int j, int k )
{
	struct node& one_node = my_nodes[i][j][k];
	float x = -1.  +  2. * (float)i / (float)(NX-1);
	float y = -1.  +  2. * (float)j / (float)(NY-1);
	float z = -1.  +  2. * (float)k / (float)(NZ-1);
	one_node.x = x;
	one_node.y = y;
	one_node.z = z;
	one_node.T = get_heat(x,y,z);
	float hue = 240.0*(one_node.T-TEMP_MIN)/(TEMP_MAX-TEMP_MIN);
	hue = 240.0 - hue;
	float rgb[3];
	float hsv[3] = {hue, 1.0, 1.0};
	hsv_to_rgb(hsv, rgb);
	one_node.r = rgb[0];
	one_node.g = rgb[1];
	one_node.b = rgb[2];
	one_node.rad = sqrt(SQR(x)+SQR(y)+SQR(z));
}

float NodesImpl::heat( vir::vec3 pos )
{
	return get_heat(pos.x, pos.y, pos.z);
}

vir::vec3 NodesImpl::get_heat_color( vir::vec3 pos )
{
	float h = get_heat(pos.x, pos.y, pos.z);
	float hue = 240.0*(h-TEMP_MIN)/(TEMP_MAX-TEMP_MIN);
	hue = 240.0 - hue;
	float rgb[3];
	float hsv[3] = {hue, 1.0, 1.0};
	hsv_to_rgb(hsv, rgb);
	return vec3(rgb[0], rgb[1], rgb[2]);
}

vir::vec3 NodesImpl::get_pos( vir::vec3 indices )
{
	return -1.0 + 2.0*indices/(vir::vec3(NX,NY,NZ)-1.0);
}


NodesImpl& nodes()
{
	static NodesImpl nodes;
	return nodes;
}