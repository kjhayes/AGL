//Copyright (C) - Kevin Hayes - 2022 - All Rights Reserved

#ifndef AGL_HPP
#define AGL_HPP

#if defined(__glad_h_) || defined(__gl_h_)
#error Do not include GL or GLAD, AGL already provides them internally!
#endif

#include "agl/opengl.hpp"
#include "agl/objects.hpp"
#include "agl/context_util.hpp"

#endif