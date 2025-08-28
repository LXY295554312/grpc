#include <iostream>
#include <memory>
#include <string>
#include <grpcpp/grpcpp.h>
#include "calculator.grpc.pb.h"

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;
using calculator::AddRequest;
using calculator::MultiplyRequest;
using calculator::ResultResponse;
using calculator::CalculatorService;

// 客户端类
class CalculatorClient {
 public:
  // 构造函数，接收gRPC通道
  CalculatorClient(std::shared_ptr<Channel> channel)
      : stub_(CalculatorService::NewStub(channel)) {}

  // 调用加法服务
  int Add(int a, int b) {
    AddRequest request;
    request.set_a(a);
    request.set_b(b);

    ResultResponse response;
    ClientContext context;

    // 调用远程方法
    Status status = stub_->Add(&context, request, &response);

    // 检查调用是否成功
    if (status.ok()) {
      return response.result();
    } else {
      std::cout << "RPC调用失败，错误码: " << status.error_code() << std::endl;
      std::cout << "错误信息: " << status.error_message() << std::endl;
      return -1;
    }
  }

  // 调用乘法服务
  int Multiply(int a, int b) {
    MultiplyRequest request;
    request.set_a(a);
    request.set_b(b);

    ResultResponse response;
    ClientContext context;

    // 调用远程方法
    Status status = stub_->Multiply(&context, request, &response);

    // 检查调用是否成功
    if (status.ok()) {
      return response.result();
    } else {
      std::cout << "RPC调用失败，错误码: " << status.error_code() << std::endl;
      std::cout << "错误信息: " << status.error_message() << std::endl;
      return -1;
    }
  }

 private:
  // 客户端Stub
  std::unique_ptr<CalculatorService::Stub> stub_;
};

int main(int argc, char**argv) {
  // 连接服务器
  CalculatorClient client(grpc::CreateChannel(
      "localhost:50051", grpc::InsecureChannelCredentials()));
  
  // 调用加法服务
  int add_result = client.Add(10, 20);
  std::cout << "加法结果: 10 + 20 = " << add_result << std::endl;
  
  // 调用乘法服务
  int multiply_result = client.Multiply(5, 8);
  std::cout << "乘法结果: 5 × 8 = " << multiply_result << std::endl;

  return 0;
}
    