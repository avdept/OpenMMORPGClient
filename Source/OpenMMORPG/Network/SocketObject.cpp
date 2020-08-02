// Fill out your copyright notice in the Description page of Project Settings.


#include "SocketObject.h"


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