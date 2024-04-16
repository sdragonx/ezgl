
#include "engine/graphics.h"

using namespace std;

vec2i screen(1024, 576);

vec2f mouse;
vec2f lastpos;
int mouse_button = -1;

// main
int main(int argc, char* argv[])
{
    //initdir(argv);

    // init glfw window
    GLFWwindow* window = initgraph("OpenGL", screen.x, screen.y, true);

    if (!window) {
        cout << "GLFW window create failed." << endl;
        return -1;
    }

    double t = glfwGetTime();
    while (running()) {
        do_events();

        // update
        t = glfwGetTime() - t;
        update(window, t);
        t = glfwGetTime();

        // clear screen
        clear(0.0f, 0.5f, 1.0f);

        // display
        display(window);
        
        // swap buffers
        flush();
    }

    // free variable and resource
    // ...
    
    // close window
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}

void reshape(GLFWwindow* window, int width, int height)
{
    // Window size cannot be 0
    if (width == 0 || height == 0) {
        return;
    }
    screen = vec2i(width, height);
    glViewport(0, 0, width, height);
}

void keyboard(GLFWwindow* window, int key, int scancode, int action, int modifier)
{
    if (action != GLFW_PRESS) {
        return;
    }
    switch (key) {
    case GLFW_KEY_ESCAPE: // press esc to exit
        glfwSetWindowShouldClose(window, GLFW_TRUE);
        break;

    case GLFW_KEY_F1:
        break;
    case GLFW_KEY_F2:
        break;
    case GLFW_KEY_F3:
        break;
    case GLFW_KEY_F4:
        break;

    case GLFW_KEY_1:
        break;
    case GLFW_KEY_2:
        break;
    case GLFW_KEY_3:
        break;
    case GLFW_KEY_4:
        break;

    default:
        break;
    }
}

void click(GLFWwindow* window, int button, int action, int mods)
{
    if (action == GLFW_PRESS) {
        mouse_button = button;
        switch (button) {
        case GLFW_MOUSE_BUTTON_LEFT:
            //printf("mouse left button down\n");
            lastpos = mouse;
            break;
        case GLFW_MOUSE_BUTTON_MIDDLE:
            break;
        case GLFW_MOUSE_BUTTON_RIGHT:
            break;
        default:
            return;
        }
    }
    else if (action == GLFW_RELEASE) {
        switch (button) {
        case GLFW_MOUSE_BUTTON_LEFT:
            //printf("mouse left button up\n");
            mouse_button = 0;
            break;
        case GLFW_MOUSE_BUTTON_MIDDLE:
            break;
        case GLFW_MOUSE_BUTTON_RIGHT:
            break;
        default:
            return;
        }
        mouse_button = -1;
    }
    else {
        cout << action;
    }
}

void motion(GLFWwindow* window, double x, double y)
{
    //printf("mouse move : %d, %d\n", int(x), int(y));
    mouse = vec2f(x, y);

    if (mouse_button == GLFW_MOUSE_BUTTON_LEFT) {
        float x = lastpos.x - mouse.x;
        float y = lastpos.y - mouse.y;

        // ...

        lastpos = mouse;
    }

}

void scroll(GLFWwindow* window, double x, double y)
{
    //printf("scroll : %0.3lf, %0.3lf\n", x, y);
}

void update(GLFWwindow* window, double delay)
{

}

void display(GLFWwindow* window)
{
    
}
