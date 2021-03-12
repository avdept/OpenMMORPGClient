// Fill out your copyright notice in the Description page of Project Settings.
// DOC:
// This is a low level class which responsible for establishing communication channels between game client and world server(logic server)



#include "SocketObject.h"

#include "NetworkConfig.h"
#include "WorldStaticManager.h"
#include "google/protobuf/message.h"
#include "Proto/MessageModels.pb.h"
#include "Network/Handlers/MessageEncoder.h"
#include "grpcpp/create_channel.h"
#include "Network/WorldServerEntity.h"
#include "Proto/Healtcheck.grpc.pb.h"
#include "Proto/Server.grpc.pb.h"
#include "Network/GrpcService.h"
#include "Proto/Server.pb.h"


FSocket *USocketObject::TCPSocket = nullptr;
TSharedPtr<FInternetAddr> USocketObject::TCPAddress = nullptr;

bool USocketObject::bIsConnected = false;
FSocket *USocketObject::UDPSocket = nullptr;

std::shared_ptr<grpc::Channel> USocketObject::GRPCChannel = nullptr;

TSharedPtr<FInternetAddr> USocketObject::UDPAddress = nullptr;
FUdpSocketReceiver *USocketObject::UDPReceiver = nullptr;

int32 USocketObject::TCPLocalPort = 0;
int32 USocketObject::UDPLocalPort = 0;

void USocketObject::InitSocket(FString ServerAddress, int32 TCPLocalP, int32 TCPServerP, int32 UDPLocalP, int32 UDPServerP)
{
    const int32 BufferSize = 2 * 1024 * 1024;

    // grpc::InsecureChannelCredentials should be replaced to ssl creds when game goes live
    GRPCChannel = CreateChannel(std::string(TCHAR_TO_UTF8(*NetworkConfig::tcp_server_url)), grpc::InsecureChannelCredentials());

    // We have an option to connect via TCP directly, bypassing http/2 from GRPC. You can uncomment following code
    // to get access to USocketObject::TCPSocket

    //TCPLocalPort = TCPLocalP;
    UDPLocalPort = UDPLocalP;

    //TCPSocket = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateSocket(NAME_Stream, TEXT("TCP_SOCKET"), false);
    //TCPAddress = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();


    //TCPSocket->SetLinger(false);
    FIPv4Address ServerIP;
    FIPv4Address::Parse(ServerAddress, ServerIP);

    // TCP

    //TCPAddress->SetIp(ServerIP.Value);
    //TCPAddress->SetPort(TCPServerP);

    //TCPSocket->Connect(*TCPAddress);
    //TCPSocket->SetNoDelay(true);
    // UDP

    UDPAddress = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();

    FIPv4Address::Parse(ServerAddress, ServerIP);

    UDPAddress->SetIp(ServerIP.Value);
    UDPAddress->SetPort(UDPServerP);


    UDPSocket = FUdpSocketBuilder(TEXT("UDP_SOCKET"))
        .AsReusable()
        .BoundToPort(14000)
        .WithBroadcast()
        .WithReceiveBufferSize(BufferSize)
        .WithSendBufferSize(BufferSize)
        .Build();

    bIsConnected = Alive();
}

bool USocketObject::SendByUDP(google::protobuf::Message *message)
{

    if (UDPSocket == nullptr) return false;

    Wrapper wrapper;

    if (message->GetTypeName() == "Utility")
    {
        Utility *mes = static_cast<Utility*>(message);
        wrapper.set_allocated_utility(mes);
    }


    size_t size = wrapper.ByteSizeLong() + 5;
    uint8_t *buffer = new uint8_t[size];

    google::protobuf::io::ArrayOutputStream arr(buffer, size);
    google::protobuf::io::CodedOutputStream output(&arr);


    output.WriteVarint32(wrapper.ByteSizeLong());

    wrapper.SerializeToCodedStream(&output);

    if (wrapper.has_utility())
    {
        wrapper.release_utility();
    }

    int32 bytesSent = 0;
    const bool sentState = UDPSocket->SendTo(buffer, output.ByteCount(), bytesSent, *UDPAddress);
    delete []buffer;
    return sentState;

}

void USocketObject::Reconnect()
{
    //TCPSocket->Close();
    //uint32 OutIP;
    //TCPAddress->GetIp(OutIP);
    //FString ip = FString::Printf(TEXT("%d.%d.%d.%d"), 0xff & (OutIP >> 24), 0xff & (OutIP >> 16), 0xff & (OutIP >> 8), 0xff & OutIP);
    //InitSocket(ip, TCPLocalPort, TCPAddress->GetPort(), UDPLocalPort, UDPAddress->GetPort());
}

bool USocketObject::ConnectToWorldServer(UWorldServerEntity* Server, int CharacterID, proto_messages::Player* Player)
{
    
    //std::unique_ptr<proto_messages::ServerSession::Stub> stub = proto_messages::ServerSession::NewStub(USocketObject::GRPCChannel);
    //grpc::ClientContext context;
    proto_messages::AuthPacket AuthPacket;
    
    auto lambda = [](std::unique_ptr<proto_messages::ServerSession::Stub> &Stub,
                     grpc::ClientContext *context,
                     proto_messages::AuthPacket Request,
                     proto_messages::Player *Response) ->grpc::Status
    {
        return Stub->JoinServer(context, Request, Response);
    };

    grpc::Status const status = SendRequest<proto_messages::ServerSession,
                proto_messages::AuthPacket,
                proto_messages::Player>(AuthPacket, *Player, lambda);
    return status.ok();
}

bool USocketObject::DisconnectFromWorldServer()
{
    proto_messages::AuthPacket Request;
    proto_messages::Player Player;


    auto lambda = [](std::unique_ptr<proto_messages::ServerSession::Stub> &Stub,
                     grpc::ClientContext *context,
                     proto_messages::AuthPacket Request,
                     proto_messages::Player *Response) ->grpc::Status
    {
        return Stub->LeaveServer(context, Request, Response);
    };

    grpc::Status const status = SendRequest<proto_messages::ServerSession,
                proto_messages::AuthPacket,
                proto_messages::Player>(Request, Player, lambda);
    return status.ok();
    
    /*grpc::Status const status = stub->LeaveServer(&context, Request, &Player);
    if (status.ok())
    {
        if(Player.requestresult())
        {
            GLog->Log("Succesfully logged out");
        } else
        {
            GLog->Log("Error while logging out");
        }
    } else
    {
        
        GLog->Log("Error speaking to server");
    }
    Request.release_token();*/
}    

bool USocketObject::Alive()
{
    std::unique_ptr<utility_messages::HealthcheckService::Stub> stub = utility_messages::HealthcheckService::NewStub(GRPCChannel);
    grpc::ClientContext context;
    utility_messages::HealthcheckResult response;
    utility_messages::HealthCheckParams request;


    grpc::Status status = stub->RunCheck(&context, request, &response);
    if (status.ok())
    {
        GLog->Log("Game Server alive");
        return true;
    }

    GLog->Log("Game Server offline");
    return false;
}

void USocketObject::RunUDPSocketReceiver()
{
    if (UDPSocket == nullptr) return;
    GLog->Log("Listening from UDP socket");
    const FTimespan ThreadWaitTime = FTimespan::FromMilliseconds(30);

    //try {
        UDPReceiver = new FUdpSocketReceiver(UDPSocket, ThreadWaitTime, TEXT("UDP Receiver"));
        UDPReceiver->OnDataReceived().BindStatic(&USocketObject::Recv);
        UDPReceiver->Start();
    //}
    //catch(const std::exception& e)
    //{
        //std::cerr << e.what() << '\n';
    //}
}

void USocketObject::Recv(const FArrayReaderPtr& ArrayReaderPtr, const FIPv4Endpoint& EndPt)
{
    GLog->Log("Received UDP Data");

    uint8_t *buffer = ArrayReaderPtr->GetData();
    size_t size = ArrayReaderPtr->Num();
    GLog->Log("Size of incoming data: " + FString::FromInt(size));

    google::protobuf::io::ArrayInputStream arr(buffer, size);
    google::protobuf::io::CodedInputStream input(&arr);
    std::shared_ptr<Wrapper> wrapper(new Wrapper);
    ReadDelimitedFrom(&input , wrapper.get());

    std::string msg;
    wrapper->SerializeToString(&msg);

    GLog->Log(msg.c_str());
}

bool USocketObject::ReadDelimitedFrom(google::protobuf::io::CodedInputStream *input, google::protobuf::MessageLite *message)
{
    uint32_t size;
    if (!input->ReadVarint32(&size)) return false;

    google::protobuf::io::CodedInputStream::Limit limit = input->PushLimit(size);
    if (!message->MergeFromCodedStream(input)) return false;

    if(!input->ConsumedEntireMessage()) return false;
    input->PopLimit(limit);

    return true;
}

void USocketObject::Shutdown()
{
    //if (TCPSocket != nullptr)
    //{
    //    GLog->Log("Closing sockets");
    //    TCPSocket->Close();
    //    ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->DestroySocket(TCPSocket);
    //    TCPSocket = nullptr;

    //}

    if (UDPSocket != nullptr)
    {
        if (UDPReceiver != nullptr)
        {
            UDPReceiver->Stop();
        }
        FPlatformProcess::Sleep(1.1f);
        UDPSocket->Close();
        GLog->Log("UDP Socket closed");
        delete UDPSocket;
    }
}
