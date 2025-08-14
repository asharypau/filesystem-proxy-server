#include <core/server.h>

int main(int argc, char* argv[])
{
    return Server(8080, "local").run();
}
