#pragma once

#include "Core/Service.h"
#include "Services/HttpServer/Helper.h"
#include "Services/HttpServer/Controller.h"

#include "httplib.h"

namespace RC {

    // Http servers
    class HttpServer : public Service {
       public:
        HttpServer(unsigned int port);
        ~HttpServer() = default;

        virtual void Init() override;
        virtual void Run() override;

        void HttpServer::AddController(
            std::shared_ptr<Controller> controller, std::vector<std::shared_ptr<Middleware>> middlewares = {}
        ) {
            this->m_controllers.push_back(controller);
        }

        void SubscribeRoute(std::shared_ptr<Controller> controller, Route route);
        void RunInterceptors(const Req &req, const Res &res, INTERCEPTOR_TYPE type);

       private:
        std::vector<std::shared_ptr<Controller>> m_controllers;
        std::shared_ptr<httplib::Server> m_server;
        std::vector<std::shared_ptr<Interceptor>> m_interceptors;

        unsigned int m_port;
    };
}  // namespace RC