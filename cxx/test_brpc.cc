#include <brpc/channel.h>
#include <brpc/server.h>
#include <unistd.h>

#include <condition_variable>
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
    VLOG(10) << "request " << request->ShortDebugString()
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

  std::this_thread::sleep_for(std::chrono::milliseconds(100));

  LOG(INFO) << "server done";
}

class BrpcClosure : public ::google::protobuf::Closure {
 public:
  static BrpcClosure *New(google::protobuf::Closure *closure) {
    return new BrpcClosure(closure);
  }

  void Run() override {
    closure_->Run();
    std::unique_lock<std::mutex> ul(mu_);
    done_ = true;
    cv_.notify_all();
  }

  void Wait() {
    std::unique_lock<std::mutex> ul(mu_);
    if (!done_) {
      cv_.wait(ul);
    }
    std::unique_ptr<BrpcClosure>(this);
  }

 private:
  explicit BrpcClosure(google::protobuf::Closure *closure)
      : closure_(closure) {}

  std::mutex mu_;
  std::condition_variable cv_;
  bool done_{false};

  google::protobuf::Closure *closure_;
};

TEST(Brpc, TestClient) {
  brpc::Server server;
  EchoServiceImp service;
  std::atomic_bool started{false};
  auto server_thread = std::thread([&]() {
    ASSERT_EQ(server.AddService(&service, brpc::SERVER_DOESNT_OWN_SERVICE), 0);
    brpc::ServerOptions options;
    ASSERT_EQ(server.Start(43210, &options), 0);
    started.store(true);
    server.Join();
  });
  while (!started.load()) {
  }

  brpc::ChannelOptions options;
  options.timeout_ms = 100;
  options.max_retry = 3;
  options.protocol = "baidu_std";
  brpc::Channel chan;
  ASSERT_EQ(chan.Init("0.0.0.0:43210", &options), 0);
  proto::EchoService_Stub echo_stub(&chan);

  auto response = new proto::EchoResponse();
  auto cntl = new brpc::Controller();
  proto::EchoRequest request;
  request.set_message("hello");

  auto done =
      BrpcClosure::New(brpc::NewCallback(&HandleResponse, cntl, response));
  echo_stub.Echo(cntl, &request, response, done);
  done->Wait();

  ASSERT_EQ(server.Stop(0), 0);
  if (server_thread.joinable()) {
    server_thread.join();
  }
}

class TestBrpc : public testing::Test {
 public:
  void SetUp() {
    server_ = std::make_unique<brpc::Server>();
    service_ = std::make_unique<EchoServiceImp>();
    CHECK_EQ(
        server_->AddService(service_.get(), brpc::SERVER_DOESNT_OWN_SERVICE),
        0);
    server_opts_.num_threads = 0;
    server_opts_.max_concurrency = 0;
    CHECK_EQ(server_->Start(port_, &server_opts_), 0);
  }

  void TearDown() {}

  std::shared_ptr<proto::EchoService_Stub> GetOrCreateStub() {
    std::lock_guard<std::mutex> guard(mtx_);
    if (channel_ == nullptr) {
      channel_ = std::make_unique<brpc::Channel>();
      channel_opts_.timeout_ms = 30;
      channel_opts_.max_retry = 3;
      channel_opts_.protocol = "baidu_std";
      CHECK_EQ(channel_->Init(server_ip_port_.c_str(), &channel_opts_), 0);
    }
    return std::make_shared<proto::EchoService_Stub>(channel_.get());
  }

 protected:
  std::unique_ptr<brpc::Server> server_{nullptr};
  std::unique_ptr<proto::EchoService> service_{nullptr};
  brpc::ServerOptions server_opts_;

  std::mutex mtx_;
  std::unique_ptr<brpc::Channel> channel_{nullptr};
  brpc::ChannelOptions channel_opts_;

  std::string server_ip_port_ = "0.0.0.0:43210";
  int port_ = 43210;
};

TEST_F(TestBrpc, Hello) {
  auto stub = GetOrCreateStub();
  for (auto i = 0; i < 10; ++i) {
    proto::EchoRequest request;
    request.set_message("hello, I'm from client");
    auto cntl = new brpc::Controller();
    auto resp = new proto::EchoResponse();
    auto done = brpc::NewCallback(&HandleResponse, cntl, resp);
    stub->Echo(cntl, &request, resp, done);
  }
  std::this_thread::sleep_for(std::chrono::milliseconds(100));
}

class TestBrpc : public testing::Test {
 public:
  void SetUp() {
    server_ = std::make_unique<brpc::Server>();
    service_ = std::make_unique<EchoServiceImp>();
    CHECK_EQ(
        server_->AddService(service_.get(), brpc::SERVER_DOESNT_OWN_SERVICE),
        0);
    server_opts_.num_threads = 0;
    server_opts_.max_concurrency = 0;
    CHECK_EQ(server_->Start(port_, &server_opts_), 0);
  }

  void TearDown() {}

  std::shared_ptr<proto::EchoService_Stub> GetOrCreateStub() {
    std::lock_guard<std::mutex> guard(mtx_);
    if (channel_ == nullptr) {
      channel_ = std::make_unique<brpc::Channel>();
      channel_opts_.timeout_ms = 30;
      channel_opts_.max_retry = 3;
      channel_opts_.protocol = "baidu_std";
      CHECK_EQ(channel_->Init(server_ip_port_.c_str(), &channel_opts_), 0);
    }
    return std::make_shared<proto::EchoService_Stub>(channel_.get());
  }

 protected:
  std::unique_ptr<brpc::Server> server_{nullptr};
  std::unique_ptr<proto::EchoService> service_{nullptr};
  brpc::ServerOptions server_opts_;

  std::mutex mtx_;
  std::unique_ptr<brpc::Channel> channel_{nullptr};
  brpc::ChannelOptions channel_opts_;

  std::string server_ip_port_ = "0.0.0.0:43210";
  int port_ = 43210;
};

TEST_F(TestBrpc, Hello) {
  auto stub = GetOrCreateStub();
  for (auto i = 0; i < 10; ++i) {
    proto::EchoRequest request;
    request.set_message("hello, I'm from client");
    auto cntl = new brpc::Controller();
    auto resp = new proto::EchoResponse();
    auto done = brpc::NewCallback(&HandleResponse, cntl, resp);
    stub->Echo(cntl, &request, resp, done);
  }
  std::this_thread::sleep_for(std::chrono::milliseconds(100));
}

}  // namespace snippets
