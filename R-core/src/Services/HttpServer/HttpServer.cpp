#include "rcpch.h"
#include "Services/HttpServer/HttpServer.h"
#include "Services/HttpServer/HttpLogging.h"

#include <mysqlx/xdevapi.h>

namespace RC {

    HttpServer::HttpServer(unsigned int port)
        : Service(), m_port(port) {}

    void HttpServer::Init(){};

    void HttpServer::Run() {
        m_server = std::make_shared<httplib::Server>();

        m_server->set_logger([this](const Req& req, const Res& res) {
            std::string s = fmt::format("{}-Log", this->GetChildClassName()).c_str();
            LogFormatter::AddThreadName(RC_THREAD_ID(), s.c_str());

            LogRequest(req, res);
        });

        // subscribe all routes and its callbacks, including interceptors and middlewares
        for (auto& controller : this->m_controllers) {
            for (auto& route : controller->m_routes) {
                SubscribeRoute(controller, route);
            }
        }

        m_server->listen("localhost", m_port);
    };

    void HttpServer::SubscribeRoute(std::shared_ptr<Controller> controller, Route route) {
        auto callback = [controller, route, this](const Req& req, const Res& res) {
            // run request interceptors
            this->RunInterceptors(req, res, REQUEST);
            // in all cases call same callback, controller will take care of middlewares
            controller->ControllerCallback(req, res, route);
            // run response interceptors
            this->RunInterceptors(req, res, RESPONSE);
        };

        switch (route.m_method) {
            case GET: m_server->Get(route.m_path, callback); break;
            case POST: m_server->Post(route.m_path, callback); break;
            case DELETE: m_server->Delete(route.m_path, callback); break;
            case PATCH: m_server->Patch(route.m_path, callback); break;
            case PUT: m_server->Put(route.m_path, callback); break;
            case OPTIONS: m_server->Options(route.m_path, callback); break;
            default: RC_LOG_WARN("Unknown Http method, skipping route subscription");
        }
    }

    void HttpServer::RunInterceptors(const Req& req, const Res& res, INTERCEPTOR_TYPE type) {
        std::map<std::string, std::string> executedInterceptors;
        for (auto& interceptor : this->m_interceptors) {
            auto interceptorName = interceptor->GetChildClassName();
            // doesn't find the middleware so it hasn't run yet
            if (executedInterceptors.find(interceptorName) == executedInterceptors.end()) {
                if (interceptor->m_type != type) {
                    continue;
                }

                interceptor->Run(req, res);
                executedInterceptors[interceptorName] = "Exists";
            }
        }
    }

}  // namespace RC