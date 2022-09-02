//Copyright (C) - Kevin Hayes - 2022 - All Rights Reserved

#define AGL_GL_OBJECT_ACCESS
#define AGL_SHADER_ACCESS
#define AGL_PROGRAM_ACCESS

#include<iostream>

#include "agl/objects.hpp"

#define STATIC_DEF(x) thread_local decltype(x) x

namespace agl {

#pragma region buffer

buffer::buffer() {
    glGenBuffers(1, &this->_id);
}
buffer::buffer(buffer&& move) noexcept
    : _id(move._id)
{
    move._id = 0;
}
buffer::~buffer() {
    if(this->_id != 0) {
        for(auto& val : _bindings) {
            if(val.second == _id) {
                val.second = 0;
            }
        }
        glDeleteBuffers(1, &this->_id);
    }
}
void buffer::bind(GLenum target) {
    _bindings.insert_or_assign(target, this->_id);
    glBindBuffer(target, this->_id);
}
GLuint buffer::id() {
    return this->_id;
}

STATIC_DEF(buffer::_bindings);

#pragma endregion

//single_binding_buffer is defined in header due to template definitions

#pragma region any_shader 

any_shader::any_shader() {}
any_shader::~any_shader() {
    if(this->_id != 0) {
        glDeleteShader(this->_id);
    }
}
GLuint any_shader::id() {
    return this->_id;
}
void any_shader::compile(std::string_view source) {
    const GLchar* data = source.data();
    GLsizei length = source.length();
    glShaderSource(this->_id, 1, &data, &length);
    glCompileShader(this->_id);
}
bool any_shader::compile_success() const  {
    GLint success;
    glGetShaderiv(this->_id, GL_COMPILE_STATUS, &success);
    return success != GL_FALSE;
}
std::string any_shader::info_log() const {
    constexpr size_t BUF_SIZE = 1024;
    GLchar buffer[BUF_SIZE];
    GLsizei actual_size;
    glGetShaderInfoLog(this->_id, BUF_SIZE, &actual_size, buffer);
    return std::string(buffer, actual_size); 
}

#pragma endregion 

//shaders are defined in header due to template definitions

#pragma region program

program::program() {
    this->_id = glCreateProgram();
}
program::program(program&& move) noexcept
    : _id(move._id)
{
    move._id = 0;
}
program::~program() {
    if(this->_id != 0) {
        if(_bound_id == this->_id) {
            _bound_id = 0;
        }
        glDeleteProgram(this->_id);
    }
}
void program::bind() {
    if(_bound_id != this->_id) {
        glUseProgram(this->_id);
        _bound_id = this->_id;
    }
}
GLuint program::id() {
    return _id;
}

void program::attach_shader(any_shader& shader) {
    glAttachShader(this->_id, shader.id());
}
void program::link() {
    glLinkProgram(this->_id);
}
bool program::link_success() const {
    GLint success;
    glGetProgramiv(this->_id, GL_LINK_STATUS, &success);
    return success != GL_FALSE;
}
std::string program::info_log() const {
    constexpr size_t BUF_SIZE = 1024;
    GLchar buffer[BUF_SIZE];
    GLsizei length;
    glGetProgramInfoLog(this->_id, BUF_SIZE, &length, buffer);
    return std::string(buffer, length);
}
GLint program::uniform_location(const char* name) {
    GLint location = glGetUniformLocation(this->_id, name);
    #ifndef NDEBUG
    if(location < 0) {
        std::cerr << "Error Uniform: \""<<name<<"\" Not Found!" << std::endl;
    }
    #endif
    return location;
}

STATIC_DEF(program::_bound_id)(0);

#pragma region uniforms

//Vectors
void program::bound::set_uniform(GLint loc, float const val) {
    glUniform1f(loc, val);
}
void program::bound::set_uniform(GLint loc, glm::fvec2 const& val) {
    glUniform2f(loc, val.x, val.y);
}
void program::bound::set_uniform(GLint loc, glm::fvec3 const& val) {
    glUniform3f(loc, val.x, val.y, val.z);
}
void program::bound::set_uniform(GLint loc, glm::fvec4 const& val) {
    glUniform4f(loc, val.x, val.y, val.z, val.z);
}
void program::bound::set_uniform(GLint loc, int const val) {
    glUniform1i(loc, val);
}
void program::bound::set_uniform(GLint loc, glm::ivec2 const& val) {
    glUniform2i(loc, val.x, val.y);
}
void program::bound::set_uniform(GLint loc, glm::ivec3 const& val) {
    glUniform3i(loc, val.x, val.y, val.z);
}
void program::bound::set_uniform(GLint loc, glm::ivec4 const& val) {
    glUniform4i(loc, val.x, val.y, val.z, val.w);
}
void program::bound::set_uniform(GLint loc, unsigned int const val) {
    glUniform1ui(loc, val);
}
void program::bound::set_uniform(GLint loc, glm::uvec2 const& val) {
    glUniform2ui(loc, val.x, val.y);
}
void program::bound::set_uniform(GLint loc, glm::uvec3 const& val) {
    glUniform3ui(loc, val.x, val.y, val.z);
}
void program::bound::set_uniform(GLint loc, glm::uvec4 const& val) {
    glUniform4ui(loc, val.x, val.y, val.z, val.w);
}
//Vector Arrays
void program::bound::set_uniform(GLint loc, GLsizei count, float const* array) {
    glUniform1fv(loc, count, array);
}
void program::bound::set_uniform(GLint loc, GLsizei count, glm::fvec2 const* array) {
    glUniform2fv(loc, count, (float*)array);
}
void program::bound::set_uniform(GLint loc, GLsizei count, glm::fvec3 const* array) {
    glUniform3fv(loc, count, (float*)array);
}
void program::bound::set_uniform(GLint loc, GLsizei count, glm::fvec4 const* array) {
    glUniform4fv(loc, count, (float*)array);
}
void program::bound::set_uniform(GLint loc, GLsizei count, int const* array) {
    glUniform1iv(loc, count, array);
}
void program::bound::set_uniform(GLint loc, GLsizei count, glm::ivec2 const* array) {
    glUniform2iv(loc, count, (int*)array);
}
void program::bound::set_uniform(GLint loc, GLsizei count, glm::ivec3 const* array) {
    glUniform3iv(loc, count, (int*)array);
}
void program::bound::set_uniform(GLint loc, GLsizei count, glm::ivec4 const* array) {
    glUniform4iv(loc, count, (int*)array);
}
void program::bound::set_uniform(GLint loc, GLsizei count, unsigned int const* array) {
    glUniform1uiv(loc, count, array);
}
void program::bound::set_uniform(GLint loc, GLsizei count, glm::uvec2 const* array) {
    glUniform2uiv(loc, count, (unsigned int*)array);
}
void program::bound::set_uniform(GLint loc, GLsizei count, glm::uvec3 const* array) {
    glUniform3uiv(loc, count, (unsigned int*)array);
}
void program::bound::set_uniform(GLint loc, GLsizei count, glm::uvec4 const* array) {
    glUniform4uiv(loc, count, (unsigned int*)array);
}
//Matrices
void program::bound::set_uniform(GLint loc, glm::mat2x2 const& val, bool transpose) {
    glUniformMatrix2fv(loc, 1, transpose, (float*)&val);
}
void program::bound::set_uniform(GLint loc, glm::mat3x3 const& val, bool transpose) {
    glUniformMatrix3fv(loc, 1, transpose, (float*)&val);
}
void program::bound::set_uniform(GLint loc, glm::mat4x4 const& val, bool transpose) {
    glUniformMatrix4fv(loc, 1, transpose, (float*)&val);
}
void program::bound::set_uniform(GLint loc, glm::mat2x3 const& val, bool transpose) {
    glUniformMatrix2x3fv(loc, 1, transpose, (float*)&val);
}
void program::bound::set_uniform(GLint loc, glm::mat3x2 const& val, bool transpose) {
    glUniformMatrix3x2fv(loc, 1, transpose, (float*)&val);
}
void program::bound::set_uniform(GLint loc, glm::mat2x4 const& val, bool transpose) {
    glUniformMatrix2x4fv(loc, 1, transpose, (float*)&val);
}
void program::bound::set_uniform(GLint loc, glm::mat4x2 const& val, bool transpose) {
    glUniformMatrix4x2fv(loc, 1, transpose, (float*)&val);
}
void program::bound::set_uniform(GLint loc, glm::mat3x4 const& val, bool transpose) {
    glUniformMatrix3x4fv(loc, 1, transpose, (float*)&val);
}
void program::bound::set_uniform(GLint loc, glm::mat4x3 const& val, bool transpose) {
    glUniformMatrix4x3fv(loc, 1, transpose, (float*)&val);
}
//Matrix Arrays
void program::bound::set_uniform(GLint loc, GLsizei count, glm::mat2x2 const* array, bool transpose) {
    glUniformMatrix2fv(loc, count, transpose, (float*)array);
} 
void program::bound::set_uniform(GLint loc, GLsizei count, glm::mat3x3 const* array, bool transpose) {
    glUniformMatrix3fv(loc, count, transpose, (float*)array);
} 
void program::bound::set_uniform(GLint loc, GLsizei count, glm::mat4x4 const* array, bool transpose) {
    glUniformMatrix4fv(loc, count, transpose, (float*)array);
} 
void program::bound::set_uniform(GLint loc, GLsizei count, glm::mat2x3 const* array, bool transpose) {
    glUniformMatrix2x3fv(loc, count, transpose, (float*)array);
} 
void program::bound::set_uniform(GLint loc, GLsizei count, glm::mat3x2 const* array, bool transpose) {
    glUniformMatrix3x2fv(loc, count, transpose, (float*)array);
} 
void program::bound::set_uniform(GLint loc, GLsizei count, glm::mat2x4 const* array, bool transpose) {
    glUniformMatrix2x4fv(loc, count, transpose, (float*)array);
} 
void program::bound::set_uniform(GLint loc, GLsizei count, glm::mat4x2 const* array, bool transpose) {
    glUniformMatrix4x2fv(loc, count, transpose, (float*)array);
} 
void program::bound::set_uniform(GLint loc, GLsizei count, glm::mat3x4 const* array, bool transpose) {
    glUniformMatrix3x4fv(loc, count, transpose, (float*)array);
} 
void program::bound::set_uniform(GLint loc, GLsizei count, glm::mat4x3 const* array, bool transpose) {
    glUniformMatrix4x3fv(loc, count, transpose, (float*)array);
} 

#pragma endregion

#pragma endregion

#pragma region vertex_array 

vertex_array::vertex_array() {
    glGenVertexArrays(1, &this->_id);
}
vertex_array::vertex_array(vertex_array&& move) noexcept
    : _id(move._id)
{
    move._id = 0;
}
vertex_array::~vertex_array() {
    if(this->_id != 0) {
        if(_bound_id == this->_id) {
            _bound_id = 0;
        }
        glDeleteVertexArrays(1, &this->_id);
    }
}
void vertex_array::bind() {
    if(_bound_id != this->_id) {
        glBindVertexArray(this->_id);
        _bound_id = this->_id;
    }
}
GLuint vertex_array::id() {
    return this->_id;
}

STATIC_DEF(vertex_array::_bound_id)(0);

#pragma endregion

#pragma region query 

query::query() {
    glGenQueries(1, &this->_id);
}
query::query(query&& move) noexcept
    : _id(move._id)
{
    move._id = 0;
}
query::~query() {
    if(this->_id != 0) {
        glDeleteQueries(1, &this->_id);
    }
}
GLuint query::id() {
    return this->_id;
}

#pragma endregion 

#pragma region program_pipeline 

program_pipeline::program_pipeline() {
    glGenProgramPipelines(1, &this->_id);
}
program_pipeline::program_pipeline(program_pipeline&& move) noexcept
    : _id(move._id)
{
    move._id = 0;
}
program_pipeline::~program_pipeline() {
    if(this->_id != 0) {
        if(_bound_id == this->_id) {
            _bound_id = 0;
        }
        glDeleteProgramPipelines(1, &this->_id);
    }
}
void program_pipeline::bind() {
    if(_bound_id != this->_id) {
        glBindProgramPipeline(this->_id);
        _bound_id = this->_id;
    }
}
GLuint program_pipeline::id() {
    return this->_id;
}

STATIC_DEF(program_pipeline::_bound_id)(0);

#pragma endregion

#pragma region transform_feedback

transform_feedback::transform_feedback() {
    glGenTransformFeedbacks(1, &this->_id);
}
transform_feedback::transform_feedback(transform_feedback&& move) noexcept
    : _id(move._id)
{
    move._id = 0;
}
transform_feedback::~transform_feedback() {
    if(this->_id != 0) {
        if(_bound_id == this->_id) {
            _bound_id = 0;
        }
        glDeleteTransformFeedbacks(1, &this->_id);
    }
}
void transform_feedback::bind() {
    if(_bound_id != this->_id) {
        glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, this->_id);
        _bound_id = this->_id;
    }
}
GLuint transform_feedback::id() {
    return this->_id;
}

STATIC_DEF(transform_feedback::_bound_id)(0);

#pragma endregion

#pragma region sampler

sampler::sampler() {
    glGenSamplers(1, &this->_id);
}
sampler::sampler(sampler&& move) noexcept
    : _id(move._id)
{
    move._id = 0;
}
sampler::~sampler() {
    if(this->_id != 0) {
        for(size_t index = 0; index < GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS; index++) {
            if(_bindings[index] == this->_id) {
                _bindings[index] = 0;
            }
        }
        glDeleteSamplers(1, &this->_id);
    }
}
void sampler::bind(GLuint unit) {
    if(_bindings[unit] != this->_id) {
        glBindSampler(unit, this->_id);
        _bindings[unit] = this->_id;
    }
}
GLuint sampler::id() {
    return this->_id;
}

STATIC_DEF(sampler::_bindings){0};

#pragma endregion

//textures are defined in header due to template definitions

#pragma region renderbuffer

renderbuffer::renderbuffer() {
    glGenRenderbuffers(1, &this->_id);
}
renderbuffer::renderbuffer(renderbuffer&& move) noexcept
    : _id(move._id)
{
    move._id = 0;
}
renderbuffer::~renderbuffer() {
    if(this->_id != 0) {
        if(_bound_id == this->_id) {
            _bound_id = 0;
        }
        glDeleteRenderbuffers(1, &this->_id);
    }
}
void renderbuffer::bind() {
    if(_bound_id != this->_id) {
        glBindRenderbuffer(GL_RENDERBUFFER, this->_id);
        _bound_id = this->_id;
    }
}
GLuint renderbuffer::id() {
    return this->_id;
}

STATIC_DEF(renderbuffer::_bound_id)(0);

#pragma endregion

#pragma region framebuffer

framebuffer::framebuffer() {
    glGenFramebuffers(1, &this->_id);
}
framebuffer::framebuffer(framebuffer&& move) noexcept
    : _id(move._id)
{
    move._id = 0;
}
framebuffer::~framebuffer() {
    if(this->_id != 0) {
        if(_read_bound_id == this->_id) {
            _read_bound_id = 0;
        }
        if(_draw_bound_id == this->_id) {
            _draw_bound_id = 0;
        }
        glDeleteFramebuffers(1, &this->_id);
    }
}
void framebuffer::bind() {
    if(_read_bound_id != this->_id || _draw_bound_id != this->_id) {
        glBindFramebuffer(GL_FRAMEBUFFER, this->_id);
        _read_bound_id = this->_id;
        _draw_bound_id = this->_id;
    }
}
void framebuffer::bind_read() {
    if(_read_bound_id != this->_id) {
        glBindFramebuffer(GL_READ_FRAMEBUFFER, this->_id);
        _read_bound_id = this->_id;
    }
}
void framebuffer::bind_write() {
    if(_draw_bound_id != this->_id) {
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, this->_id);
        _draw_bound_id = this->_id;
    }
}
GLuint framebuffer::id() {
    return this->_id;
}

framebuffer::framebuffer(GLuint id) : _id(id) {}

STATIC_DEF(framebuffer::DEFAULT)(0);
STATIC_DEF(framebuffer::_read_bound_id)(0);
STATIC_DEF(framebuffer::_draw_bound_id)(0);

#pragma endregion

}