#include <iostream>
#include <memory>
#include <string>
#include <thread>

#include "absl/flags/flag.h"
#include "absl/flags/parse.h"
#include "absl/strings/str_format.h"

#include <grpc/support/log.h>
#include <grpcpp/grpcpp.h>

#include "helloworld.grpc.pb.h"
#include "async_server.h"
#include "helloworld_rpc.h"

std::atomic<bool> shutdown(false);
ABSL_FLAG(uint16_t, port, 50051, "Server port for the service");

AsyncServer::AsyncServer(const int num_threads)
    : num_threads_(num_threads) {
}

AsyncServer::~AsyncServer() {
    std::cerr << "Shutting down..." << std::endl;
    for (auto cq = cqs_.begin(); cq != cqs_.end(); ++cq)
        (*cq)->Shutdown();
    void* tag;
    bool ok;
    for (auto cq = cqs_.begin(); cq != cqs_.end(); ++cq) {
        while ((*cq)->Next(&tag, &ok)) {
            auto proceed = static_cast<std::function<void(bool)>*>(tag);
            (*proceed)(ok);
        }
    }
}

void AsyncServer::Init(const int port) {
    std::string server_address = absl::StrFormat("0.0.0.0:%d", port);

    grpc::ServerBuilder builder;
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    builder.RegisterService(&service_);
    for (int i = 0; i < num_threads_; i++)
        cqs_.emplace_back(builder.AddCompletionQueue());
    server_ = builder.BuildAndStart();
    std::cout << "Server listening on " << server_address << std::endl;
}

void AsyncServer::Run() {
    std::vector<std::thread> threads;
    for (int i = 0; i < num_threads_; i++) {
        CallData data{ &service_, cqs_[i].get() };
        new SayHello(&data);
        new SayBye(&data);
        threads.emplace_back(std::thread(&AsyncServer::ServeThread, this, i));
    }
    while (true) {
    }
}

void AsyncServer::ServeThread(int i) {
    void* tag;
    bool ok;
    while (cqs_[i]->Next(&tag, &ok)) {
        auto proceed = static_cast<std::function<void(bool)>*>(tag);
        (*proceed)(ok);
        if (shutdown.load())
            break;
    }
}