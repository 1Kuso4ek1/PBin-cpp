#pragma once
#include <drogon/HttpController.h>

using namespace drogon;

using Callback = std::function<void(const HttpResponsePtr&)>;

namespace Controllers
{

class PasteController final : public HttpController<PasteController>
{
public:
    static void paste(const HttpRequestPtr& req, Callback&& callback);

    static void getPaste(const HttpRequestPtr& req, Callback&& callback, const std::string& id);
    static void getRawPaste(const HttpRequestPtr& req, Callback&& callback, const std::string& id);
    static void getJsonPaste(const HttpRequestPtr& req, Callback&& callback, const std::string& id);

    static void deletePaste(const HttpRequestPtr& req, Callback&& callback, const std::string& id);

public:
    METHOD_LIST_BEGIN

        ADD_METHOD_TO(paste, "/paste", Post);

        ADD_METHOD_TO(getPaste, "/paste/{id}", Get);
        ADD_METHOD_TO(getRawPaste, "/paste/{id}/raw", Get);
        ADD_METHOD_TO(getJsonPaste, "/paste/{id}/json", Get);

        ADD_METHOD_TO(deletePaste, "/paste/{id}", Delete);

    METHOD_LIST_END

};

}
