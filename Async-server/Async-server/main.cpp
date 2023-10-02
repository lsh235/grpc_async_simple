#include "async_server.h"

int main(int argc, char** argv) {
    int threads = 2;
    AsyncServer server(threads);
    server.Init(50051);
    server.Run();

    return 0;
}