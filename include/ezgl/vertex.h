#ifndef _EZGL_VERTEX_H
#define _EZGL_VERTEX_H

#include "public.h"

class vertex
{
public:
    vec3f position;
    vec3f normal;
    vec4f color;
    vec2f texcoord;

    vertex() : position(), normal(), color(), texcoord() {}

    vertex(const vec3f& pos, const vec2f& st, const vec4f& diff, const vec3f& norm = vec3f()) :
        position(pos), normal(norm), color(diff), texcoord(st) {}
};

#endif // _EZGL_VERTEX_H
