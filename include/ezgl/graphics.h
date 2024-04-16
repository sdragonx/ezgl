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
// 框架
//

// 初始化图形库
GLFWwindow* initgraph(const char* title, int width, int height, bool resizable = true);

// 关闭图形库
void closegraph();

// 返回程序窗口
GLFWwindow* main_window();

// 返回当前程序是否运行
bool running();

// 处理窗口消息
bool do_events();

// 返回帧间隔时间
double delay_time();

// 退出
void quit();

// 清屏
void clear(float r, float g, float b, float a = 1.0f);

// 结束绘制
void finish();

// 重新设置视口
void viewport(int x, int y, int width, int height);

// 2D 视图模式
void ortho(float left, float right, float bottom, float top, float zNear = -1.0f, float zFar = 1.0f);
void ortho(int width, int height);

// 3D 视图模式
void perspective(float fov, float aspectRadio, float zNear, float zFar);

// 开始一个 ImGui 页面
void ui_begin();

// 结束 ImGui 页面，渲染显示
void ui_end();

//
// 事件
//

void reshape(GLFWwindow* window, int width, int height);
void keyboard(GLFWwindow* window, int key, int scancode, int action, int modifier);
void click(GLFWwindow* window, int button, int action, int mods);
void motion(GLFWwindow* window, double x, double y);
void scroll(GLFWwindow* window, double x, double y);
void update(GLFWwindow* window, double delay);
void display(GLFWwindow* window);

//
// 绘图
//

// 绘制顶点列表
void draw_arrays(int shape, const vec2f* vs, size_t pos, size_t size);
void draw_arrays(int shape, const vec3f* vs, size_t pos, size_t size);
void draw_arrays(int shape, const vertex* vs, size_t pos, size_t size);

// 绘制索引的顶点列表
void draw_elements(int shape, const vec2f* vs, const uint32_t* indices, size_t size);
void draw_elements(int shape, const vec3f* vs, const uint32_t* indices, size_t size);
void draw_elements(int shape, const vertex* vs, const uint32_t* indices, size_t size);

//
// 二维绘图
//

// 设置画笔颜色
void pen_color(float r, float g, float b, float a = 1.0f);
void pen_color(int r, int g, int b, int a = 255);
void pen_color(const vec4f& color);

// 获取画笔颜色
vec4f pen_color();

// 设置画笔宽度
void pen_width(float value);

// 获取画笔宽度
float pen_width();

// 设置填充颜色
void fill_color(float r, float g, float b, float a = 1.0f);
void fill_color(const vec4f& color);

// 获取填充颜色
vec4f fill_color();

// 设置填充模式
void fill_mode(bool fill);

// 绘制点
void draw_point(float x, float y, float size);
void draw_point(const vec2f& pos, float size);

// 绘制直线
void draw_line(float x1, float y1, float z1, float x2, float y2, float z2);
void draw_line(float x1, float y1, float x2, float y2);

void draw_line(const vec3f& p1, const vec3f& p2);
void draw_line(const vec2f& p1, const vec2f& p2);

// 绘制矩形
void draw_rect(float x, float y, float width, float height);

// 填充矩形
void fill_rect(float x, float y, float width, float height);

// 绘制图片
void draw_image(GLuint image, float x, float y, float width, float height,
    float u1 = 0.0f, float v1 = 0.0f, float u2 = 1.0f, float v2 = 1.0f);

//
// 文字输出
//

// 设置字体颜色
void text_color(float r, float g, float b, float a = 1.0f);
void text_color(const vec4f& color);

// 获取字体颜色
vec4f text_color();

// 返回文本宽度
float text_width(const std::string& text);

// 返回文本高度
float text_height(const std::string& text);

// 输出文本
void textout(float x, float y, const std::string& text);

//
// 三维绘图
//

// 绘制网格平台
void draw_grid(float y, int size);

// 绘制平台
void draw_plane(float x, float y, float z, float size,
    float u1 = 0.0f, float v1 = 0.0f, float u2 = 1.0f, float v2 = 1.0f);

// 绘制立方体
void draw_cube(float x1, float y1, float z1, float x2, float y2, float z2);
void draw_cube(float x, float y, float z, float size);
void draw_cube(const vec3f& pos, float size);

// 填充立方体
void fill_cube(float x1, float y1, float z1, float x2, float y2, float z2);
void fill_cube(float x, float y, float z, float size);
void fill_cube(const vec3f& pos, float size);

// 绘制箭头
void draw_arrow(float angle, float x, float y, float z);

// 绘制坐标轴
void draw_axes(int x, int y, int size, mat4f mv);

//
// 图片函数
//

// 加载图片
//GLuint loadimage(const char* filename);

//
// 光照
//

// 光照属性
void direction_light(GLuint light, const vec3f& pos, const vec3f& color);
void point_light(GLuint light, const vec3f& pos, const vec3f& color);
void spot_light(GLuint light, const vec3f& pos, const vec3f& dir, const vec3f& color);
void light_pos(GLuint light, const vec3f& pos, bool isDirectionLight);
void light_dir(GLuint light, const vec3f& dir);

// 设置材质属性
void set_material(const glmaterial& material);

#endif // _EZGL_GRAPHICS_H
