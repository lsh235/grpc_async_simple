#include "helloworld.grpc.pb.h"

struct CallData {
    helloworld::Greeter::AsyncService* service_;
    grpc::ServerCompletionQueue* cq;
};

class Call {
public:
    virtual void Proceed(bool ok) = 0;
};

class SayHello final : public Call {
public:
    explicit SayHello(CallData* data);

    void Proceed(bool ok);

    void OnDone(bool ok);

    std::function<void(bool)> proceed;
    std::function<void(bool)> on_done;

private:
    CallData* data_;
    grpc::ServerContext ctx_;
    helloworld::HelloRequest request_;
    helloworld::HelloReply response_;
    grpc::ServerAsyncResponseWriter<helloworld::HelloReply> responder_;
    enum CallStatus { REQUEST, FINISH };
    CallStatus status_;
    bool finish_called_ = false;
    bool on_done_called_ = false;
};

class SayBye final : public Call {
public:
    explicit SayBye(CallData* data);

    void Proceed(bool ok);

    void OnDone(bool ok);

    std::function<void(bool)> proceed;
    std::function<void(bool)> on_done;

private:
    CallData* data_;
    grpc::ServerContext ctx_;
    helloworld::ByeRequest request_;
    helloworld::ByeReply response_;
    grpc::ServerAsyncResponseWriter<helloworld::ByeReply> responder_;
    enum CallStatus { REQUEST, FINISH };
    CallStatus status_;
    bool finish_called_ = false;
    bool on_done_called_ = false;
};