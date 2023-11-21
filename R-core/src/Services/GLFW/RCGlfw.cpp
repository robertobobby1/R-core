#include "rcpch.h"

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "Services/GLFW/RCGlfw.h"
#include "Core/Core.h"

namespace RC {

    std::shared_ptr<RCGlfw> RCGlfw::Create() { return std::make_shared<RCGlfw>(); }

    static void OnGLFWError(int error, const char* description) {
        RC_LOG_ERROR("GLFW Error ({0}): {1}", error, description);
    }

    void RCGlfw::PrintOpenGLInfo() {
        if (m_isPrinted) return;

        GLint major, minor;
        glGetIntegerv(GL_MAJOR_VERSION, &major);
        glGetIntegerv(GL_MINOR_VERSION, &minor);

        std::string s;

        s += fmt::format("GL Vendor            : {}\n", (char*)glGetString(GL_VENDOR));
        s += fmt::format("GL Renderer          : {}\n", (char*)glGetString(GL_RENDERER));
        s += fmt::format("GL Version (string)  : {}\n", (char*)glGetString(GL_VERSION));
        s += fmt::format("GL Version (integer) : {}.{}\n", major, minor);
        s += fmt::format("GLSL Version         : {}\n", (char*)glGetString(GL_SHADING_LANGUAGE_VERSION));

        RC_LOG_DEBUG("OpenGL information: \n{0}", s);

        m_isPrinted = true;
    }

    void RCGlfw::Init() {
        glfwSetErrorCallback(OnGLFWError);
        int success = glfwInit();
        RC_ASSERT_MSG(success, "Could not initialize GLFW!");
    }

    void RCGlfw::Run() {}
}  // namespace RC