#pragma once
#include "httplib.h"
#include "Services/HttpServer/Helper.h"
#include "Core/Core.h"

namespace RC {

    static std::string DumpHeaders(const Headers &headers) {
        std::string s;
        char buf[BUFSIZ];

        for (const auto &x : headers) {
            snprintf(buf, sizeof(buf), "\t%s: %s\n", x.first.c_str(), x.second.c_str());
            s += buf;
        }
        return s;
    }

    static std::string DumpMultipartFiles(const MultipartForm &files) {
        std::string s;
        char buf[BUFSIZ];
        int iteration = 1;

        for (const auto &x : files) {
            const auto &name = x.first;
            const auto &file = x.second;

            snprintf(buf, sizeof(buf), "Name: %s\n", name.c_str());
            snprintf(buf, sizeof(buf), "Filename: %s\n", file.filename.c_str());
            snprintf(buf, sizeof(buf), "Content type: %s\n", file.content_type.c_str());
            snprintf(buf, sizeof(buf), "Text length: %zu\n", file.content.size());
        }
        return s;
    }

    static void LogRequest(const Req &req, const Res &res) {
        std::string s;
        char buf[1024];

        snprintf(buf, sizeof(buf), "%s %s %s", req.method.c_str(), req.version.c_str(), req.path.c_str());
        s += buf;

        std::string query;
        for (auto it = req.params.begin(); it != req.params.end(); ++it) {
            const auto &x = *it;
            snprintf(buf, sizeof(buf), "%c%s=%s", (it == req.params.begin()) ? '?' : '&', x.first.c_str(), x.second.c_str());
            query += buf;
        }
        snprintf(buf, sizeof(buf), "%s\n", query.c_str());
        s += buf;

        s += DumpHeaders(req.headers);
        s += DumpMultipartFiles(req.files);

        snprintf(buf, sizeof(buf), "%d\n", res.status);
        s += buf;
        s += DumpHeaders(res.headers);

        RC_LOG_DEBUG(s);
    }
}  // namespace RC