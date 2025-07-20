#pragma once
#include <drogon/HttpAppFramework.h>

#include <drogon/orm/DbClient.h>
#include <drogon/orm/Mapper.h>

#include "Models/Pastes.hpp"
#include "Utils/Singleton.hpp"

using namespace drogon;

namespace Models = drogon_model::pbin;

namespace Services
{

class ExpiredPastesCleaner
{
public:
    static void run();
    static void cleanup();

private:
    inline static bool running{};
};

}
