// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Networking.h"
#include "SocketSubsystem.h"
#include "google/protobuf/message.h"
#include "grpcpp/create_channel.h"

#include "SocketObject.generated.h"

/**
 * 
 */
UCLASS()
class OPENMMORPG_API USocketObject : public UObject
{
	GENERATED_BODY()

public:
	static FSocket *TCPSocket;

	// TCP
	static TSharedPtr<FInternetAddr> TCPAddress;
	static bool bIsConnected;
	static void Shutdown();
	static bool Alive();
	static bool SendByUDP(google::protobuf::Message* message);
	static void RunUDPSocketReceiver();
	static void Reconnect();
	static bool ReadDelimitedFrom(google::protobuf::io::CodedInputStream* input, google::protobuf::MessageLite* message);
	static void Recv(const FArrayReaderPtr& ArrayReaderPtr, const FIPv4Endpoint& EndPt);


	static std::shared_ptr<grpc::Channel>  GRPCChannel;

	//UDP

	static FSocket *UDPSocket;

	static TSharedPtr<FInternetAddr> UDPAddress;

	static FUdpSocketReceiver *UDPReceiver;

	static int32 TCPLocalPort;
	static int32 UDPLocalPort;

	static void InitSocket(FString ServerAddress, int32 TCPLocalP, int32 TCPServerP, int32 UDPLocalP, int32 UDPServerP);
};
