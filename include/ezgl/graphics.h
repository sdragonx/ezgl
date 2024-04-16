#ifndef _EZGL_GRAPHICS_H
#define _EZGL_GRAPHICS_H

//
// Easy OpenGL Graphics
//

#include "public.h"
#include "glimage.h"
#include "gllighting.h"
#include "mesh.h"
#include "vertex.h"

//
// ���
//

// ��ʼ��ͼ�ο�
GLFWwindow* initgraph(const char* title, int width, int height, bool resizable = true);

// �ر�ͼ�ο�
void closegraph();

// ���س��򴰿�
GLFWwindow* main_window();

// ���ص�ǰ�����Ƿ�����
bool running();

// ��������Ϣ
bool do_events();

// ����֡���ʱ��
double delay_time();

// �˳�
void quit();

// ����
void clear(float r, float g, float b, float a = 1.0f);

// ��������
void finish();

// ���������ӿ�
void viewport(int x, int y, int width, int height);

// 2D ��ͼģʽ
void ortho(float left, float right, float bottom, float top, float zNear = -1.0f, float zFar = 1.0f);
void ortho(int width, int height);

// 3D ��ͼģʽ
void perspective(float fov, float aspectRadio, float zNear, float zFar);

// ��ʼһ�� ImGui ҳ��
void ui_begin();

// ���� ImGui ҳ�棬��Ⱦ��ʾ
void ui_end();

//
// �¼�
//

void reshape(GLFWwindow* window, int width, int height);
void keyboard(GLFWwindow* window, int key, int scancode, int action, int modifier);
void click(GLFWwindow* window, int button, int action, int mods);
void motion(GLFWwindow* window, double x, double y);
void scroll(GLFWwindow* window, double x, double y);
void update(GLFWwindow* window, double delay);
void display(GLFWwindow* window);

//
// ��ͼ
//

// ���ƶ����б�
void draw_arrays(int shape, const vec2f* vs, size_t pos, size_t size);
void draw_arrays(int shape, const vec3f* vs, size_t pos, size_t size);
void draw_arrays(int shape, const vertex* vs, size_t pos, size_t size);

// ���������Ķ����б�
void draw_elements(int shape, const vec2f* vs, const uint32_t* indices, size_t size);
void draw_elements(int shape, const vec3f* vs, const uint32_t* indices, size_t size);
void draw_elements(int shape, const vertex* vs, const uint32_t* indices, size_t size);

//
// ��ά��ͼ
//

// ���û�����ɫ
void pen_color(float r, float g, float b, float a = 1.0f);
void pen_color(int r, int g, int b, int a = 255);
void pen_color(const vec4f& color);

// ��ȡ������ɫ
vec4f pen_color();

// ���û��ʿ��
void pen_width(float value);

// ��ȡ���ʿ��
float pen_width();

// ���������ɫ
void fill_color(float r, float g, float b, float a = 1.0f);
void fill_color(const vec4f& color);

// ��ȡ�����ɫ
vec4f fill_color();

// �������ģʽ
void fill_mode(bool fill);

// ���Ƶ�
void draw_point(float x, float y, float size);
void draw_point(const vec2f& pos, float size);

// ����ֱ��
void draw_line(float x1, float y1, float z1, float x2, float y2, float z2);
void draw_line(float x1, float y1, float x2, float y2);

void draw_line(const vec3f& p1, const vec3f& p2);
void draw_line(const vec2f& p1, const vec2f& p2);

// ���ƾ���
void draw_rect(float x, float y, float width, float height);

// ������
void fill_rect(float x, float y, float width, float height);

// ����ͼƬ
void draw_image(GLuint image, float x, float y, float width, float height,
    float u1 = 0.0f, float v1 = 0.0f, float u2 = 1.0f, float v2 = 1.0f);

//
// �������
//

// ����������ɫ
void text_color(float r, float g, float b, float a = 1.0f);
void text_color(const vec4f& color);

// ��ȡ������ɫ
vec4f text_color();

// �����ı����
float text_width(const std::string& text);

// �����ı��߶�
float text_height(const std::string& text);

// ����ı�
void textout(float x, float y, const std::string& text);

//
// ��ά��ͼ
//

// ��������ƽ̨
void draw_grid(float y, int size);

// ����ƽ̨
void draw_plane(float x, float y, float z, float size,
    float u1 = 0.0f, float v1 = 0.0f, float u2 = 1.0f, float v2 = 1.0f);

// ����������
void draw_cube(float x1, float y1, float z1, float x2, float y2, float z2);
void draw_cube(float x, float y, float z, float size);
void draw_cube(const vec3f& pos, float size);

// ���������
void fill_cube(float x1, float y1, float z1, float x2, float y2, float z2);
void fill_cube(float x, float y, float z, float size);
void fill_cube(const vec3f& pos, float size);

// ���Ƽ�ͷ
void draw_arrow(float angle, float x, float y, float z);

// ����������
void draw_axes(int x, int y, int size, mat4f mv);

//
// ͼƬ����
//

// ����ͼƬ
//GLuint loadimage(const char* filename);

//
// ����
//

// ��������
void direction_light(GLuint light, const vec3f& pos, const vec3f& color);
void point_light(GLuint light, const vec3f& pos, const vec3f& color);
void spot_light(GLuint light, const vec3f& pos, const vec3f& dir, const vec3f& color);
void light_pos(GLuint light, const vec3f& pos, bool isDirectionLight);
void light_dir(GLuint light, const vec3f& dir);

// ���ò�������
void set_material(const glmaterial& material);

#endif // _EZGL_GRAPHICS_H
