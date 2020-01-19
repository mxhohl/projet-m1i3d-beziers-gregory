/*******************************************************************************
* EasyCppOGL:   Copyright (C) 2019,                                            *
* Sylvain Thery, IGG Group, ICube, University of Strasbourg, France            *
*                                                                              *
* This library is free software; you can redistribute it and/or modify it      *
* under the terms of the GNU Lesser General Public License as published by the *
* Free Software Foundation; either version 2.1 of the License, or (at your     *
* option) any later version.                                                   *
*                                                                              *
* This library is distributed in the hope that it will be useful, but WITHOUT  *
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or        *
* FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License  *
* for more details.                                                            *
*                                                                              *
* You should have received a copy of the GNU Lesser General Public License     *
* along with this library; if not, write to the Free Software Foundation,      *
* Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301 USA.           *
*                                                                              *
* Contact information: thery@unistra.fr                                        *
*******************************************************************************/


#ifndef EASY_CPP_OGL_SHADERS_SHADERPROGRAM_H_
#define EASY_CPP_OGL_SHADERS_SHADERPROGRAM_H_

#include<string>
#include <iostream>
#include <cassert>
#include <memory>
#include <array>
#include <map>

#include "gl_eigen.h"
#include "vao.h"



namespace EZCOGL
{

std::string load(const std::string& file);


class Shader
{
protected:
	GLuint id_;

public:
	Shader() = delete;
	Shader(const Shader&) = delete;
	Shader& operator=(const Shader&) = delete;

	inline Shader(GLenum type)
	{
		id_ = glCreateShader(type);
	}

	inline ~Shader()
	{
		glDeleteShader(id_);
	}

	inline GLuint shaderId() const
	{
		return id_;
	}

	void compile(const std::string& src, const std::string& name);
};


class ShaderProgram;
using SP_ShaderProgram = std::shared_ptr<ShaderProgram>;

class ShaderProgram
{
protected:
	std::string name_;
	GLuint id_;
	std::vector<Shader*> shaders_;
	std::map<std::string,int> ulocations;
	std::vector<int> utranslat;

public:
	static ShaderProgram* current_binded_;

	ShaderProgram(const std::vector<std::pair<GLenum,const std::string&>> sources,  const std::string& name, const std::vector<char*> tf_outs = { });

	ShaderProgram(const ShaderProgram&) = delete;
	ShaderProgram& operator=(const ShaderProgram&) = delete;
	virtual ~ShaderProgram();

	void location_analyser(std::string& src);

	static SP_ShaderProgram create(const std::vector<std::pair<GLenum,const std::string&>> sources,  const std::string& name);

	inline GLuint id() const		{ return id_; }
	inline static void unbind()		{ glUseProgram(0); }
	inline void bind()				{ glUseProgram(id_); current_binded_ = this;}
	inline GLint uniform_location(const GLchar* str) const
	{
		return glGetUniformLocation(this->id_,str);
	}

	inline void set_uniform_val(GLint unif, const bool v) { glUniform1i(utranslat[unif],int32_t(v));}
	inline void set_uniform_val(GLint unif, const float v) { glUniform1f(utranslat[unif],v);}
	inline void set_uniform_val(GLint unif, const double v) { glUniform1f(utranslat[unif],float(v));}
	inline void set_uniform_val(GLint unif, const std::array<float,2>& v) { glUniform2fv(utranslat[unif],1,v.data());}
	inline void set_uniform_val(GLint unif, const std::array<float,3>& v) { glUniform3fv(utranslat[unif],1,v.data());}
	inline void set_uniform_val(GLint unif, const std::array<float,4>& v) { glUniform4fv(utranslat[unif],1,v.data());}
	inline void set_uniform_val(GLint unif, const int32_t v) { glUniform1i(utranslat[unif],v);}
	inline void set_uniform_val(GLint unif, const std::array<int32_t,2>& v) { glUniform2iv(utranslat[unif],1,v.data());}
	inline void set_uniform_val(GLint unif, const std::array<int32_t,3>& v) { glUniform3iv(utranslat[unif],1,v.data());}
	inline void set_uniform_val(GLint unif, const std::array<int32_t,4>& v) { glUniform4iv(utranslat[unif],1,v.data());}
	inline void set_uniform_val(GLint unif, const uint32_t v) { glUniform1ui(utranslat[unif],v);}
	inline void set_uniform_val(GLint unif, const std::array<uint32_t,2>& v) { glUniform2uiv(utranslat[unif],1,v.data());}
	inline void set_uniform_val(GLint unif, const std::array<uint32_t,3>& v) { glUniform3uiv(utranslat[unif],1,v.data());}
	inline void set_uniform_val(GLint unif, const std::array<uint32_t,4>& v) { glUniform4uiv(utranslat[unif],1,v.data());}
	inline void set_uniform_val(GLint unif, const GLVec2& v) { glUniform2fv(utranslat[unif],1,v.data());}
	inline void set_uniform_val(GLint unif, const GLVec3& v) { glUniform3fv(utranslat[unif],1,v.data());}
	inline void set_uniform_val(GLint unif, const GLVec4& v) { glUniform4fv(utranslat[unif],1,v.data());}
	inline void set_uniform_val(GLint unif, const GLMat2& v) { glUniformMatrix2fv(utranslat[unif], 1, false, v.data()); }
	inline void set_uniform_val(GLint unif, const GLMat3& v) { glUniformMatrix3fv(utranslat[unif], 1, false, v.data());}
	inline void set_uniform_val(GLint unif, const GLMat4& v) { glUniformMatrix4fv(utranslat[unif], 1, false, v.data());}

	inline void set_uniform_val(GLint unif, const GLVVec2& v) { glUniform2fv(utranslat[unif],v.size(),v[0].data());}
	inline void set_uniform_val(GLint unif, const GLVVec3& v) { glUniform3fv(utranslat[unif],v.size(),v[0].data());}
	inline void set_uniform_val(GLint unif, const GLVVec4& v) { glUniform4fv(utranslat[unif],v.size(),v[0].data());}

	inline void set_uniform_val(GLint unif, const std::vector<GLMat2>& v) { glUniformMatrix2fv(utranslat[unif], v.size(), false, v[0].data());}
	inline void set_uniform_val(GLint unif, const std::vector<GLMat3>& v) { glUniformMatrix3fv(utranslat[unif], v.size(), false, v[0].data());}
	inline void set_uniform_val(GLint unif, const std::vector<GLMat4>& v) { glUniformMatrix4fv(utranslat[unif], v.size(), false, v[0].data());}

	template <typename T>
	inline auto set_uniform_value(const std::string& uname, const T& v)
	-> typename std::enable_if<!is_eigen<T>::value>::type
	{
		auto uni = glGetUniformLocation(this->id_,uname.c_str());
		if (uni < 0)
			std::cerr << "Warning uniform "<< uname << " not found"<< std::endl;
		else
			set_uniform_val(uni,v);
	}

	template <typename T>
	inline auto set_uniform_value(const std::string& uname, T& v)
	-> typename std::enable_if<is_eigen<T>::value>::type
	{
		auto uni = glGetUniformLocation(this->id_,uname.c_str());
		if (uni < 0)
			std::cerr << "Warning uniform "<< uname << " not found"<< std::endl;
		else
			set_uniform_val(uni,v.eval());
	}


	template <typename T>
	inline auto set_uniform_value(GLint uni, const T& v)
	-> typename std::enable_if<!is_eigen<T>::value>::type
	{
		set_uniform_val(uni,v);
	}

	template <typename T>
	inline auto set_uniform_value(GLint uni, T& v)
	-> typename std::enable_if<is_eigen<T>::value>::type
	{
		set_uniform_val(uni,v.eval());
	}
};

template <typename T>
inline void set_uniform_value(const std::string& uname, const T& v)
{
	ShaderProgram::current_binded_->set_uniform_value(uname,v);
}

template <typename T>
inline void set_uniform_value(GLint uni, const T& v)
{
	ShaderProgram::current_binded_->set_uniform_value(uni,v);
}

} // namespace

#endif // EASY_CPP_OGL_SHADERS_SHADERPROGRAM_H_
