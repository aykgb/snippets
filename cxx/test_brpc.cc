#include <brpc/server.h>

#include <thread>

#include "common/common.h"
#include "echo.pb.h"

namespace snippets {

class EchoServiceImp : public proto::EchoService {
 public:
  void Echo(::google::protobuf::RpcController* controller,
            const ::snippets::proto::EchoRequest* request,
            ::snippets::proto::EchoResponse* response,
            ::google::protobuf::Closure* done) {
    brpc::ClosureGuard done_guard(done);
    brpc::Controller* b_controller = static_cast<brpc::Controller*>(controller);
    auto remote = b_controller->remote_side();
    LOG(INFO) << "request " << request->ShortDebugString()
              << " received from client"
              << std::string(butil::ip2str(remote.ip).c_str()) << ":"
              << remote.port;
    std::string resp_msg = "response from " + std::string(butil::my_ip_cstr());
    response->set_message(resp_msg.c_str());
  }
};

TEST(Brpc, TestServer) {
  brpc::Server server;

  EchoServiceImp service;

  ASSERT_TRUE(server.AddService(&service, brpc::SERVER_DOESNT_OWN_SERVICE) ==
              0);

  brpc::ServerOptions options;

  ASSERT_TRUE(server.Start(43210, &options) == 0);

  std::this_thread::sleep_for(std::chrono::milliseconds(1000));

  LOG(INFO) << "server done";
}

}  // namespace snippets
