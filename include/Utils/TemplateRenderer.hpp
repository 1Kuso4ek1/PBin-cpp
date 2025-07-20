#pragma once
#include <json/json.h>

#include <trantor/utils/Date.h>

#include <format>
#include <fstream>
#include <ranges>
#include <sstream>

using trantor::Date;

namespace Utils
{

inline constexpr auto pathToTemplate = "../static/html/paste.html";

inline std::string escapeHtml(const std::string& str)
{
    static const auto escape = [](const char c) -> std::string
    {
        switch(c)
        {
        case '&': return "&amp;";
        case '<': return "&lt;";
        case '>': return "&gt;";
        case '"': return "&quot;";
        default: return { 1, c };
        }
    };

    std::ostringstream oss;
    for(const auto i : str | std::views::transform(escape))
        oss << i;

    return oss.str();
}

inline std::string renderPasteHtml(const Json::Value& paste)
{
    const std::string templateHtml{
        std::istreambuf_iterator(std::ifstream(pathToTemplate).rdbuf()),
        std::istreambuf_iterator<char>()
    };

    auto id = paste["id"].asString();
    auto lang = paste["language"].asString();
    auto text = escapeHtml(paste["text"].asString());
    auto created = Date::fromDbString(paste["created"].asString())
                        .toCustomFormattedString("%d.%m.%Y %H:%M");

    int viewCount = paste["viewcount"].asInt();

    return std::vformat(
        templateHtml,
        std::make_format_args(id, lang, text, viewCount, created)
    );
}

}
