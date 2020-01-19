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


#include "fbo.h"
#include <iostream> //Debug

namespace EZCOGL
{
std::array<GLint,4> FBO::initial_viewport_ = {0,0,0,0};

FBO::FBO(const std::vector<std::shared_ptr<Texture2D>>& textures)
{
	glGenFramebuffers(1, &id_);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, id_);
	GLenum att = GL_COLOR_ATTACHMENT0;
	for (auto t: textures)
	{
		glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, att, GL_TEXTURE_2D, t->id(), 0);
		tex_.push_back(t);
		attach_.push_back(att++);
	}
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
}

FBO::~FBO()
{
	glDeleteFramebuffers(1,&id_);
}


void FBO::bind()
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, id_);
	glViewport(0,0,tex_[0]->width(),tex_[0]->height());
	glDrawBuffers(GLsizei(attach_.size()),attach_.data());
}

void FBO::unbind()
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	glViewport(initial_viewport_[0],initial_viewport_[1],initial_viewport_[2],initial_viewport_[3]);
	GLenum db = GL_BACK;
	glDrawBuffers(1u, &db);
}


void FBO::resize(int w, int h)
{
	for (auto t: tex_)
		t->resize(w,h);
}



FBO_Depth::FBO_Depth(const std::vector<std::shared_ptr<Texture2D>>& textures, std::shared_ptr<FBO_Depth> from):
	FBO(textures)
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, id_);
	if (from != nullptr)
	{
		depth_render_buffer_ = from->depth_render_buffer_;
		glBindRenderbuffer( GL_RENDERBUFFER, depth_render_buffer_ );
		glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depth_render_buffer_);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
	}
	else
	{
		glGenRenderbuffers(1,&depth_render_buffer_);
		glBindRenderbuffer( GL_RENDERBUFFER, depth_render_buffer_ );
		glRenderbufferStorage( GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, tex_[0]->width(), tex_[0]->height());
		glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depth_render_buffer_);
		glBindRenderbuffer( GL_RENDERBUFFER, 0);
	}
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
}


void FBO_Depth::resize(int w, int h)
{
	FBO::resize(w,h);
	glBindRenderbuffer(GL_RENDERBUFFER, depth_render_buffer_ );
	glRenderbufferStorage( GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, w, h );
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
}



FBO_DepthTexture::FBO_DepthTexture(const std::vector<std::shared_ptr<Texture2D>>& textures, std::shared_ptr<Texture2D> dt):
	FBO(textures)
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, id_);
	if (dt != nullptr)
	{
		depth_tex_ = dt;
	}
	else
	{
		depth_tex_ = Texture2D::create({GL_NEAREST});
		depth_tex_->init(GL_DEPTH_COMPONENT32F);
	}
	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depth_tex_->id(), 0);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
}

FBO_DepthTexture::FBO_DepthTexture(const std::vector<std::shared_ptr<Texture2D>>& textures, std::shared_ptr<FBO_DepthTexture> from):
	FBO(textures)
{
	if (from != nullptr)
	{
		depth_tex_ = from->depth_tex_;
	}
	else
	{
		depth_tex_ = Texture2D::create({GL_NEAREST});
		depth_tex_->init(GL_DEPTH_COMPONENT32F);
	}
	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depth_tex_->id(), 0);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
}



void FBO_DepthTexture::resize(int w, int h)
{
	FBO::resize(w,h);
	depth_tex_->resize(w,h);
}

}

