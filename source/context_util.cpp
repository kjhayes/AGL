#include "agl/context_util.hpp"

#include<iostream>
#include<format>

#include "glad/glad.h"

namespace agl {

bool init() {
    int res = gladLoadGL();
    return res != 0;
}
bool init(load_proc proc) {
    int res = gladLoadGLLoader(proc);
    return res != 0;
}

//Credit for this function's core design to: https://www.khronos.org/opengl/wiki/Debug_Output
void GLAPIENTRY gl_debug_logging_callback( 
                GLenum source,
                GLenum type,
                GLuint id,
                GLenum severity,
                GLsizei length,
                const GLchar* message,
                const void* userParam )
{
    std::cerr << std::format("GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
        ( type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : "" ),
        type, severity, message ) << std::endl;
}
//

void set_gl_debug_logging(bool enable) {
    if(enable) {
        glEnable(GL_DEBUG_OUTPUT);
        glDebugMessageCallback(gl_debug_logging_callback, 0);
    } else {
        glDisable(GL_DEBUG_OUTPUT);
    }
}

}