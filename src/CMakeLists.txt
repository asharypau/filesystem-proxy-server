add_executable(
    FileSystemProxy
    "main.cpp"

    "core/server.h" "core/server.cpp"
    "core/session.h" "core/session.cpp"
    "core/sessionmanager.h" "core/sessionmanager.cpp"

    "models/client.h"
    "models/clientinput.h"
    "models/clientpayload.h"
    "models/clientcontext.h"
    "models/subscriber.h"

    "network/constants.h"
    "network/requesttype.h"
    "network/headerdraft.h"
    "network/headerpackage.h"
    "network/operationexception.h"
    "network/serialization.h"
    "network/tcp/acceptor.h" "network/tcp/acceptor.cpp"
    "network/tcp/socket.h" "network/tcp/socket.cpp"

    "services/clientregistry.h"
    "services/iclientregisterobserver.h"
    "services/clientmapper.h" "services/clientmapper.cpp"
    "services/clientsubscriptionsmanager.h" "services/clientsubscriptionsmanager.cpp"

    "utils/colors.h"
    "utils/datetimeprovider.h"
    "utils/logger.h"
 "network/headerpackage.h" "network/requesttype.h" "network/headerdraft.h")

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
