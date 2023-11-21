#pragma once

#include "rcpch.h"
#include "httplib.h"

namespace RC {
    typedef struct httplib::Request Req;
    typedef struct httplib::Response Res;
    typedef httplib::MultipartFormDataMap MultipartForm;
    typedef httplib::Headers Headers;
    typedef httplib::Server HttpLibServer;

    enum HTTP_METHOD {
        GET,
        POST,
        PUT,
        PATCH,
        DELETE,
        OPTIONS
    };

}  // namespace RC