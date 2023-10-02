#include "async_client.h"

GreeterClient::GreeterClient(std::shared_ptr<Channel> channel)
    :stub_(Greeter::NewStub(channel)) {
}

std::string GreeterClient::SayHello(const std::string& user) {
    HelloRequest request;
    request.set_name(user);
    ClientContext context_;
    SetDeadline(context_);

    HelloReply reply;

    CompletionQueue cq;

    Status status;

    std::unique_ptr<ClientAsyncResponseReader<HelloReply> > rpc(
        stub_->AsyncSayHello(&context_, request, &cq));

    rpc->Finish(&reply, &status, (void*)1);
    void* got_tag;
    bool ok = false;
    GPR_ASSERT(cq.Next(&got_tag, &ok));

    GPR_ASSERT(got_tag == (void*)1);
    GPR_ASSERT(ok);

    if (status_check(status)) {
        return reply.message();
    }
    else {
        return "RPC failed";
    }
}

std::string GreeterClient::SayBye(const std::string& user) {
    ByeRequest request;
    request.set_name(user);
    ClientContext context_;
    SetDeadline(context_);

    ByeReply reply;
    CompletionQueue cq;
    Status status;
    std::unique_ptr<ClientAsyncResponseReader<ByeReply>> rpc(stub_->AsyncSayBye(&context_, request, &cq));

    rpc->Finish(&reply, &status, (void*)1);
    void* got_tag;
    bool ok = false;
    GPR_ASSERT(cq.Next(&got_tag, &ok));
    GPR_ASSERT(got_tag == (void*)1);
    GPR_ASSERT(ok);
    if (status_check(status)) {
        return reply.message();
    }
    else {
        return "RPC failed";
    }
}

bool GreeterClient::status_check(const Status& status) {
    if (status.ok()) {
        return true;
    }
    else {
        if (status.error_code() == grpc::DEADLINE_EXCEEDED) {
            std::cerr << "RPC call canceled due to deadline exceeded." << std::endl;
        }
        return false;
    }
}

void GreeterClient::SetDeadline(ClientContext& context, int time) {
    std::chrono::system_clock::time_point deadline =
        std::chrono::system_clock::now() + std::chrono::seconds(time);
    context.set_deadline(deadline);
}
