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
        HTTP_GET,
        HTTP_POST,
        HTTP_PUT,
        HTTP_PATCH,
        HTTP_DELETE,
        HTTP_OPTIONS
    };

}  // namespace RC