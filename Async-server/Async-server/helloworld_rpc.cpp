#include "helloworld_rpc.h"

SayHello::SayHello(CallData* data) : data_(data), responder_(&ctx_), status_(REQUEST) {
    on_done = [&](bool ok) { OnDone(ok); };
    proceed = [&](bool ok) { Proceed(ok); };
    ctx_.AsyncNotifyWhenDone(&on_done);
    data_->service_->RequestSayHello(&ctx_, &request_, &responder_, data_->cq,
        data_->cq, &proceed);
}

void SayHello::Proceed(bool ok) {
    switch (status_) {
    case REQUEST: {
        if (!ok) {
            delete this;
            break;
        }
        new SayHello(data_);
        std::string prefix("Hello ");
        response_.set_message(prefix + request_.name());
        responder_.Finish(response_, grpc::Status::OK, &proceed);
        status_ = FINISH;
        break;
    }

    case FINISH:
        finish_called_ = true;
        if (on_done_called_)
            delete this;
        break;
    }
}

void SayHello::OnDone(bool ok) {
    assert(ok);
    if (ctx_.IsCancelled())
        std::cerr << " SayHello cancelled" << std::endl;
    on_done_called_ = true;
    if (finish_called_)
        delete this;
    else
        status_ = FINISH;
}

SayBye::SayBye(CallData* data)
    : data_(data), responder_(&ctx_), status_(REQUEST) {
    on_done = [&](bool ok) { OnDone(ok); };
    proceed = [&](bool ok) { Proceed(ok); };
    ctx_.AsyncNotifyWhenDone(&on_done);
    data_->service_->RequestSayBye(&ctx_, &request_, &responder_, data_->cq,
        data_->cq, &proceed);
}

void SayBye::Proceed(bool ok) {
    switch (status_) {
    case REQUEST: {
        if (!ok) {
            delete this;
            break;
        }
        new SayBye(data_);
        std::string prefix("GoodBye ");
        response_.set_message(prefix + request_.name());
        responder_.Finish(response_, grpc::Status::OK, &proceed);
        status_ = FINISH;
        break;
    }

    case FINISH:
        finish_called_ = true;
        if (on_done_called_)
            delete this;
        break;
    }
}

void SayBye::OnDone(bool ok) {
    assert(ok);
    if (ctx_.IsCancelled())
        std::cerr << " SayBye cancelled" << std::endl;
    on_done_called_ = true;
    if (finish_called_)
        delete this;
    else
        status_ = FINISH;
}