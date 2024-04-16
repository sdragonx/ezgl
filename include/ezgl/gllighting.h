#ifndef _EZGL_LIGHTING_H
#define _EZGL_LIGHTING_H

#include "public.h"

//
// light
//

const int MAX_LIGHTS        = 8;
const int LIGHT_NONE        = 0;
const int LIGHT_DIRECTIONAL = 1;
const int LIGHT_POINT_LIGHT = 2;
const int LIGHT_SPOTLIGHT   = 3;

struct gllight
{
    int state;                  // 0 off
                                // 1 direction light
                                // 2 point light
                                // 3 spotlight
    vec3f position;
    vec3f color;

    vec3f spotDirection;
    float spotExponent;
    float spotCutoff;
    float constantAttenuation;  // Attenuation constant
    float linearAttenuation;    // Linear attenuation coefficient
    float quadraticAttenuation; // Attenuation quadratic coefficient

    gllight() :
        state(LIGHT_NONE),
        position(0.0f),
        color(1.0f),
        spotDirection(0.0f, 0.0f, -1.0f), spotExponent(0.0f), spotCutoff(180.0f),
        constantAttenuation(1.0f), linearAttenuation(0.0f), quadraticAttenuation(0.0f)
    {
    }

    static gllight direction_light(const vec3f& pos, const vec3f& color)
    {
        gllight light;

        light.state = LIGHT_DIRECTIONAL;
        light.position = pos;
        light.color = color;

        return light;
    }

    static gllight point_light(const vec3f& pos, const vec3f& color,
        float constantAtt = 1.0f, float linearAtt = 0.0f, float quadraticAtt = 0.0f)
    {
        gllight light;

        light.state = LIGHT_POINT_LIGHT;
        light.position = pos;
        light.color = color;
        light.constantAttenuation = constantAtt;
        light.linearAttenuation = linearAtt;
        light.quadraticAttenuation = quadraticAtt;

        return light;
    }

    static gllight spot_light(const vec3f& pos, const vec3f& dir, const vec3f& color,
        float constantAtt = 1.0f, float linearAtt = 0.0f, float quadraticAtt = 0.0f,
        float cutoff = 180.0f, float exponent = 0.0f)
    {
        gllight light;

        light.state = LIGHT_SPOTLIGHT;
        light.position = pos;
        light.color = color;
        light.constantAttenuation = constantAtt;
        light.linearAttenuation = linearAtt;
        light.quadraticAttenuation = quadraticAtt;
        light.spotDirection = dir;
        light.spotCutoff = cutoff;
        light.spotExponent = exponent;

        return light;
    }
};

//
// material
//

struct glmaterial
{
    vec3f ambient;
    vec3f diffuse;
    vec3f emission;
    vec3f specular;
    float shininess;

    std::string name;
    std::string map_Kd;
    glimage tex_Kd;

    glmaterial() :
        ambient(0.2f), diffuse(0.8f), emission(0.0f), specular(0.0f), shininess(0.0f) { }

    glmaterial(float Ka, float Kd, float Ke, float Ks, float Ns) :
        ambient(Ka), diffuse(Kd), emission(Ke), specular(Ks), shininess(Ns) { }

    glmaterial(const vec3f& Ka, const vec3f& Kd, const vec3f& Ke, const vec3f& Ks, float Ns) :
        ambient(Ka), diffuse(Kd), emission(Ke), specular(Ks), shininess(Ns) { }
};

//
// fog
//

const int FOG_NONE      = 0;
const int FOG_EXP       = 1;
const int FOG_EXP2      = 2;
const int FOG_LINEAR    = 3;

struct glfog
{
    int mode;
    vec3f color;
    float density;
    float start;
    float end;
    float scale;        // 1 / (end - start)

    glfog(int mode = FOG_NONE, vec3f color = vec3f(0.0), float density = 1.0f, float start = 0.0f, float end = 1.0f)
    {
        this->mode = mode;
        this->color = color;
        this->density = density;
        this->start = start;
        this->end = end;
        this->scale = 1.0f / (end - start);
    }
};

#endif // _EZGL_LIGHTING_H
