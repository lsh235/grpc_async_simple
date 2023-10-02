#include <iostream>
#include <memory>
#include <string>
#include <format>

#include "absl/flags/flag.h"
#include "absl/flags/parse.h"

#include <grpc/support/log.h>
#include <grpcpp/grpcpp.h>

#include "helloworld.grpc.pb.h"

using grpc::Channel;
using grpc::ClientAsyncResponseReader;
using grpc::ClientContext;
using grpc::CompletionQueue;
using grpc::Status;
using helloworld::Greeter;
using helloworld::HelloReply;
using helloworld::HelloRequest;
using helloworld::ByeReply;
using helloworld::ByeRequest;

class GreeterClient {
public:
    explicit GreeterClient(std::shared_ptr<Channel> channel);
    
    // rpc
    std::string SayHello(const std::string& user);

    std::string SayBye(const std::string& user);

private:
    bool status_check(const Status& status);

    void SetDeadline(ClientContext& context, int time = 3);

    std::unique_ptr<Greeter::Stub> stub_;
};