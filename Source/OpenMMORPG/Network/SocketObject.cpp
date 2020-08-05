// Fill out your copyright notice in the Description page of Project Settings.


#include "SocketObject.h"



#include "NetworkConfig.h"
#include "google/protobuf/message.h"
#include "Proto/MessageModels.pb.h"
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
    TCPAddress = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM) ->CreateInternetAddr();

    FIPv4Address ServerIP;
    FIPv4Address::Parse(ServerAddress, ServerIP);

    TCPAddress->SetIp(ServerIP.Value);
    TCPAddress->SetPort(TCPServerP);

    TCPSocket->Connect(*TCPAddress);

    bIsConnected = Alive();

    // UDP

    UDPAddress = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM) -> CreateInternetAddr();

    FIPv4Address::Parse(ServerAddress, ServerIP);

    UDPAddress->SetIp(ServerIP.Value);
    UDPAddress->SetPort(UDPServerP);

    UDPSocket = FUdpSocketBuilder("UDP_SOCKET").AsReusable().BoundToPort(UDPLocalPort).WithBroadcast().WithReceiveBufferSize(BufferSize).WithSendBufferSize(BufferSize).Build();
}

bool USocketObject::Alive()
{
    return false;
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

    //auto deb = BytesFromString
    std::cout << &buffer << std::endl;

    //UE_LOG(LogTemp, Log, TEXT("%s"), outputString);
    const bool sentState = UDPSocket->SendTo(buffer, output.ByteCount(), bytesSent, *UDPAddress);


    delete []buffer;

    return sentState;
    
}

void USocketObject::RunUDPSocketReceiver()
{
    GLog->Log("Listening from UDP socket");
    FTimespan ThreadWaitTime = FTimespan::FromMilliseconds(30);

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

USocketObject::~USocketObject()
{
    if (TCPSocket != nullptr || UDPSocket != nullptr)
    {
        TCPSocket->Close();
        //UDPSocket->Close();

        delete TCPSocket;
        delete UDPSocket;
    }
}