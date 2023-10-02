#include <memory>
#include <string>
#include <thread>
#include <vector>

#include "helloworld.grpc.pb.h"

class AsyncServer final {
public:
    AsyncServer(int num_threads);
    ~AsyncServer();

    void Init(const int port);

    void Run();

private:
    void ServeThread(int i);

    helloworld::Greeter::AsyncService service_;
    std::unique_ptr<grpc::Server> server_;
    std::vector<std::unique_ptr<grpc::ServerCompletionQueue>> cqs_;
    std::unique_ptr<grpc::ServerCompletionQueue> migrate_cq_;
    void* call_ = nullptr;
    std::thread watcher_;
    int num_threads_;
};
