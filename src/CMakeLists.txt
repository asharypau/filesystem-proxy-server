add_executable(
    FileSystemProxy
    "main.cpp"

    "core/dispatcher.h" "core/dispatcher.cpp"
    "core/server.h" "core/server.cpp"
    "core/session.h" "core/session.cpp"
    "core/sessionmanager.h" "core/sessionmanager.cpp"

    "handlers/ihandler.h"
    "handlers/clienthandler.h" "handlers/clienthandler.cpp"

    "models/client.h"
    "models/subscriber.h"

    "network/protocol.h" "network/protocol.cpp"
    "network/protocoltypes.h"
    "network/iclientconnection.h"
    "network/operationexception.h"
    "network/serialization.h"
    "network/tcp/acceptor.h" "network/tcp/acceptor.cpp"
    "network/tcp/socket.h" "network/tcp/socket.cpp"

    "services/clientactivator.h" "services/clientactivator.cpp"
    "services/clientmapper.h" "services/clientmapper.cpp"
    "services/clientregistry.h"
    "services/iclientregisterobserver.h"
    "services/clientsubscriptionsmanager.h" "services/clientsubscriptionsmanager.cpp"
    "services/idgenerator.h" "services/idgenerator.cpp"

    "utils/colors.h"
    "utils/datetimeprovider.h"
    "utils/logger.h"
)

target_include_directories(
    FileSystemProxy PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${Boost_INCLUDE_DIRS}
)

target_link_libraries(
    FileSystemProxy PRIVATE
    Boost::system
    Boost::beast
    Boost::asio
)
