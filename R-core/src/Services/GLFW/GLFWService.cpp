#include "rcpch.h"

#include "Services/GLFW/GLFWService.h"
#include "Core/Core.h"



namespace RC {

	void GLFWService::Init()
	{
		
	}

	void GLFWService::OnUpdate()
	{
	}

	std::shared_ptr<GLFWService> GLFWService::Create()
	{
		return std::make_shared<GLFWService>();
	}
}