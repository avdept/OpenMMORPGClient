// Fill out your copyright notice in the Description page of Project Settings.


#include "OpenMMORPG/Public/Network/NetworkInterface.h"



#include "OpenMMORPG/Network/NetworkConfig.h"
#include "ThirdParty/grpc/include/grpcpp/create_channel.h"
#include "OpenMMORPG/Public/Network/Handlers/MessageEncoder.h"
#include "OpenMMORPG/Network/Proto/MessageModels.pb.h"
#include "OpenMMORPG/Network/Proto/User.grpc.pb.h"



void UNetworkInterface::SendTCPMessage()
{
    std::shared_ptr<Registration> registration(new Registration);
    std::string login = "TestFromTCP";
    std::string email = "alex@test22.com";
    registration->set_login(login);
    registration->set_mail(email);

    GLog->Log("Sending TCP connection request");
    UMessageEncoder::Send(registration.get(), false, true);
}

void UNetworkInterface::SendUDPMessage()
{

    std::shared_ptr<Registration> registration(new Registration);
    std::string login = "TestFromUDP";
    std::string email = "alex@test22.com";
    registration->set_login(login);
    registration->set_mail(email);

    GLog->Log("Sending UDP connection request");
    UMessageEncoder::Send(registration.get(), false, false);
}

void UNetworkInterface::SendGRCPMessage()
{
    
    
    auto channel = grpc::CreateChannel(std::string(TCHAR_TO_UTF8(*NetworkConfig::tcp_server_url)), grpc::InsecureChannelCredentials());
    auto stub = UserService::NewStub(channel);
    grpc::ClientContext context;
    UserResponse response;
    UserRequest request;
    request.set_alive(false);

    grpc::Status status = stub->CheckAlive(&context, request, &response);
    if (status.ok())
    {
        GLog->Log("Success");
    } else
    {
        GLog->Log("Error");   
    }
}
