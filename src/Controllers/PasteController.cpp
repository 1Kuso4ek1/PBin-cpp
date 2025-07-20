#include "Controllers/PasteController.hpp"

#include "Models/Pastes.hpp"

#include "Utils/TemplateRenderer.hpp"

#include "Services/ExpiredPastesCleaner.hpp"

namespace Models = drogon_model::pbin;

namespace Controllers
{

void PasteController::paste(const HttpRequestPtr& req, Callback&& callback)
{
    auto& json = *req->getJsonObject();

    static auto mapper = orm::Mapper<Models::Pastes>(app().getDbClient());

    const auto id = utils::base64Encode(utils::getUuid()).substr(0, 8);
    const auto url = std::format("http://{}/paste/{}", req->getHeader("host"), id);
    const auto deleteToken = utils::getUuid();

    Models::Pastes paste;
    paste.setId(id);
    paste.setText(json["text"].asString());
    paste.setLanguage(json["language"].asString());
    paste.setCreated(Date::now());
    paste.setDeletetoken(deleteToken);
    paste.setIpaddress(req->peerAddr().toIp());

    if(json["ttlSeconds"].asInt() > 0)
        paste.setExpires(Date::now().after(json["ttlSeconds"].asInt()));
    else
        paste.setExpiresToNull();

    mapper.insert(paste);

    Json::Value res;
    res["id"] = id;
    res["url"] = url;
    res["deleteToken"] = deleteToken;

    const auto response = HttpResponse::newHttpJsonResponse(res);
    response->setStatusCode(k201Created);

    callback(response);
}

void PasteController::getPaste(const HttpRequestPtr& req, Callback&& callback, const std::string& id)
{
    Services::ExpiredPastesCleaner::cleanup();

    static auto mapper = orm::Mapper<Models::Pastes>(app().getDbClient());

    std::string res;

    auto code = k200OK;

    try
    {
        auto paste = mapper.findOne({ "id", id });
        res = Utils::renderPasteHtml(paste.toJson());

        paste.setViewcount(*paste.getViewcount() + 1);

        mapper.update(paste);
    }
    catch(...)
    {
        code = k404NotFound;
        res = std::format("Error opening paste {}", id);
    }

    const auto response = HttpResponse::newHttpResponse(code, CT_TEXT_HTML);
    response->setBody(res);

    callback(response);
}

void PasteController::getRawPaste(const HttpRequestPtr& req, Callback&& callback, const std::string& id)
{
    Services::ExpiredPastesCleaner::cleanup();

    static auto mapper = orm::Mapper<Models::Pastes>(app().getDbClient());

    std::string res;

    auto code = k200OK;

    try
    {
        const auto paste = mapper.findOne({ "id", id });
        res = *paste.getText();
    }
    catch(...)
    {
        code = k404NotFound;
        res = std::format("Error opening paste {}", id);
    }

    const auto response = HttpResponse::newHttpResponse(code, CT_TEXT_PLAIN);
    response->setBody(res);

    callback(response);
}

void PasteController::getJsonPaste(const HttpRequestPtr& req, Callback&& callback, const std::string& id)
{
    Services::ExpiredPastesCleaner::cleanup();

    static auto mapper = orm::Mapper<Models::Pastes>(app().getDbClient());

    Json::Value res;

    auto code = k200OK;

    try
    {
        const auto paste = mapper.findOne({ "id", id });
        res = paste.toJson();
        res.removeMember("deletetoken");
    }
    catch(...)
    {
        code = k404NotFound;
        res["error"] = std::format("Error opening paste {}", id);
    }

    const auto response = HttpResponse::newHttpJsonResponse(res);
    response->setStatusCode(code);

    callback(response);
}

void PasteController::deletePaste(const HttpRequestPtr& req, Callback&& callback, const std::string& id)
{
    if(!req->headers().contains("x-delete-token"))
    {
        const auto response = HttpResponse::newHttpResponse(k400BadRequest, CT_TEXT_PLAIN);
        response->setBody("Missing delete token");

        callback(response);

        return;
    }

    static auto mapper = orm::Mapper<Models::Pastes>(app().getDbClient());

    const auto token = req->headers().at("x-delete-token");

    try
    {
        const auto n = mapper.deleteBy(
            orm::Criteria{ "id", id }
            && orm::Criteria{ "deletetoken", token }
        );

        if(n == 0)
            throw std::runtime_error("Paste not found");

        const auto response = HttpResponse::newHttpResponse(k200OK, CT_TEXT_PLAIN);
        response->setBody("Paste deleted");

        callback(response);
    }
    catch(...)
    {
        const auto response = HttpResponse::newHttpResponse(k404NotFound, CT_TEXT_PLAIN);
        response->setBody("Paste not found");

        callback(response);
    }
}

}
