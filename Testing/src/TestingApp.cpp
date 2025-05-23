#include <iostream>
#include "R-core.h"
#include <memory>

int main(int argc, char **argv) {
    RC::Application *app = new RC::Application();

    app->AddService(std::make_shared<RC::ApplicationGui>());
    app->AddService(std::make_shared<RC::FileSystemGui>());
    app->AddService(std::make_shared<RC::HttpServer>(2000));

    app->Run();
}
