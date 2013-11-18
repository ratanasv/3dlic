#pragma once
#include <cstdio>
#include <memory>
#include <cmath>

namespace vir{
	class Abstract{
	public:
		/** Native data type. */
		typedef enum {
			VALUE_TYPE_NONE = 0,
			VALUE_TYPE_STRING = 1,
			VALUE_TYPE_INT,
			VALUE_TYPE_BOOL,
			VALUE_TYPE_FLOAT,
		} TYPE;
	private:
		void* Blob;
		TYPE Type;
	public:
		/** Constructor. Create a new string value.
		 * @param value A null-terminated c string.
		 */
		Abstract(const char* value);
		/** Constructor. Create a new integer value.
		 * @param value An integer value.
		 */
		Abstract(int value);
		/** Constructor. Create a new boolean value.
		 * @param value A boolean value.
		 */
		Abstract(bool value);
		/** Constructor. Create a new double value.
		 * @param value A double value.
		 */
		Abstract(float value);

		Abstract(const Abstract& value);

		~Abstract();
		/** Get the type of this value. 
		 * @return The type of this value.
		 */
		TYPE	get_type() const;
		/** Get this value as a string. Will perform conversion if necessary. 
		 * @return This value as a string.
		 */
		std::shared_ptr<char>	get_string() const;
		/** Get this value as an integer. Will perform conversion if necessary. 
		 * @return This value as an integer. 
		 */
		int			get_int() const;
		/** Get this value as a boolean. Will perform conversion if necessary. 
		 * @return This value as a boolean. 
		 */
		bool		get_bool() const;
		/** Get this value as a double. Will perform conversion if necessary. 
		 * @return This value as a double. 
		 */
		float		get_float() const;
		/** Relace the current value with a new one. Will changes this value's type to VALUE_TYPE_STRING.
		 * @param value A string.
		 */
		void	set(const char* value);
		/** Relace the current value with a new one. Will changes this value's type to VALUE_TYPE_INT.
		 * @param value An integer value.
		 */
		void	set(int value);
		/** Relace the current value with a new one. Will changes this value's type to VALUE_TYPE_BOOL.
		 * @param value A boolean value.
		 */
		void	set(bool value);
		/** Relace the current value with a new one. Will changes this value's type to VALUE_TYPE_FLOAT.
		 * @param value A double value.
		 */
		void	set(float value);

		void	set(const Abstract& value);

		void clear();

		Abstract&	operator=(const char* value) { set(value); return *this; }
		Abstract&	operator=(int value) { set(value); return *this; }
		Abstract&	operator=(bool value) { set(value); return *this; }
		Abstract&	operator=(float value) { set(value); return *this; }
		Abstract&	operator=(const Abstract& value){set(value); return *this;}

		/** Overloaded comparison operator. Comparisons are based on the type of this. */
		bool	operator==(const Abstract& other) const;
		/** Overloaded comparison operator. Comparisons are based on the type of this. */
		bool	operator!=(const Abstract& other) const { return !(*this == other); }
		/** Overloaded comparison operator. Comparisons are based on the type of this. */
		bool	operator>(const Abstract& other) const;
		/** Overloaded comparison operator. Comparisons are based on the type of this. */
		bool	operator<(const Abstract& other) const { return !(*this == other || *this > other); }
		/** Overloaded comparison operator. Comparisons are based on the type of this. */
		bool	operator>=(const Abstract& other) const { return (*this == other || *this > other); }
		/** Overloaded comparison operator. Comparisons are based on the type of this. */
		bool	operator<=(const Abstract& other) const { return (*this == other || *this < other); }

		operator int(){return get_int();}
		operator int*(){return (int*)Blob;}
		//inline operator float*();
	
	};

	/*
	* Template delete[] method. This comes in handy for setting a custom delete
	* function for std::shared_ptr
	*/
	template <class T> inline void del(T* i)
	{
		delete[] i;
	}
}