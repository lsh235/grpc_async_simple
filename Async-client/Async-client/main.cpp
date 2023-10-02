#include "async_client.h"
ABSL_FLAG(std::string, target, "localhost:50051", "Server address");

int main(int argc, char** argv) {
    absl::ParseCommandLine(argc, argv);
    std::string target_str = absl::GetFlag(FLAGS_target);
    for (int i = 0; i < 10; ++i) {
        std::thread t1([=]() {
            while (true) {
                GreeterClient greeter(
                    grpc::CreateChannel(target_str, grpc::InsecureChannelCredentials()));
                std::string reply = greeter.SayHello("World!");
                std::cout << "[thread : " << i << "] Hello received: " << reply << std::endl;
                std::this_thread::sleep_for(std::chrono::seconds(1));
            }
            });

        std::thread t2([=]() {
            while (true) {
                GreeterClient greeter(
                    grpc::CreateChannel(target_str, grpc::InsecureChannelCredentials()));
                std::string reply = greeter.SayBye("World!");
                std::cout << "[thread : " << i << "] GoogBye received: " << reply << std::endl;
                std::this_thread::sleep_for(std::chrono::seconds(1));
            }
            });
        t1.detach();
        t2.detach();
    }

    while (true) {

    }

    return 0;
}