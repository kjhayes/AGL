//Copyright (C) - Kevin Hayes - 2022 - All Rights Reserved

#ifndef AGL_OBJECTS_HPP
#define AGL_OBJECTS_HPP

#include<unordered_map>
#include<string_view>

#include "agl/opengl.hpp"

namespace agl 
{

struct buffer {
public:
    buffer(buffer&) = delete;

    buffer();
    buffer(buffer&&) noexcept;
    ~buffer();

    void bind(GLenum target);
    GLuint id();

private:
    GLuint _id;
    static thread_local std::unordered_map<GLenum, GLuint> _bindings;
};
template<GLenum TARGET>
struct single_binding_buffer : public buffer {

static_assert(
    TARGET == GL_ARRAY_BUFFER ||
    TARGET == GL_ATOMIC_COUNTER_BUFFER ||
    TARGET == GL_COPY_READ_BUFFER ||
    TARGET == GL_COPY_WRITE_BUFFER ||
    TARGET == GL_DISPATCH_INDIRECT_BUFFER ||
    TARGET == GL_DRAW_INDIRECT_BUFFER ||
    TARGET == GL_ELEMENT_ARRAY_BUFFER ||
    TARGET == GL_PIXEL_PACK_BUFFER ||
    TARGET == GL_PIXEL_UNPACK_BUFFER ||
    TARGET == GL_QUERY_BUFFER ||
    TARGET == GL_SHADER_STORAGE_BUFFER ||
    TARGET == GL_TEXTURE_BUFFER ||
    TARGET == GL_TRANSFORM_FEEDBACK_BUFFER ||
    TARGET == GL_UNIFORM_BUFFER,
    "Invalid TARGET when instantiating agl::buffer<TARGET>! "
    "(Instead of instantiating agl::buffer<TARGET> "
    "use an agl typedef-ed shader type such as agl::array_buffer)!"
);

private:
    using buffer::bind;

public:
    void bind() {buffer::bind(TARGET);}
};
using array_buffer = single_binding_buffer<GL_ARRAY_BUFFER>;
using element_array_buffer = single_binding_buffer<GL_ELEMENT_ARRAY_BUFFER>;
//Unsure of use cases of other buffer types, so explicit typedef-ing might be wrong
// Example: GL_COPY_READ_BUFFER should not be singly bound
#ifndef AGL_GL_OBJECT_ACCESS
    #undef glGenBuffers
    #undef glDeleteBuffers
    #undef glBindBuffer
#endif

struct any_shader {
public:
    ~any_shader();
    
    GLuint id();

    void compile(std::string_view source);
    bool compile_success() const;
    std::string info_log() const;

protected:
    any_shader();

    GLuint _id;
};
#ifndef AGL_SHADER_ACCESS
    #undef glShaderSource
    #undef glCompileShader
    #undef glGetShaderInfoLog
#endif

template<GLenum TYPE>
struct shader : public any_shader {

    static_assert(
        TYPE == GL_COMPUTE_SHADER ||
        TYPE == GL_VERTEX_SHADER || 
        TYPE == GL_FRAGMENT_SHADER ||
        TYPE == GL_GEOMETRY_SHADER ||
        TYPE == GL_TESS_CONTROL_SHADER ||
        TYPE == GL_TESS_EVALUATION_SHADER,
        "Invalid TYPE when instantiating agl::shader<TYPE>! "
        "(Instead of instantiating agl::shader<TYPE> "
        "use an agl typedef-ed shader type such as agl::vertex_shader)!"
    );

public:
    shader(shader&) = delete;

    shader() {
        this->_id = glCreateShader(TYPE);
    }
    shader(shader&& move) noexcept
        : _id(move._id)
    {
        move._id = 0;
    }

    constexpr static GLenum type = TYPE;
};
#ifndef AGL_GL_OBJECT_ACCESS
    #undef glCreateShader
    #undef glDeleteShader
#endif

using compute_shader = shader<GL_COMPUTE_SHADER>;
using vertex_shader = shader<GL_VERTEX_SHADER>;
using fragment_shader = shader<GL_FRAGMENT_SHADER>;
using geometry_shader = shader<GL_GEOMETRY_SHADER>;
using tess_control_shader = shader<GL_TESS_CONTROL_SHADER>;
using tess_evaluation_shader = shader<GL_TESS_EVALUATION_SHADER>;

struct program {
public:
    program(program&) = delete;

    program();
    program(program&&) noexcept;
    ~program();

    void bind();
    GLuint id();

    void attach_shader(any_shader&);
    void link();
    bool link_success() const;
    std::string info_log() const;

    GLint uniform_location(const char*);

    struct bound final {
        bound() = delete;

        static void set_uniform(GLint, float const);
        static void set_uniform(GLint, glm::fvec2 const&);
        static void set_uniform(GLint, glm::fvec3 const&);
        static void set_uniform(GLint, glm::fvec4 const&);
        static void set_uniform(GLint, int const);
        static void set_uniform(GLint, glm::ivec2 const&);
        static void set_uniform(GLint, glm::ivec3 const&);
        static void set_uniform(GLint, glm::ivec4 const&);
        static void set_uniform(GLint, unsigned int const);
        static void set_uniform(GLint, glm::uvec2 const&);
        static void set_uniform(GLint, glm::uvec3 const&);
        static void set_uniform(GLint, glm::uvec4 const&);

        static void set_uniform(GLint, GLsizei, float const*);
        static void set_uniform(GLint, GLsizei, glm::fvec2 const*);
        static void set_uniform(GLint, GLsizei, glm::fvec3 const*);
        static void set_uniform(GLint, GLsizei, glm::fvec4 const*);
        static void set_uniform(GLint, GLsizei, int const*);
        static void set_uniform(GLint, GLsizei, glm::ivec2 const*);
        static void set_uniform(GLint, GLsizei, glm::ivec3 const*);
        static void set_uniform(GLint, GLsizei, glm::ivec4 const*);
        static void set_uniform(GLint, GLsizei, unsigned int const*);
        static void set_uniform(GLint, GLsizei, glm::uvec2 const*);
        static void set_uniform(GLint, GLsizei, glm::uvec3 const*);
        static void set_uniform(GLint, GLsizei, glm::uvec4 const*);

        static void set_uniform(GLint, glm::mat2x2 const&, bool transpose = false);
        static void set_uniform(GLint, glm::mat3x3 const&, bool transpose = false);
        static void set_uniform(GLint, glm::mat4x4 const&, bool transpose = false);
        static void set_uniform(GLint, glm::mat2x3 const&, bool transpose = false);
        static void set_uniform(GLint, glm::mat3x2 const&, bool transpose = false);
        static void set_uniform(GLint, glm::mat2x4 const&, bool transpose = false);
        static void set_uniform(GLint, glm::mat4x2 const&, bool transpose = false);
        static void set_uniform(GLint, glm::mat3x4 const&, bool transpose = false);
        static void set_uniform(GLint, glm::mat4x3 const&, bool transpose = false);

        static void set_uniform(GLint, GLsizei, glm::mat2x2 const*, bool transpose = false);
        static void set_uniform(GLint, GLsizei, glm::mat3x3 const*, bool transpose = false);
        static void set_uniform(GLint, GLsizei, glm::mat4x4 const*, bool transpose = false);
        static void set_uniform(GLint, GLsizei, glm::mat2x3 const*, bool transpose = false);
        static void set_uniform(GLint, GLsizei, glm::mat3x2 const*, bool transpose = false);
        static void set_uniform(GLint, GLsizei, glm::mat2x4 const*, bool transpose = false);
        static void set_uniform(GLint, GLsizei, glm::mat4x2 const*, bool transpose = false);
        static void set_uniform(GLint, GLsizei, glm::mat3x4 const*, bool transpose = false);
        static void set_uniform(GLint, GLsizei, glm::mat4x3 const*, bool transpose = false);
    };
    
private:
    GLuint _id;
    static thread_local GLuint _bound_id;
};
#ifndef AGL_GL_OBJECT_ACCESS
    #undef glCreateProgram
    #undef glDeleteProgram
    #undef glUseProgram
#endif
#ifndef AGL_PROGRAM_ACCESS
    #undef glLinkProgram
    #undef glAttachShader
    #undef glGetProgramInfoLog
    #undef glGetUniformLocation
#endif

struct vertex_array {
public:
    vertex_array(vertex_array&) = delete;

    vertex_array();
    vertex_array(vertex_array&&) noexcept;
    ~vertex_array();

    void bind();
    GLuint id();

private:
    GLuint _id;
    static thread_local GLuint _bound_id;
};
#ifndef AGL_GL_OBJECT_ACCESS
    #undef glGenVertexArrays
    #undef glDeleteVertexArrays
    #undef glBindVertexArray
#endif

struct query {
public:
    query(query&) = delete;

    query();
    query(query&&) noexcept;
    ~query();

    GLuint id();

private:
    GLuint _id;
};
#ifndef AGL_GL_OBJECT_ACCESS
    #undef glGenQueries
    #undef glDeleteQueries
#endif

struct program_pipeline {
public:
    program_pipeline(program_pipeline&) = delete;

    program_pipeline();
    program_pipeline(program_pipeline&&) noexcept;
    ~program_pipeline();

    void bind();
    GLuint id();

private:
    GLuint _id;
    static thread_local GLuint _bound_id;
};
#ifndef AGL_GL_OBJECT_ACCESS
    #undef glGenProgramPipelines
    #undef glDeleteProgramPipelines
    #undef glBindProgramPipeline
#endif

struct transform_feedback {
public:
    transform_feedback(transform_feedback&) = delete;

    transform_feedback();
    transform_feedback(transform_feedback&&) noexcept;
    ~transform_feedback();

    void bind();
    GLuint id();

private:
    GLuint _id;
    static thread_local GLuint _bound_id;
};
#ifndef AGL_GL_OBJECT_ACCESS
    #undef glGenTransformFeedbacks
    #undef glDeleteTransformFeedbacks
    #undef glBindTransformFeedback
#endif

struct sampler {
public:
    sampler(sampler&) = delete;

    sampler();
    sampler(sampler&&) noexcept;
    ~sampler();

    void bind(GLuint unit);
    GLuint id();

private:
    GLuint _id;
    static thread_local GLuint _bindings[GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS];
};
#ifndef AGL_GL_OBJECT_ACCESS
    #undef glGenSamplers
    #undef glDeleteSamplers
    #undef glBindSampler
#endif

template<GLenum TARGET>
struct texture {

    static_assert(
        TARGET == GL_TEXTURE_1D ||
        TARGET == GL_TEXTURE_2D ||
        TARGET == GL_TEXTURE_3D ||
        TARGET == GL_TEXTURE_1D_ARRAY ||
        TARGET == GL_TEXTURE_2D_ARRAY ||
        TARGET == GL_TEXTURE_RECTANGLE ||
        TARGET == GL_TEXTURE_CUBE_MAP ||
        TARGET == GL_TEXTURE_CUBE_MAP_ARRAY ||
        TARGET == GL_TEXTURE_BUFFER ||
        TARGET == GL_TEXTURE_2D_MULTISAMPLE ||
        TARGET == GL_TEXTURE_2D_MULTISAMPLE_ARRAY,
        "Invalid TARGET when instantiating agl::texture<TARGET>! "
        "(Instead of instantiating texture<TARGET> "
        "use an agl typedef-ed texture type such as agl::texture_2d)!"
    );

public:
    texture(texture&) = delete;

    texture() {
        glGenTextures(1, &this->_id);
    }
    texture(texture&& move) noexcept
        : _id(move._id)
    {
        move._id = 0;
    }
    ~texture() {
        if(this->_id != 0) {
            if(_bound_id == this->_id) {
                _bound_id = 0;
            }
            glDeleteTextures(1, &this->_id);
        }
    }

    void bind() {
        if(_bound_id != this->_id) {
            glBindTexture(TARGET, this->_id);
            _bound_id = this->_id;
        }
    }
    GLuint id() {
        return this->_id;
    }

    constexpr static GLenum target = TARGET;

private:
    GLuint _id;
    static thread_local GLuint _bound_id;
};
#ifndef AGL_GL_OBJECT_ACCESS
    #undef glGenTextures
    #undef glDeleteTextures
    #undef glBindTexture
#endif

using texture_1d = texture<GL_TEXTURE_1D>; 
using texture_2d = texture<GL_TEXTURE_2D>;
using texture_3d = texture<GL_TEXTURE_3D>;
using array_texture_1d = texture<GL_TEXTURE_1D_ARRAY>; 
using array_texture_2d = texture<GL_TEXTURE_2D_ARRAY>; 
using rectangle_texture = texture<GL_TEXTURE_RECTANGLE>;
using cube_map_texture = texture<GL_TEXTURE_CUBE_MAP>;
using cube_map_array_texture = texture<GL_TEXTURE_CUBE_MAP_ARRAY>;
using buffer_texture = texture<GL_TEXTURE_BUFFER>;
using multisample_texture_2d = texture<GL_TEXTURE_2D_MULTISAMPLE>; 
using multisample_array_texture_2d = texture<GL_TEXTURE_2D_MULTISAMPLE_ARRAY>;

struct renderbuffer {
public:
    renderbuffer(renderbuffer&) = delete;

    renderbuffer();
    renderbuffer(renderbuffer&&) noexcept;
    ~renderbuffer();

    void bind();
    GLuint id();

private:
    GLuint _id;
    static thread_local GLuint _bound_id;
};
#ifndef AGL_GL_OBJECT_ACCESS
    #undef glGenRenderbuffers
    #undef glDeleteRenderbuffers
    #undef glBindRenderbuffer
#endif

struct framebuffer {
public:
    framebuffer(framebuffer&) = delete;

    framebuffer();
    framebuffer(framebuffer&&) noexcept;
    ~framebuffer();
    

    void bind();
    void bind_read();
    void bind_write();
    GLuint id();

    static thread_local framebuffer DEFAULT;

private:
    GLuint _id;

    framebuffer(GLuint id);

    static thread_local GLuint _read_bound_id;
    static thread_local GLuint _draw_bound_id;
};
#ifndef AGL_GL_OBJECT_ACCESS
    #undef glGenFramebuffers
    #undef glDeleteFramebuffers
    #undef glBindFramebuffer
#endif

}

#endif //AGL_OBJECTS_HPP