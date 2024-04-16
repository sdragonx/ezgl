#include <direct.h>
#include <map>

#include "graphics.h"

struct GraphicsContext
{
    GLFWwindow* window;

    vec4f penColor;
    //float penWidth;
    vec4f fillColor;
    vec4f textColor;

    double time;
    double delayTime;

    GraphicsContext()
    {
        window = nullptr;
        penColor = vec4f(0.0f, 0.0f, 0.0f, 1.0f);
        //penWidth = 1.0f;
        fillColor = vec4f(1.0f);
        textColor = penColor;

        time = glfwGetTime();
        delayTime = 0.0;
    }
};

static GraphicsContext _m_gl;

//------------------------------------------------------------------------------
// glfw

// glfw 错误回调函数
static void error_event(int error, const char* description)
{
    printf("glfw> 0x%08X: %s\n", error, description);
}

// init glfw
static GLFWwindow* initglfw(const char* title, int width, int height, bool resizable)
{
    // 设置 glfw 错误回调
    glfwSetErrorCallback(error_event);

    // 初始化 glfw
    if (!glfwInit()) {
        return nullptr;
    }

    // 初始化 glfw 配置

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_ANY_PROFILE);       // 设置 OpenGL 任意渲染模式
    //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);      // 设置 OpenGL 核心渲染模式
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);    // 设置 OpenGL 兼容渲染模式

    #if MACOS
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif

    // multisampling
    glfwWindowHint(GLFW_SAMPLES, 4);

    // window resizable
    glfwWindowHint(GLFW_RESIZABLE, resizable ? GLFW_TRUE : GLFW_FALSE);

    // create window
    GLFWwindow* window = glfwCreateWindow(width, height, title, nullptr, nullptr);

    if (!window) {
        glfwTerminate();
        return nullptr;
    }

    // make current
    glfwMakeContextCurrent(window);

    // vertical sync
    glfwSwapInterval(1);

    // init glew
    glewInit();

    // init opengl state
    printf("GL_RENDERER: %s %s\n", glGetString(GL_VENDOR), glGetString(GL_RENDERER));
    printf("GL_VERSION : %s\n", glGetString(GL_VERSION));
    printf("GL_SHADING_LANGUAGE_VERSION: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

    // clear color
    glClearColor(0.0f, 0.5f, 1.0f, 1.0f);

    // shade model
    glShadeModel(GL_SMOOTH);

    // blend
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // texture
    glEnable(GL_TEXTURE_2D);

    // depth test
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glDepthFunc(GL_LEQUAL);
    glClearDepth(1.0);

    //glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    glEnable(GL_NORMALIZE);

    // 表示是否在近处观看，若是则设置为 GL_TRUE，否则（即在无限远处观看）设置为 GL_FALSE
    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);

    // GL_SINGLE_COLOR              表示按通常顺序操作，先计算光照，再计算纹理。
    // GL_SEPARATE_SPECULAR_COLOR   表示将 GL_SPECULAR 属性分离出来，先计算光照的其它部分，
    //                              待纹理操作完成后再计算 GL_SPECULAR。
    //glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL, GL_SEPARATE_SPECULAR_COLOR);

    return window;
}

static void _glfw_keyboard(GLFWwindow* window, int key, int scancode, int action, int modifier)
{
    if (ImGui::IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows | ImGuiFocusedFlags_AnyWindow)) {
        return;
    }

    keyboard(window, key, scancode, action, modifier);
}

static void _glfw_click(GLFWwindow* window, int button, int action, int mods)
{
    if (ImGui::IsWindowHovered(ImGuiHoveredFlags_RootAndChildWindows | ImGuiHoveredFlags_AnyWindow)) {
        return;
    }
    click(window, button, action, mods);
}

static void _glfw_motion(GLFWwindow* window, double x, double y)
{
    if (ImGui::IsWindowHovered(ImGuiHoveredFlags_RootAndChildWindows | ImGuiHoveredFlags_AnyWindow)) {
        return;
    }
    if (ImGui::IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows | ImGuiFocusedFlags_AnyWindow)) {
        //return;
    }

    motion(window, x, y);
}

static void _glfw_scroll(GLFWwindow* window, double x, double y)
{
    if (ImGui::IsWindowHovered(ImGuiHoveredFlags_RootAndChildWindows | ImGuiHoveredFlags_AnyWindow)) {
        return;
    }
    scroll(window, x, y);
}

//------------------------------------------------------------------------------
// ImGui

// 初始化 ImGui
static void initgui(GLFWwindow* window, bool useConfig)
{
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO();
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // 是否使用 ImGui 配置文件
    if (useConfig) {
    }
    else {
        // 关闭 ImGui  配置文件的显示
        //io.ConfigFlags = ImGuiConfigFlags_NoMouseCursorChange;
        io.WantSaveIniSettings = false;
        io.IniFilename = nullptr;
    }

    // 加载微软雅黑字体
    ImFont* font = io.Fonts->AddFontFromFileTTF("c:/windows/fonts/msyh.ttc", 20.0f, nullptr, io.Fonts->GetGlyphRangesChineseFull());
    io.FontDefault = font;

    // 设置主题
    //ImGui::StyleColorsDark();
    ImGui::StyleColorsLight();
    //ImGui::StyleColorsClassic();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL2_Init();
}


// 释放 ImGui 资源
static void closegui()
{
    ImGui_ImplOpenGL2_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

// 开始一个 ImGui 页面
void ui_begin()
{
    ImGui_ImplOpenGL2_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

// 结束 ImGui 页面，渲染显示
void ui_end()
{
    int w, h;
    glfwGetFramebufferSize(_m_gl.window, &w, &h);
    glViewport(0, 0, w, h);

    ImGui::Render();

    // 如果使用了 program，这里停用 program（ImGui 的译文）。
    GLint pid;
    glGetIntegerv(GL_CURRENT_PROGRAM, &pid);
    glUseProgram(GL_NONE);
    glLoadIdentity();

    ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());

    if (pid) {
        glUseProgram(pid);
    }
}

//------------------------------------------------------------------------------

// 初始化图形库
GLFWwindow* initgraph(const char* title, int width, int height, bool resizable)
{
    GLFWwindow* window = initglfw(title, width, height, resizable);

    if (window) {
        _m_gl.window = window;

        // 事件注册在 ImGui 初始化之前
        glfwSetFramebufferSizeCallback(window, reshape);
        glfwSetKeyCallback(window, _glfw_keyboard);
        glfwSetMouseButtonCallback(window, _glfw_click);
        glfwSetCursorPosCallback(window, _glfw_motion);
        glfwSetScrollCallback(window, _glfw_scroll);

        // 初始化 ImGui
        initgui(window, true);
    }

    return window;
}

// 关闭图形库
void closegraph()
{
    closegui();

    glfwDestroyWindow(_m_gl.window);
    _m_gl.window = nullptr;
    glfwTerminate();
}

// 返回程序窗口
GLFWwindow* main_window()
{
    return _m_gl.window;
}

// 返回当前程序是否运行
bool running()
{
    return glfwWindowShouldClose(_m_gl.window) == GLFW_FALSE;
}

// 处理窗口消息
bool do_events()
{
    glfwPollEvents();

    // update time
    if (glfwGetWindowAttrib(_m_gl.window, GLFW_FOCUSED) == GLFW_TRUE) {
        _m_gl.delayTime = glfwGetTime() - _m_gl.time;
        _m_gl.time = glfwGetTime();
    }

    return running();
}

// 返回帧间隔时间
double delay_time()
{
    return _m_gl.delayTime;
}

// 退出
void quit()
{
    glfwSetWindowShouldClose(_m_gl.window, GLFW_TRUE);
}

//------------------------------------------------------------------------------

// 清屏
void clear(float r, float g, float b, float a)
{
    glClearColor(r, g, b, a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

// 结束绘制
void finish()
{
    glfwSwapBuffers(_m_gl.window);
}

// 重新设置视口
void viewport(int x, int y, int width, int height)
{
    glViewport(0, 0, width, height);
}

// 2D 视图模式
void ortho(float left, float right, float bottom, float top, float zNear, float zFar)
{
    // 正交投影
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(left, right, bottom, top, zNear, zFar);
    glFrontFace(GL_CW);

    glMatrixMode(GL_MODELVIEW);
    glDisable(GL_DEPTH_TEST);   // 关闭深度测试
    glDisable(GL_CULL_FACE);    // 关闭面剔除
}

void ortho(int width, int height)
{
    ortho(0.0f, (float) width, (float) height, 0.0f);
}

// 3D 视图模式
void perspective(float fov, float aspectRadio, float zNear, float zFar)
{
    // 透视投影
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(fov, aspectRadio, zNear, zFar);
    glFrontFace(GL_CCW);

    glMatrixMode(GL_MODELVIEW);
    glEnable(GL_DEPTH_TEST);    // 开启深度测试
    glEnable(GL_CULL_FACE);     // 开启面剔除
}

//------------------------------------------------------------------------------

//
// vec2f
//

void vertex_begin(const vec2f* vs)
{
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(2, GL_FLOAT, sizeof(vec2f), vs);
}

void vertex_end(const vec2f* vs)
{
    glDisableClientState(GL_VERTEX_ARRAY);
}

// 绘制顶点列表
void draw_arrays(int shape, const vec2f* vs, size_t pos, size_t size)
{
    vertex_begin(vs);
    glDrawArrays(shape, GLint(pos), GLsizei(size));
    vertex_end(vs);
}

// 绘制索引的顶点列表
void draw_elements(int shape, const vec2f* vs, const uint32_t* indices, size_t size)
{
    vertex_begin(vs);
    glDrawElements(shape, GLsizei(size), GL_UNSIGNED_INT, indices);
    vertex_end(vs);
}

//
// vec3f
//

void vertex_begin(const vec3f* vs)
{
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, sizeof(vec3f), vs);
}

void vertex_end(const vec3f* vs)
{
    glDisableClientState(GL_VERTEX_ARRAY);
}

// 绘制顶点列表
void draw_arrays(int shape, const vec3f* vs, size_t pos, size_t size)
{
    vertex_begin(vs);
    glDrawArrays(shape, GLint(pos), GLsizei(size));
    vertex_end(vs);
}

// 绘制索引的顶点列表
void draw_elements(int shape, const vec3f* vs, const uint32_t* indices, size_t size)
{
    vertex_begin(vs);
    glDrawElements(shape, GLsizei(size), GL_UNSIGNED_INT, indices);
    vertex_end(vs);
}

//
// vertex
//

void vertex_begin(const vertex* vs)
{
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    glVertexPointer(3, GL_FLOAT, sizeof(vertex), glm::value_ptr(vs->position));
    glNormalPointer(GL_FLOAT, sizeof(vertex), glm::value_ptr(vs->normal));
    glColorPointer(4, GL_FLOAT, sizeof(vertex), glm::value_ptr(vs->color));
    glTexCoordPointer(2, GL_FLOAT, sizeof(vertex), glm::value_ptr(vs->texcoord));
}

void vertex_end(const vertex* vs)
{
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}

// 绘制顶点列表
void draw_arrays(int shape, const vertex* vs, size_t pos, size_t size)
{
    vertex_begin(vs);
    glDrawArrays(shape, GLint(pos), GLsizei(size));
    vertex_end(vs);
}

// 绘制索引的顶点列表
void draw_elements(int shape, const vertex* vs, const uint32_t* indices, size_t size)
{
    vertex_begin(vs);
    glDrawElements(shape, GLsizei(size), GL_UNSIGNED_INT, indices);
    vertex_end(vs);
}

//------------------------------------------------------------------------------

//
// 二维绘图
//

static const float _m_INV_255 = 1.0f / 255.0f;

// 设置画笔颜色
void pen_color(float r, float g, float b, float a)
{
    _m_gl.penColor = vec4f(r, g, b, a);
}

void pen_color(int r, int g, int b, int a)
{
    _m_gl.penColor = vec4f(r * _m_INV_255, g * _m_INV_255, b * _m_INV_255, a * _m_INV_255);
}

void pen_color(const vec4f& color)
{
    _m_gl.penColor = color;
}

// 获取画笔颜色
vec4f pen_color()
{
    return _m_gl.penColor;
}

// 设置填充颜色
void fill_color(float r, float g, float b, float a)
{
    _m_gl.fillColor = vec4f(r, g, b, a);
}

void fill_color(int r, int g, int b, int a)
{
    _m_gl.fillColor = vec4f(r * _m_INV_255, g * _m_INV_255, b * _m_INV_255, a * _m_INV_255);
}

void fill_color(const vec4f& color)
{
    _m_gl.fillColor = color;
}

// 设置画笔宽度
void pen_width(float value)
{
    glLineWidth(value);
}

// 获取画笔宽度
float pen_width()
{
    float n;
    glGetFloatv(GL_LINE_WIDTH, &n);
    return n;
}

// 获取填充颜色
vec4f fill_color()
{
    return _m_gl.fillColor;
}

// 设置填充模式
void fill_mode(bool fill)
{
    glPolygonMode(GL_FRONT_AND_BACK, fill ? GL_FILL : GL_LINE);
}

// 绘制点
void draw_point(float x, float y, float size)
{
    const vec4f& color = _m_gl.penColor;
    glBindTexture(GL_TEXTURE_2D, 0);
    if (size <= 1.0f) {
        vertex v(vec3f(x - size, y - size, 0.0f), vec2f(0.0f, 0.0f), color, vec3f(0.0f, 0.0f, 1.0f));
        draw_arrays(GL_POINTS, &v, 0, 1);
    }
    else {
        size *= 0.5f;
        vertex vs[] = {
            vertex(vec3f(x - size, y - size, 0.0f), vec2f(0.0f, 0.0f), color, vec3f(0.0f, 0.0f, 1.0f)),
            vertex(vec3f(x + size, y - size, 0.0f), vec2f(1.0f, 0.0f), color, vec3f(0.0f, 0.0f, 1.0f)),
            vertex(vec3f(x + size, y + size, 0.0f), vec2f(1.0f, 1.0f), color, vec3f(0.0f, 0.0f, 1.0f)),
            vertex(vec3f(x - size, y + size, 0.0f), vec2f(0.0f, 1.0f), color, vec3f(0.0f, 0.0f, 1.0f)),
        };
        draw_arrays(GL_TRIANGLE_FAN, vs, 0, 4);
    }
}

void draw_point(const vec2f& pos, float size)
{
    draw_point(pos.x, pos.y, size);
}

// 绘制直线
void draw_line(float x1, float y1, float z1, float x2, float y2, float z2)
{
    vertex vs[] = {
        vertex(vec3f(x1, y1, z1), vec2f(0.0f, 0.0f), _m_gl.penColor, vec3f(0.0f, 0.0f, 1.0f)),
        vertex(vec3f(x2, y2, z2), vec2f(1.0f, 0.0f), _m_gl.penColor, vec3f(0.0f, 0.0f, 1.0f)),
    };
    draw_arrays(GL_LINES, vs, 0, 2);
}

void draw_line(float x1, float y1, float x2, float y2)
{
    draw_line(x1, y1, 0.0f, x2, y2, 0.0f);
}

void draw_line(const vec3f& p1, const vec3f& p2)
{
    draw_line(p1.x, p1.y, p1.z, p2.x, p2.y, p2.z);
}

void draw_line(const vec2f& p1, const vec2f& p2)
{
    draw_line(p1.x, p1.y, p2.x, p2.y);
}

// 绘制矩形
void draw_rect(float x, float y, float width, float height)
{
    const vec4f& color = _m_gl.penColor;

    vertex vs[] = {
        vertex(vec3f(x,           y,          0.0f), vec2f(0.0f, 0.0f), color, vec3f(0.0f, 0.0f, 1.0f)),
        vertex(vec3f(x + width,   y,          0.0f), vec2f(1.0f, 0.0f), color, vec3f(0.0f, 0.0f, 1.0f)),
        vertex(vec3f(x + width,   y + height, 0.0f), vec2f(1.0f, 1.0f), color, vec3f(0.0f, 0.0f, 1.0f)),
        vertex(vec3f(x,           y + height, 0.0f), vec2f(0.0f, 1.0f), color, vec3f(0.0f, 0.0f, 1.0f)),
    };

    glBindTexture(GL_TEXTURE_2D, 0);
    draw_arrays(GL_LINE_LOOP, vs, 0, 4);
}

// 填充矩形
void fill_rect(float x, float y, float width, float height)
{
    const vec4f& color = _m_gl.fillColor;

    vertex vs[] = {
        vertex(vec3f(x,           y,          0.0f), vec2f(0.0f, 0.0f), color, vec3f(0.0f, 0.0f, 1.0f)),
        vertex(vec3f(x + width,   y,          0.0f), vec2f(1.0f, 0.0f), color, vec3f(0.0f, 0.0f, 1.0f)),
        vertex(vec3f(x + width,   y + height, 0.0f), vec2f(1.0f, 1.0f), color, vec3f(0.0f, 0.0f, 1.0f)),
        vertex(vec3f(x,           y + height, 0.0f), vec2f(0.0f, 1.0f), color, vec3f(0.0f, 0.0f, 1.0f)),
    };

    glBindTexture(GL_TEXTURE_2D, 0);
    draw_arrays(GL_TRIANGLE_FAN, vs, 0, 4);
}

// 绘制图片
void draw_image(GLuint image, float x, float y, float width, float height,
    float u1, float v1, float u2, float v2)
{
    const vec4f& color = _m_gl.fillColor;

    vertex vs[] = {
        vertex(vec3f(x,           y,          0.0f), vec2f(u1, v1), color, vec3f(0.0f, 0.0f, 1.0f)),
        vertex(vec3f(x + width,   y,          0.0f), vec2f(u2, v1), color, vec3f(0.0f, 0.0f, 1.0f)),
        vertex(vec3f(x + width,   y + height, 0.0f), vec2f(u2, v2), color, vec3f(0.0f, 0.0f, 1.0f)),
        vertex(vec3f(x,           y + height, 0.0f), vec2f(u1, v2), color, vec3f(0.0f, 0.0f, 1.0f)),
    };
    glBindTexture(GL_TEXTURE_2D, image);
    draw_arrays(GL_TRIANGLE_FAN, vs, 0, 4);
}

//------------------------------------------------------------------------------

//
// 文字输出
//

// 设置字体颜色
void text_color(float r, float g, float b, float a)
{
    _m_gl.textColor = vec4f(r, g, b, a);
}

void text_color(const vec4f& color)
{
    _m_gl.textColor = color;
}

// 获取字体颜色
vec4f text_color()
{
    return _m_gl.textColor;
}

// 返回文本宽度
float text_width(const std::string& text)
{
    ImFont* font = ImGui::GetFont();
    if (font) {
        ImVec2 size = font->CalcTextSizeA(font->FontSize, FLT_MAX, 0.0f, text.c_str(), nullptr, nullptr);
        return size.x;
    }
    return 0.0f;
}

// 返回文本高度
float text_height(const std::string& text)
{
    ImFont* font = ImGui::GetFont();
    if (font) {
        return font->FontSize;
    }
    return 0.0f;
}

// 输出文本
void textout(float x, float y, const std::string& text)
{
    //_m_gl.font.textout(float(x), float(y), text.c_str());
    ImDrawList* dc = ImGui::GetBackgroundDrawList();
    if (dc) {
        dc->AddText(ImVec2(x, y), ImColor(_m_gl.textColor), text.c_str());
    }
}

//------------------------------------------------------------------------------

//
// 光照
//

void direction_light(GLuint light, float x, float y, float z, float r, float g, float b)
{
    GLfloat position[4] = { x, y, z, 0.0f };
    GLfloat ambient[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
    GLfloat diffuse[4] = { r, g, b, 1.0f };
    GLfloat specular[4] = { r, g, b, 1.0f };

    glLightfv(light, GL_POSITION, position);
    glLightfv(light, GL_AMBIENT, ambient);
    glLightfv(light, GL_DIFFUSE, diffuse);
    glLightfv(light, GL_SPECULAR, specular);

    glEnable(light);
}

void point_light(GLuint light, float x, float y, float z, float r, float g, float b)
{
    GLfloat position[4] = { x, y, z, 1.0f };
    GLfloat ambient[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
    GLfloat diffuse[4] = { r, g, b, 1.0f };
    GLfloat specular[4] = { r, g, b, 1.0f };

    glLightfv(light, GL_POSITION, position);
    glLightfv(light, GL_AMBIENT, ambient);
    glLightfv(light, GL_DIFFUSE, diffuse);
    glLightfv(light, GL_SPECULAR, specular);

    glLightf(light, GL_CONSTANT_ATTENUATION, 1.0f);
    glLightf(light, GL_LINEAR_ATTENUATION, 0.0f);
    glLightf(light, GL_QUADRATIC_ATTENUATION, 0.0f);

    glEnable(light);
}

void spot_light(GLuint light, float x, float y, float z, float xdir, float ydir, float zdir,
    float r, float g, float b)
{
    GLfloat direction[4] = { xdir, ydir, zdir, 1.0f };
    point_light(light, x, y, z, r, g, b);
    glLightfv(light, GL_SPOT_DIRECTION, direction);
    glLightf(light, GL_SPOT_EXPONENT, 1.0f);
    glLightf(light, GL_SPOT_CUTOFF, 45.0f);
}

void direction_light(GLuint light, const vec3f& pos, const vec3f& color)
{
    direction_light(light, pos.x, pos.y, pos.z, color.r, color.g, color.b);
}

void point_light(GLuint light, const vec3f& pos, const vec3f& color)
{
    point_light(light, pos.x, pos.y, pos.z, color.r, color.g, color.b);
}

void spot_light(GLuint light, const vec3f& pos, const vec3f& dir, const vec3f& color)
{
    spot_light(light, pos.x, pos.y, pos.z, dir.x, dir.y, dir.z, color.r, color.g, color.b);
}

void light_pos(GLuint light, const vec3f& pos, bool isDirectionLight)
{
    GLfloat position[4] = { pos.x, pos.y, pos.z, 1.0f };
    if (isDirectionLight)position[3] = 0.0f;
    glLightfv(light, GL_SPOT_DIRECTION, position);
}

void light_dir(GLuint light, const vec3f& dir)
{
    GLfloat direction[4] = { dir.x, dir.y, dir.z, 1.0f };
    glLightfv(light, GL_SPOT_DIRECTION, direction);
}

// 设置材质属性
void set_material(const glmaterial& material)
{
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, glm::value_ptr(material.ambient));
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, glm::value_ptr(material.diffuse));
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, glm::value_ptr(material.specular));
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, glm::value_ptr(material.emission));
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, material.shininess);

    material.tex_Kd.bind();
}

//------------------------------------------------------------------------------

//
// 三维绘图
//

// 绘制网格平台
void draw_grid(float y, int size)
{
    glDisable(GL_TEXTURE_2D);
    GLboolean lightingEnabled;
    glGetBooleanv(GL_LIGHTING, &lightingEnabled);
    if (lightingEnabled) glDisable(GL_LIGHTING);

    glBegin(GL_LINES);
    for (int i = -size; i <= size; ++i) {
        if (i % 10 == 0) {
            glColor4f(1.0f, 1.0f, 1.0f, 0.25f);
        }
        else {
            glColor4f(1.0f, 1.0f, 1.0f, 0.125f);
        }
        glVertex3f(-float(size), y, float(i));
        glVertex3f(float(size), y, float(i));
        glVertex3f(-float(i), y, float(size));
        glVertex3f(-float(i), y, -float(size));
    }
    glEnd();

    glEnable(GL_TEXTURE_2D);
    if (lightingEnabled) glEnable(GL_LIGHTING);
}

// 绘制平台
void draw_plane(float x, float y, float z, float size,
    float u1, float v1, float u2, float v2)
{
    const vec4f& color = _m_gl.fillColor;

    size *= 0.5f;
    vertex vs[] = {
        vertex(vec3f(x - size, y, z - size), vec2f(u1, v1), color, vec3f(0.0f, 1.0f, 0.0f)),
        vertex(vec3f(x - size, y, z + size), vec2f(u1, v2), color, vec3f(0.0f, 1.0f, 0.0f)),
        vertex(vec3f(x + size, y, z + size), vec2f(u2, v2), color, vec3f(0.0f, 1.0f, 0.0f)),
        vertex(vec3f(x + size, y, z - size), vec2f(u2, v1), color, vec3f(0.0f, 1.0f, 0.0f))
    };

    draw_arrays(GL_TRIANGLE_FAN, vs, 0, 4);
}

// 绘制立方体
void draw_cube(float x1, float y1, float z1, float x2, float y2, float z2)
{
    const vec4f& color = _m_gl.penColor;

    vertex vs[] = {
        // front
        vertex(vec3f(x1, y2, z2), vec2f(0.0f, 0.0f), color, vec3f(0.0f, 0.0f, 1.0f)),
        vertex(vec3f(x1, y1, z2), vec2f(0.0f, 1.0f), color, vec3f(0.0f, 0.0f, 1.0f)),
        vertex(vec3f(x2, y1, z2), vec2f(1.0f, 1.0f), color, vec3f(0.0f, 0.0f, 1.0f)),
        vertex(vec3f(x2, y2, z2), vec2f(1.0f, 0.0f), color, vec3f(0.0f, 0.0f, 1.0f)),

        // back
        vertex(vec3f(x1, y2, z1), vec2f(0.0f, 0.0f), color, vec3f(0.0f, 0.0f, 1.0f)),
        vertex(vec3f(x1, y1, z1), vec2f(0.0f, 1.0f), color, vec3f(0.0f, 0.0f, 1.0f)),
        vertex(vec3f(x2, y1, z1), vec2f(1.0f, 1.0f), color, vec3f(0.0f, 0.0f, 1.0f)),
        vertex(vec3f(x2, y2, z1), vec2f(1.0f, 0.0f), color, vec3f(0.0f, 0.0f, 1.0f)),
    };

    static uint32_t indices[24] = {
        0, 1, 1, 2, 2, 3, 3, 0,
        4, 5, 5, 6, 6, 7, 7, 4,
        0, 4, 1, 5, 2, 6, 3, 7
    };

    draw_elements(GL_LINES, vs, indices, 24u);
}

void draw_cube(float x, float y, float z, float size)
{
    size *= 0.5f;
    draw_cube(x - size, y - size, z - size, x + size, y + size, z + size);
}

void draw_cube(const vec3f& pos, float size)
{
    draw_cube(pos.x, pos.y, pos.z, size);
}

// 填充立方体
void fill_cube(float x1, float y1, float z1, float x2, float y2, float z2)
{
    const vec4f& color = _m_gl.fillColor;

    vertex vs[] = {
        // front
        vertex(vec3f(x1, y2, z2), vec2f(0.0f, 0.0f), color, vec3f(0.0f, 0.0f, 1.0f)),
        vertex(vec3f(x1, y1, z2), vec2f(0.0f, 1.0f), color, vec3f(0.0f, 0.0f, 1.0f)),
        vertex(vec3f(x2, y1, z2), vec2f(1.0f, 1.0f), color, vec3f(0.0f, 0.0f, 1.0f)),
        vertex(vec3f(x2, y2, z2), vec2f(1.0f, 0.0f), color, vec3f(0.0f, 0.0f, 1.0f)),

        // back
        vertex(vec3f(x2, y2, z1), vec2f(0.0f, 0.0f), color, vec3f(0.0f, 0.0f, -1.0f)),
        vertex(vec3f(x2, y1, z1), vec2f(0.0f, 1.0f), color, vec3f(0.0f, 0.0f, -1.0f)),
        vertex(vec3f(x1, y1, z1), vec2f(1.0f, 1.0f), color, vec3f(0.0f, 0.0f, -1.0f)),
        vertex(vec3f(x1, y2, z1), vec2f(1.0f, 0.0f), color, vec3f(0.0f, 0.0f, -1.0f)),

        // left
        vertex(vec3f(x1, y2, z1), vec2f(0.0f, 0.0f), color, vec3f(-1.0f, 0.0f, 0.0f)),
        vertex(vec3f(x1, y1, z1), vec2f(0.0f, 1.0f), color, vec3f(-1.0f, 0.0f, 0.0f)),
        vertex(vec3f(x1, y1, z2), vec2f(1.0f, 1.0f), color, vec3f(-1.0f, 0.0f, 0.0f)),
        vertex(vec3f(x1, y2, z2), vec2f(1.0f, 0.0f), color, vec3f(-1.0f, 0.0f, 0.0f)),

        // right
        vertex(vec3f(x2, y2, z2), vec2f(0.0f, 0.0f), color, vec3f(1.0f, 0.0f, 0.0f)),
        vertex(vec3f(x2, y1, z2), vec2f(0.0f, 1.0f), color, vec3f(1.0f, 0.0f, 0.0f)),
        vertex(vec3f(x2, y1, z1), vec2f(1.0f, 1.0f), color, vec3f(1.0f, 0.0f, 0.0f)),
        vertex(vec3f(x2, y2, z1), vec2f(1.0f, 0.0f), color, vec3f(1.0f, 0.0f, 0.0f)),

        // top
        vertex(vec3f(x1, y2, z1), vec2f(0.0f, 0.0f), color, vec3f(0.0f, 1.0f, 0.0f)),
        vertex(vec3f(x1, y2, z2), vec2f(0.0f, 1.0f), color, vec3f(0.0f, 1.0f, 0.0f)),
        vertex(vec3f(x2, y2, z2), vec2f(1.0f, 1.0f), color, vec3f(0.0f, 1.0f, 0.0f)),
        vertex(vec3f(x2, y2, z1), vec2f(1.0f, 0.0f), color, vec3f(0.0f, 1.0f, 0.0f)),

        // bottom
        vertex(vec3f(x1, y1, z2), vec2f(0.0f, 0.0f), color, vec3f(0.0f, -1.0f, 0.0f)),
        vertex(vec3f(x1, y1, z1), vec2f(0.0f, 1.0f), color, vec3f(0.0f, -1.0f, 0.0f)),
        vertex(vec3f(x2, y1, z1), vec2f(1.0f, 1.0f), color, vec3f(0.0f, -1.0f, 0.0f)),
        vertex(vec3f(x2, y1, z2), vec2f(1.0f, 0.0f), color, vec3f(0.0f, -1.0f, 0.0f)),
    };

    static uint32_t indices[36] = {
        0,1,2,0,2,3,  4,5,6,4,6,7,  8,9,10, 8,10,11,  12,13,14,12,14,15,  16,17,18,16,18,19,  20,21,22,20,22,23
    };

    draw_elements(GL_TRIANGLES, vs, indices, 36u);
}

void fill_cube(float x, float y, float z, float size)
{
    size *= 0.5f;
    fill_cube(x - size, y - size, z - size, x + size, y + size, z + size);
}

void fill_cube(const vec3f& pos, float size)
{
    fill_cube(pos.x, pos.y, pos.z, size);
}

//
// 绘制箭头
//

static vec3f arrowVertices[] = {
    { 0.875000, 0.031250, -0.000000 },
    { 0.875000, 0.022097, -0.022097 },
    { 0.875000, 0.000000, -0.031250 },
    { 0.875000, -0.022097, -0.022097 },
    { 0.875000, -0.031250, -0.000000 },
    { 0.875000, -0.022097, 0.022097 },
    { 0.875000, -0.000000, 0.031250 },
    { 0.875000, 0.022097, 0.022097 },
    { 0.875000, 0.007813, -0.000000 },
    { 0.875000, 0.005524, -0.005524 },
    { 0.875000, 0.000000, -0.007813 },
    { 0.875000, -0.005524, -0.005524 },
    { 0.875000, -0.007813, -0.000000 },
    { 0.875000, -0.005524, 0.005524 },
    { 0.875000, -0.000000, 0.007813 },
    { 0.875000, 0.005524, 0.005524 },
    { 0.000000, 0.007813, -0.000000 },
    { 0.000000, 0.005524, -0.005524 },
    { 0.000000, 0.000000, -0.007813 },
    { 0.000000, -0.005524, -0.005524 },
    { 0.000000, -0.007813, -0.000000 },
    { 0.000000, -0.005524, 0.005524 },
    { 0.000000, -0.000000, 0.007813 },
    { 0.000000, 0.005524, 0.005524 },
    { 1.000000, 0.000000, 0.000000 },
    { 0.875000, 0.000000, 0.000000 },
    { 0.000000, 0.000000, 0.000000 },
};

static uint32_t arrowIndices[] = {
    24,0,1, 24,1,2, 24,2,3, 24,3,4, 24,4,5, 24,5,6, 24,6,7, 24,7,0,
    0,25,1, 1,25,2, 2,25,3, 3,25,4, 4,25,5, 5,25,6, 6,25,7, 7,25,0,
    8,16,17, 8,17,9, 9,17,18, 9,18,10, 10,18,19, 10,19,11, 11,19,20, 11,20,12,
    12,20,21, 12,21,13, 13,21,22, 13,22,14, 14,22,23, 14,23,15, 15,23,16, 15,16,8,
    16,26,17, 17,26,18, 18,26,19, 19,26,20, 20,26,21, 21,26,22, 22,26,23, 23,26,16,
};

// 绘制箭头
void draw_arrow(float angle, float x, float y, float z)
{
    glPushMatrix();
    glRotatef(angle, x, y, z);
    draw_elements(GL_TRIANGLES, arrowVertices, arrowIndices, _countof(arrowIndices));
    glPopMatrix();
}

// 绘制坐标轴
void draw_axes(int x, int y, int size, mat4f mv)
{
    glViewport(x - size / 2, y - size / 2, size, size);
    ortho(-1, 1, -1, 1, -1000.0f, 1000.0f);
    
    glLoadMatrixf(glm::value_ptr(mv));

    glDisable(GL_TEXTURE_2D);
    glDisable(GL_LIGHTING);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    glColor3f(1.0f, 0.0f, 0.0f);
    draw_arrow(0.0f, 1.0f, 0.0f, 0.0f);

    glColor3f(0.0f, 0.0f, 1.0f);
    draw_arrow(90.0f, 0.0f, 1.0f, 0.0f);

    glColor3f(0.0f, 1.0f, 0.0f);
    draw_arrow(90.0f, 0.0f, 0.0f, 1.0f);
}
