#ifndef _EZGL_PUBLIC_H
#define _EZGL_PUBLIC_H

#if defined(_MSC_VER)
#pragma execution_character_set("utf-8")

#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif
#endif

// std
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iostream>
#include <sstream>
#include <map>
#include <stdint.h>
#include <string>
#include <vector>

// glew
#define GLEW_STATIC
#include <glew/glew.h>

// glfw
#define GLFW_INCLUDE_GLU
#include <GLFW/glfw3.h>

// glm
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/io.hpp>
#include <glm/gtx/normal.hpp>
#include <glm/gtx/rotate_vector.hpp>

// imgui
#define ImTextureID ImU64

#define IM_VEC2_CLASS_EXTRA                                                 \
        ImVec2(const glm::vec2& f) { x = f.x; y = f.y; }                    \
        operator glm::vec2() const { return glm::vec2(x,y); }

#define IM_VEC4_CLASS_EXTRA                                                 \
        ImVec4(const glm::vec4& f) { x = f.x; y = f.y; z = f.z; w = f.w; }  \
        operator glm::vec4() const { return glm::vec4(x,y,z,w); }

#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_opengl2.h>

#if defined(__BORLANDC__) || defined(_MSC_VER)

    #pragma comment(lib, "opengl32.lib")
    #pragma comment(lib, "glu32.lib")

    #if defined(_M_AMD64) || defined(_M_X64)
        #pragma comment(lib, "x64/glew32s.lib")
        #pragma comment(lib, "x64/glfw3.lib")
        #pragma comment(lib, "x64/imgui.lib")
    #else
        #pragma comment(lib, "x86/glew32s.lib")
        #pragma comment(lib, "x86/glfw3.lib")
        #pragma comment(lib, "x86/imgui.lib")
    #endif
#endif

#define countof(a) (sizeof(a) / sizeof(a[0]))
#define offset_element(obj,element) ((void*)(offsetof(obj, element)))

#ifndef __stringify
#define __stringify_1(x) x
#define __stringify(x) __stringify_1(#x)
#endif

#ifndef M_PI
#define M_PI       3.14159265358979323846   // pi
#endif

#ifndef M_RD
#define M_RD (M_PI / 180.0)
#define M_INV_RD (1.0 / M_RD)
#endif

typedef uint8_t byte_t;

typedef glm::u8vec3 vec3ub;
typedef glm::u8vec4 vec4ub;
typedef glm::ivec2 vec2i;
typedef glm::ivec3 vec3i;
typedef glm::ivec4 vec4i;
typedef glm::vec2 vec2f;
typedef glm::vec3 vec3f;
typedef glm::vec4 vec4f;
typedef glm::dvec2 vec2d;
typedef glm::dvec3 vec3d;
typedef glm::dvec4 vec4d;
typedef glm::mat3 mat3f;
typedef glm::mat4 mat4f;

//
// constants
//

// axes
const vec3f axis_x = vec3f(1.0f, 0.0f, 0.0f);
const vec3f axis_y = vec3f(0.0f, 1.0f, 0.0f);
const vec3f axis_z = vec3f(0.0f, 0.0f, 1.0f);

// texcoords
const vec2f st00(0.0, 0.0);
const vec2f st01(0.0, 1.0);
const vec2f st11(1.0, 1.0);
const vec2f st10(1.0, 0.0);

// cube normals
const vec3f pos_x(1.0f, 0.0f, 0.0f);
const vec3f neg_x(-1.0f, 0.0f, 0.0f);
const vec3f pos_y(0.0f, 1.0f, 0.0f);
const vec3f neg_y(0.0f, -1.0f, 0.0f);
const vec3f pos_z(0.0f, 0.0f, 1.0f);
const vec3f neg_z(0.0f, 0.0f, -1.0f);

//
// color
//

namespace colors
{
    const vec4f black  (0.0f, 0.0f, 0.0f, 1.0f);    // #000000
    const vec4f gray   (0.5f, 0.5f, 0.5f, 1.0f);    // #808080
    const vec4f red    (1.0f, 0.0f, 0.0f, 1.0f);    // #FF0000
    const vec4f yellow (1.0f, 1.0f, 0.0f, 1.0f);    // #FFFF00
    const vec4f lime   (0.0f, 1.0f, 0.0f, 1.0f);    // #00FF00
    const vec4f cyan   (0.0f, 1.0f, 1.0f, 1.0f);    // #00FFFF
    const vec4f blue   (0.0f, 0.0f, 1.0f, 1.0f);    // #00FFFF
    const vec4f magenta(1.0f, 0.0f, 1.0f, 1.0f);    // #FF00FF
    const vec4f maroon (0.5f, 0.0f, 0.0f, 1.0f);    // #800000
    const vec4f olive  (0.5f, 0.5f, 0.0f, 1.0f);    // #808000
    const vec4f green  (0.0f, 0.5f, 0.0f, 1.0f);    // #008000
    const vec4f teal   (0.0f, 0.5f, 0.5f, 1.0f);    // #008080
    const vec4f navy   (0.0f, 0.0f, 0.5f, 1.0f);    // #000080
    const vec4f purple (0.5f, 0.0f, 0.5f, 1.0f);    // #800080
    const vec4f silver (0.75f, 0.75f, 0.75f, 1.0f); // #C0C0C0
    const vec4f white  (1.0f, 1.0f, 1.0f, 1.0f);    // #FFFFFF

    
    static const vec4f elements[] = {
        black, gray, red, yellow, lime, cyan, blue, magenta,
        maroon, olive, green, teal, navy, purple, silver, white
    };

    static const char* names[] = {
        "black", "gray", "red", "yellow", "lime", "cyan", "blue", "magenta",
        "maroon", "olive", "green", "teal", "navy", "purple", "silver", "white"
    };
}

//
// random()
//

#if !defined(__BORLANDC__)
inline void randomize()
{
    srand(static_cast<unsigned int>(time(nullptr)));
}

inline int random(int n)
{
    return rand() % n;
}
#endif

inline int random(int a, int b)
{
    return a + random(b - a);
}

inline float random_real()
{
    return rand() / float(RAND_MAX);
}

inline float random_real(float n)
{
    return random_real() * n;
}

inline float random_real(float a, float b)
{
    return a + random_real(b - a);
}

// random_vec2f()
inline vec2f random_vec2f()
{
    return vec2f(random_real(), random_real());
}

inline vec2f random_vec2f(float n)
{
    return vec2f(random_real(n), random_real(n));
}

inline vec2f random_vec2f(float a, float b)
{
    return vec2f(random_real(a, b), random_real(a, b));
}

// random_vec3f()
inline vec3f random_vec3f()
{
    return vec3f(random_real(), random_real(), random_real());
}

inline vec3f random_vec3f(float n)
{
    return vec3f(random_real(n), random_real(n), random_real(n));
}

inline vec3f random_vec3f(float a, float b)
{
    return vec3f(random_real(a, b), random_real(a, b), random_real(a, b));
}

//
// singleton
//

template<typename T>
class singleton
{
public:
    static T instance;
};

template<typename T>
T singleton<T>::instance = T();

//
// io
//

// load file to buffer
template<typename T>
int load_file(std::vector<T>& buf, const std::string& filename)
{
    std::ifstream in;
    in.open(filename.c_str(), std::ios::binary);
    if (in.is_open()) {
        in.seekg(0, std::ios::end);
        size_t size = static_cast<size_t>(in.tellg());
        buf.resize(size / sizeof(T));
        in.seekg(0, std::ios::beg);
        in.read(reinterpret_cast<char*>(buf.data()), buf.size() * sizeof(T));
        return static_cast<int>(in.gcount());
    }
    else {
        std::cout << "load file \"" << filename << "\" failed!" << std::endl;
        return 0;
    }
}

// load string from file
template<typename T>
int load_file(std::basic_string<T>& str, const std::string& filename)
{
    std::vector<T> buf;
    if (load_file(buf, filename)) {
        str.assign(reinterpret_cast<T*>(buf.data()), buf.size());
        return static_cast<int>(buf.size());
    }
    return 0;
}

//
// initdir
//

#include <direct.h>

#ifdef _MSC_VER
#pragma warning(disable: 4996) // The POSIX name for this item is deprecated.
#endif

inline void initdir(char* argv[])
{
    //char buf[256];
    //cout << getcwd(buf, 256) << endl;

    std::string path = argv[0];
    size_t n = path.find_last_of("\\/");
    path.erase(n, path.length() - n);

    //_chdir(path.c_str());
    chdir(path.c_str());

    std::cout << "current directory: " << path << std::endl;
}

//
// utils
//

// bool to string
inline const char* to_string(bool value)
{
    return value ? "true" : "false";
}

static int fps_state()
{
    static int total = 0;
    static int frames = 0;
    static double t = glfwGetTime();

    ++frames;

    if (glfwGetTime() - t >= 1.0) {
        t = glfwGetTime();
        total = frames;
        frames = 0;
    }

    return total;
}

template<typename T>
void dispose_all(std::vector<T>& container)
{
    for (auto& obj : container) {
        obj.dispose();
    }
    container.clear();
}

template<typename Key, typename Value>
void dispose_all(std::map<Key, Value>& container)
{
    for (auto& obj : container) {
        obj.second.dispose();
    }
    container.clear();
}

#endif // _EZGL_PUBLIC_H
