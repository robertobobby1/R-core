#include "rcpch.h"

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "Services/GLFW/RCGlfw.h"
#include "Core/Core.h"

namespace RC {

	std::shared_ptr<RCGlfw> RCGlfw::Create()
	{
		return std::make_shared<RCGlfw>();
	}

	static void OnGLFWError(int error, const char* description)
	{
		RC_LOG_ERROR("GLFW Error ({0}): {1}", error, description);
	}

	void RCGlfw::PrintOpenGLInfo()
	{
		if (m_isPrinted)
			return;

		GLint major, minor;
		glGetIntegerv(GL_MAJOR_VERSION, &major);
		glGetIntegerv(GL_MINOR_VERSION, &minor);

		RC_LOG_INFO("GL Vendor            : {0}", (char*)glGetString(GL_VENDOR));
		RC_LOG_INFO("GL Renderer          : {0}", (char*)glGetString(GL_RENDERER));
		RC_LOG_INFO("GL Version (string)  : {0}", (char*)glGetString(GL_VERSION));
		RC_LOG_INFO("GL Version (integer) : {0}.{1}", major, minor);
		RC_LOG_INFO("GLSL Version         : {0}", (char*)glGetString(GL_SHADING_LANGUAGE_VERSION));

		m_isPrinted = true;
	}

	void RCGlfw::Init()
	{
		glfwSetErrorCallback(OnGLFWError);
		int success = glfwInit();
		RC_ASSERT_MSG(success, "Could not initialize GLFW!");
	}

	void RCGlfw::Run()
	{
	}
}