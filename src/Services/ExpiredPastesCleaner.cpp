#include "Services/ExpiredPastesCleaner.hpp"

#include <trantor/utils/Date.h>

using trantor::Date;

namespace Services
{

void ExpiredPastesCleaner::run()
{
    if(running)
        return;

    running = true;

    app().getLoop()->runEvery(600.0, [] { cleanup(); });
}

void ExpiredPastesCleaner::cleanup()
{
    auto mapper = orm::Mapper<Models::Pastes>(app().getDbClient());
    mapper.deleteBy({ "expires", orm::CompareOperator::LE, Date::now().toDbStringLocal() });
}

}
