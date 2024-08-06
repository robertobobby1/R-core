#pragma once
#include "rcpch.h"

#include "Core/Core.h"
#include "Services/HttpServer/Helper.h"

#include "Services/HttpServer/Middleware.h"

namespace RC {

    struct Route {
        HTTP_METHOD m_method;
        std::string m_path;
        std::function<void(const Req &, const Res &)> m_callback;
        std::vector<std::shared_ptr<Middleware>> m_middlewares;

        void inline AddMiddleware(std::shared_ptr<Middleware> middleware) { m_middlewares.push_back(middleware); }
    };

    class Controller {
       public:
        Controller() = default;
        ~Controller() = default;

        void inline AddRoute(Route route) { m_routes.push_back(route); }

        void ControllerCallback(const Req &req, const Res &res, Route route) {
            // controller middlewares first
            this->RunMiddlewares(req, res, m_controllerMiddlewares);
            // route middlewares later
            this->RunMiddlewares(req, res, route.m_middlewares);
            // finally run the route itself
            route.m_callback(req, res);
        }

        void RunMiddlewares(const Req &req, const Res &res, std::vector<std::shared_ptr<Middleware>> middlewares) {
            std::map<std::string, std::string> executedMiddlewares;
            for (auto &routeMiddleware : middlewares) {
                auto middlewareName = routeMiddleware->GetChildClassName();
                // doesn't find the middleware so it hasn't run yet
                if (executedMiddlewares.find(middlewareName) == executedMiddlewares.end()) {
                    routeMiddleware->Run(req, res);
                    executedMiddlewares[middlewareName] = "Exists";
                }
            }
        }

        std::vector<Route> m_routes;
        std::vector<std::shared_ptr<Middleware>> m_controllerMiddlewares;
    };
}  // namespace RC