#include <brpc/channel.h>
#include <brpc/server.h>
#include <unistd.h>

#include <thread>

#include "common/common.h"
#include "proto/echo.pb.h"

namespace snippets {

void HandleResponse(brpc::Controller *cntl, proto::EchoResponse *response) {
  std::unique_ptr<brpc::Controller> cntl_guard(cntl);
  std::unique_ptr<proto::EchoResponse> resp_guard(response);
  if (cntl->Failed()) {
    LOG(ERROR) << "Failed to send echo request " << cntl->ErrorText();
    return;
  }
  LOG(INFO) << "Received response from " << cntl->remote_side() << " message "
            << response->message()
            << " (attached=" << cntl->response_attachment() << ")"
            << " latency=" << cntl->latency_us() << "us";
}

class EchoServiceImp : public proto::EchoService {
 public:
  void Echo(::google::protobuf::RpcController *controller,
            const ::snippets::proto::EchoRequest *request,
            ::snippets::proto::EchoResponse *response,
            ::google::protobuf::Closure *done) {
    brpc::ClosureGuard done_guard(done);
    brpc::Controller *b_controller =
        static_cast<brpc::Controller *>(controller);
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

  ASSERT_EQ(server.AddService(&service, brpc::SERVER_DOESNT_OWN_SERVICE), 0);

  brpc::ServerOptions options;

  ASSERT_EQ(server.Start(43210, &options), 0);

  auto pid = fork();

  std::this_thread::sleep_for(std::chrono::milliseconds(100));

  LOG(INFO) << "server done";
}

TEST(Brpc, TestClient) {
  brpc::Channel chan;

  brpc::ChannelOptions options;
  options.timeout_ms = 100;
  options.max_retry = 3;
  options.protocol = "baidu_std";

  ASSERT_EQ(chan.Init("0.0.0.0:43210", &options), 0);

  proto::EchoService_Stub echo_stub(&chan);

  auto response = new proto::EchoResponse();
  auto cntl = new brpc::Controller();
  proto::EchoRequest request;
  request.set_message("hello");

  auto done = brpc::NewCallback(&HandleResponse, cntl, response);
  echo_stub.Echo(cntl, &request, response, done);
}

}  // namespace snippets
