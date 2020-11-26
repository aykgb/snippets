#include "common/common.h"
#include "echo.pb.h"

namespace snippets {

TEST(Proto, EchoTest) {
  snippets::EchoRequest request;
  snippets::EchoResponse response;
  request.set_message("hello");
  response.set_message("hi");
  LOG(INFO) << "request " << request.ShortDebugString();
  LOG(INFO) << "response " << response.ShortDebugString();
}

}  // namespace snippets