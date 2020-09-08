// Fill out your copyright notice in the Description page of Project Settings.


#include "Network/NetworkSocketManager.h"
#include "OpenMMORPG/Network/NetworkConfig.h"
#include "google/protobuf/message.h"
#include "OpenMMORPG/Network/Proto/MessageModels.pb.h"
#include "Network/Handlers/MessageEncoder.h"

UNetworkSocketManager* UNetworkSocketManager::SocketManagerInstance;

UNetworkSocketManager::UNetworkSocketManager()
{
    bIsConnected = false;
    InitSocket(NetworkConfig::address.c_str(),
               NetworkConfig::tcp_local_port,
               NetworkConfig::tcp_server_port,
               NetworkConfig::udp_local_port,
               NetworkConfig::udp_server_port);
}

UNetworkSocketManager::~UNetworkSocketManager()
{
    GLog->Log("Destroying NetworkSocketManager");
    Shutdown();
}

// ReSharper disable once CppMemberFunctionMayBeConst
bool UNetworkSocketManager::SendByUDP(google::protobuf::Message *message) 
{
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

void UNetworkSocketManager::InitSocket(FString ServerAddress, int32 TCPLocalP, int32 TCPServerP, int32 UDPLocalP, int32 UDPServerP)
{
    GLog->Log("Initializing sockets");
    const int32 BufferSize = 2 * 1024 * 1024;

    TCPLocalPort = TCPLocalP;
    UDPLocalPort = UDPLocalP;

    TCPSocket = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateSocket(NAME_Stream, TEXT("TCP_SOCKET"), false);
    TCPAddress = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();


    TCPSocket->SetLinger(false);
    FIPv4Address ServerIP;
    FIPv4Address::Parse(ServerAddress, ServerIP);

    // TCP

    TCPAddress->SetIp(ServerIP.Value);
    TCPAddress->SetPort(TCPServerP);

    TCPSocket->Connect(*TCPAddress);
    TCPSocket->SetNoDelay(true);
    // UDP

    UDPAddress = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();

    FIPv4Address::Parse(ServerAddress, ServerIP);

    UDPAddress->SetIp(ServerIP.Value);
    UDPAddress->SetPort(UDPServerP);

    UDPSocket = FUdpSocketBuilder(TEXT("UDP_SOCKET2"))  
    .AsReusable()
    .BoundToPort(UDPServerP)
    .WithBroadcast()
    .WithReceiveBufferSize(BufferSize)
    .WithSendBufferSize(BufferSize)
    .Build();

    bIsConnected = Alive();
}

void UNetworkSocketManager::TCPReconnect()
{
    TCPSocket->Close();

    uint32 OutIP;
    TCPAddress->GetIp(OutIP);

    FString ip = FString::Printf(TEXT("%d.%d.%d.%d"), 0xff & (OutIP >> 24), 0xff & (OutIP >> 16), 0xff & (OutIP >> 8), 0xff & OutIP);

    InitSocket(ip, TCPLocalPort, TCPAddress->GetPort(), UDPLocalPort, UDPAddress->GetPort());
}

// ReSharper disable once CppMemberFunctionMayBeStatic
bool UNetworkSocketManager::Alive()
{
    std::shared_ptr<Utility> utility(new Utility);
    utility->set_alive(true);
    
    return UMessageEncoder::Send(utility.get(), false, true);
}

void UNetworkSocketManager::RunUDPSocketReceiver()
{
    GLog->Log("Listening from UDP socket");
    const FTimespan ThreadWaitTime = FTimespan::FromMilliseconds(30);

    UDPReceiver = new FUdpSocketReceiver(UDPSocket, ThreadWaitTime, TEXT("UDP Receiver"));

    UDPReceiver->OnDataReceived().BindStatic(&UNetworkSocketManager::Recv);
    UDPReceiver->Start();
}

void UNetworkSocketManager::Recv(const FArrayReaderPtr& ArrayReaderPtr, const FIPv4Endpoint& EndPt)
{
    GLog->Log("Received UDP Data");

    uint8_t *buffer = ArrayReaderPtr->GetData();
    const size_t size = ArrayReaderPtr->Num();
    GLog->Log("Size of incoming data: " + FString::FromInt(size));

    google::protobuf::io::ArrayInputStream arr(buffer, size);
    google::protobuf::io::CodedInputStream input(&arr);
    std::shared_ptr<Wrapper> wrapper(new Wrapper);
    ReadDelimitedFrom(&input , wrapper.get());

    std::string msg;
    wrapper->SerializeToString(&msg);

    GLog->Log(msg.c_str());
}

// ReSharper disable once CppMemberFunctionMayBeStatic
bool UNetworkSocketManager::ReadDelimitedFrom(google::protobuf::io::CodedInputStream *input, google::protobuf::MessageLite *message)
{
    uint32_t size;
    if (!input->ReadVarint32(&size)) return false;

    const google::protobuf::io::CodedInputStream::Limit limit = input->PushLimit(size);
    if (!message->MergeFromCodedStream(input)) return false;

    if(!input->ConsumedEntireMessage()) return false;
    input->PopLimit(limit);

    return true;
}

void UNetworkSocketManager::Shutdown()
{
    if (TCPSocket != nullptr)
    {
        GLog->Log("Closing sockets");
        TCPSocket->Close();
        ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->DestroySocket(TCPSocket);
        TCPSocket = nullptr;
        
    }

    if (UDPSocket != nullptr)
    {
        if (UDPReceiver != nullptr)
        {
            UDPReceiver->Stop();    
        }
        UDPSocket->Close();
        GLog->Log("UDP Socket closed");
        delete UDPSocket;
    }
}


UNetworkSocketManager* UNetworkSocketManager::GetInstance()
{
    if (SocketManagerInstance)
    {
       return SocketManagerInstance;
   } 
    auto obj = new UNetworkSocketManager();
    SocketManagerInstance = obj;
    return SocketManagerInstance;
}
