// Fill out your copyright notice in the Description page of Project Settings.


#include "SocketObject.h"



#include "NetworkConfig.h"
#include "google/protobuf/message.h"
#include "Proto/MessageModels.pb.h"
#include "Network/Handlers/MessageEncoder.h"
#include "google/protobuf/port_def.inc"


FSocket *USocketObject::TCPSocket = nullptr;
TSharedPtr<FInternetAddr> USocketObject::TCPAddress = nullptr;

bool USocketObject::bIsConnected = false;
FSocket *USocketObject::UDPSocket = nullptr;

TSharedPtr<FInternetAddr> USocketObject::UDPAddress = nullptr;
FUdpSocketReceiver *USocketObject::UDPReceiver = nullptr;

int32 USocketObject::TCPLocalPort = 0;
int32 USocketObject::UDPLocalPort = 0;

void USocketObject::InitSocket(FString ServerAddress, int32 TCPLocalP, int32 TCPServerP, int32 UDPLocalP, int32 UDPServerP)
{
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

bool USocketObject::SendByUDP(google::protobuf::Message *message)
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

void USocketObject::Reconnect()
{
    TCPSocket->Close();

    uint32 OutIP;
    TCPAddress->GetIp(OutIP);

    FString ip = FString::Printf(TEXT("%d.%d.%d.%d"), 0xff & (OutIP >> 24), 0xff & (OutIP >> 16), 0xff & (OutIP >> 8), 0xff & OutIP);

    InitSocket(ip, TCPLocalPort, TCPAddress->GetPort(), UDPLocalPort, UDPAddress->GetPort());
}

bool USocketObject::Alive()
{
    std::shared_ptr<Utility> utility(new Utility);
    utility->set_alive(true);
    
    return UMessageEncoder::Send(utility.get(), false, true);
}

void USocketObject::RunUDPSocketReceiver()
{
    GLog->Log("Listening from UDP socket");
    const FTimespan ThreadWaitTime = FTimespan::FromMilliseconds(30);

    UDPReceiver = new FUdpSocketReceiver(UDPSocket, ThreadWaitTime, TEXT("UDP Receiver"));

    UDPReceiver->OnDataReceived().BindStatic(&USocketObject::Recv);
    UDPReceiver->Start();
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