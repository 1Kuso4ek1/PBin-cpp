#pragma once
#include <drogon/HttpController.h>

using namespace drogon;

using Callback = std::function<void(const HttpResponsePtr&)>;

namespace Controllers
{

class StaticController final : public HttpController<StaticController>
{
public:
    static void swaggerPage(const HttpRequestPtr& req, Callback&& callback);
    static void swaggerFile(const HttpRequestPtr& req, Callback&& callback);

    static void pasteCss(const HttpRequestPtr& req, Callback&& callback);

    static void image(const HttpRequestPtr& req, Callback&& callback, const std::string& path);

public:
    METHOD_LIST_BEGIN

        ADD_METHOD_TO(swaggerPage, "/swagger", Get);
        ADD_METHOD_TO(swaggerFile, "/swagger/swagger.json", Get);

        ADD_METHOD_TO(pasteCss, "/css/paste.css", Get);

        ADD_METHOD_TO(image, "/img/{path}", Get);

    METHOD_LIST_END

};

}
