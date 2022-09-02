#ifndef AGL_CONTEXT_UTIL_HPP
#define AGL_CONTEXT_UTIL_HPP

namespace agl {

using load_proc = void*(*)(const char*);

//true = success
bool init();
//true = success
bool init(load_proc);

void set_gl_debug_logging(bool);

}

#endif