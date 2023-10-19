#include <iostream>
#include "R-core.h"
#include <memory>

int main(int argc, char** argv)
{
	RC::Application* app = new RC::Application();
	app->AddService(std::make_shared<RC::ImGui>());

	RC::ServerInput serverInput;
	app->AddService(RC::Server::Create(serverInput));

	RC::WindowInput windowInput;
	app->AddService(RC::Window::Create(windowInput));

	app->Run();
}

