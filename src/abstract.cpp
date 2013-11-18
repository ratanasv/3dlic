#include "StdAfx.h"
#include "abstract.h"


namespace vir{

	Abstract::Abstract( const char* value )
	{
		set(value);
	}

	Abstract::Abstract( int value )
	{
		set(value);
	}

	Abstract::Abstract( bool value )
	{
		set(value);
	}

	Abstract::Abstract( float value )
	{
		set(value);
	}
	
	Abstract::Abstract(const Abstract& value)
	{
		set(value);
	}

	Abstract::~Abstract()
	{
		clear();
	}

	Abstract::TYPE Abstract::get_type() const
	{
		return Type;
	}

	std::shared_ptr<char> Abstract::get_string() const
	{
		switch (Type) {
		case VALUE_TYPE_STRING:{
			int length = strlen((char*)Blob)+1;
			char* s = new char[length];
			strcpy(s, (char*)Blob);
			return std::shared_ptr<char>(s, del<char>);
							   }
		case VALUE_TYPE_INT:{
			std::stringstream ss;//create a stringstream
			ss << *((int*)Blob);//add number to the stream
			const char* cstr = ss.str().c_str();
			int length = strlen(cstr)+1;
			char* s = new char[length];
			strcpy(s, cstr);
			return std::shared_ptr<char>(s, del<char>);
							}
		case VALUE_TYPE_FLOAT:{
			std::stringstream ss;//create a stringstream
			ss << *((float*)Blob);//add number to the stream
			const char* cstr = ss.str().c_str();
			int length = strlen(cstr)+1;
			char* s = new char[length];
			strcpy(s, cstr);
			return std::shared_ptr<char>(s, del<char>);
							  }
		case VALUE_TYPE_BOOL:{
			char t[] = "true";
			char f[] = "false";
			if(get_bool()){
				int length = strlen(t)+1;
				char* s = new char[length];
				strcpy(s, (char*)t);
				return std::shared_ptr<char>(s, del<char>);
			}
			else{
				int length = strlen(f)+1;
				char* s = new char[length];
				strcpy(s, (char*)f);
				return std::shared_ptr<char>(s, del<char>);
			}
							 }
		default:{
			char* s = new char[1];
			s[0] = '\0';
			return std::shared_ptr<char>(s, del<char>);
				}
		}
		
	}

	int Abstract::get_int() const
	{
		switch (Type) {
		case VALUE_TYPE_STRING:
			return atoi((char*)Blob);

		case VALUE_TYPE_INT:
			return *((int*)Blob);

		case VALUE_TYPE_BOOL:
			return get_bool() ? 1 : 0; 

		case VALUE_TYPE_FLOAT:
			return (int) get_float();
		default:
			return -1;
		}
	}

	bool Abstract::get_bool() const
	{
		switch (Type) {
		case VALUE_TYPE_STRING:
			return strlen((char*)Blob)>0;

		case VALUE_TYPE_INT:
		case VALUE_TYPE_FLOAT:
			return (get_int() != 0);

		case VALUE_TYPE_BOOL:
			return *((bool*) Blob);

		default:
			return false;
		}
	}

	float Abstract::get_float() const
	{
		switch(Type) {
		case VALUE_TYPE_STRING:
			return atof((char*)Blob);

		case VALUE_TYPE_INT:
			return (float) get_int();
		case VALUE_TYPE_BOOL:
			return get_bool() ? 1.0 : 0.0;

		case VALUE_TYPE_FLOAT:
			return *((float*)Blob);
		default:
			return -1.0;
		}
	}

	void Abstract::set( const char* value )
	{
		clear();
		int length = strlen(value)+1;
		Blob = new char[length];
		strcpy((char*)Blob, value);
		Type = VALUE_TYPE_STRING;
	}

	void Abstract::set( int value )
	{
		clear();
		Blob = new int;
		*((int*)Blob) = value;
		Type = VALUE_TYPE_INT;
	}

	void Abstract::set( bool value )
	{
		clear();
		Blob = new bool;
		*((bool*)Blob) = value;
		Type = VALUE_TYPE_BOOL;
	}

	void Abstract::set( float value )
	{
		clear();
		Blob = new float;
		*((float*)Blob) = value;
		Type = VALUE_TYPE_FLOAT;
	}

	void Abstract::set( const Abstract& value )
	{
		clear();
		switch(value.get_type()) {
		case VALUE_TYPE_STRING:
			set(value.get_string().get());
			break;

		case VALUE_TYPE_INT:
			set(value.get_int());
			break;

		case VALUE_TYPE_BOOL:
			set(value.get_bool());
			break;

		case VALUE_TYPE_FLOAT:
			set(value.get_float());
			break;
		}
	}

	void Abstract::clear()
	{
		switch(get_type()){
		case Abstract::VALUE_TYPE_STRING:
			delete[] (char*)Blob;
			break;
		case Abstract::VALUE_TYPE_INT:
			delete (int*)Blob;
			break;
		case Abstract::VALUE_TYPE_BOOL:
			delete (bool*)Blob;
			break;
		case Abstract::VALUE_TYPE_FLOAT:
			delete (float*)Blob;
			break;
		}
		Blob = NULL;
		Type = VALUE_TYPE_NONE;
	}

	bool Abstract::operator==( const Abstract& other ) const
	{
		switch(Type) {
		case VALUE_TYPE_NONE:
			if (other.get_type() == VALUE_TYPE_NONE) {
				return true;
			}
			else {
				return false;
			}
		case VALUE_TYPE_STRING:
			if(strcmp(get_string().get(), other.get_string().get()) == 0)
				return true;
			else 
				false;
		case VALUE_TYPE_INT:
			return get_int() == other.get_int();
		case VALUE_TYPE_FLOAT:
			return get_float() == other.get_float();
			break;
		default:
			return false;
			break;
		}
	}

	bool Abstract::operator>( const Abstract& other ) const
	{
		switch(Type) {
		case VALUE_TYPE_NONE:
			return false;

		case VALUE_TYPE_STRING:
			if(strcmp(get_string().get(), other.get_string().get()) > 0)
				return true;
			else 
		case VALUE_TYPE_INT:
			return get_int() > other.get_int();
		case VALUE_TYPE_FLOAT:
			return get_float() > other.get_float();
		case VALUE_TYPE_BOOL:
			return get_int() > other.get_int();
		default:
			return false;
		}
	}



}