#include <iostream>
#include "R-core.h"
#include <memory>

int main(int argc, char** argv)
{
	RC::Application* app = new RC::Application();

	RC::ServerInput serverInput;
	app->AddService(RC::Server::Create(serverInput));

	app->AddService(std::make_shared<RC::ServerGui>());

	RC::WindowInput windowInput;
	app->AddService(RC::Window::Create(windowInput));

	app->Run();
}

