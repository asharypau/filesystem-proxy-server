add_executable(
    FileSystemProxy
    "main.cpp"

    "core/server.cpp"
    "core/session.cpp"
    "core/sessionmanager.cpp"

    "models/clientmetadata.h"
    "models/client.h"

    "network/constants.h"
    "network/operationexception.h"
    "network/serialization.h"
    "network/tcp/acceptor.cpp"
    "network/tcp/socket.cpp"

    "services/clientregistry.h"
    "services/clientsubscriptionsmanager.cpp"

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
