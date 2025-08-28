#include <iostream>
#include <memory>
#include <string>
#include <grpcpp/grpcpp.h>
#include "calculator.grpc.pb.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;
using calculator::AddRequest;
using calculator::MultiplyRequest;
using calculator::ResultResponse;
using calculator::CalculatorService;

// 实现服务接口
class CalculatorServiceImpl final : public CalculatorService::Service {

  Status Add(ServerContext* context, const AddRequest* request,
             ResultResponse* response) override {
    int result = request->a() + request->b();
    std::cout << "收到加法请求: " << request->a() << " + " << request->b() 
              << " = " << result << std::endl;
    response->set_result(result);
    return Status::OK;
  }

  Status Multiply(ServerContext* context, const MultiplyRequest* request,
                  ResultResponse* response) override {
    int result = request->a() * request->b();
    std::cout << "收到乘法请求: " << request->a() << " × " << request->b() 
              << " = " << result << std::endl;
    response->set_result(result);
    return Status::OK;
  }
  
};

// 启动服务器
void RunServer() {
  std::string server_address("0.0.0.0:50051");
  CalculatorServiceImpl service;

  ServerBuilder builder;
  // 监听指定端口，不使用TLS加密
  builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
  // 注册服务实现
  builder.RegisterService(&service);
  // 构建并启动服务器
  std::unique_ptr<Server> server(builder.BuildAndStart());
  std::cout << "服务器已启动，监听端口: " << server_address << std::endl;

  // 等待服务器关闭（阻塞）
  server->Wait();
}

int main(int argc, char**argv) {
  RunServer();
  return 0;
}
    