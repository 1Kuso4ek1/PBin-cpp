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

    static void index(const HttpRequestPtr& req, Callback&& callback);
    static void created(const HttpRequestPtr& req, Callback&& callback);

public:
    METHOD_LIST_BEGIN

        ADD_METHOD_TO(swaggerPage, "/swagger", Get);
        ADD_METHOD_TO(swaggerFile, "/swagger/swagger.json", Get);

        ADD_METHOD_TO(index, "/", Get);
        ADD_METHOD_TO(created, "/created", Get);

    METHOD_LIST_END

};

}
