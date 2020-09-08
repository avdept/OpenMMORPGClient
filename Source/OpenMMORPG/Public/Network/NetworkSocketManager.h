// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Networking.h"
#include "SocketSubsystem.h"
#include "google/protobuf/message.h"

#include "CoreMinimal.h"

/**
 * 
 */
class OPENMMORPG_API UNetworkSocketManager
{

public:
	
	static UNetworkSocketManager* GetInstance();

	FSocket *TCPSocket;

	void InitSocket(FString ServerAddress, int32 TCPLocalP, int32 TCPServerP, int32 UDPLocalP, int32 UDPServerP);

	// TCP
	TSharedPtr<FInternetAddr> TCPAddress;
	bool bIsConnected;
	void Shutdown();
	bool Alive();
	bool SendByUDP(google::protobuf::Message* message);
	void RunUDPSocketReceiver();
	void TCPReconnect();
	static bool ReadDelimitedFrom(google::protobuf::io::CodedInputStream* input, google::protobuf::MessageLite* message);
	static void Recv(const FArrayReaderPtr& ArrayReaderPtr, const FIPv4Endpoint& EndPt);
	~UNetworkSocketManager();

	//UDP

	FSocket *UDPSocket;

	TSharedPtr<FInternetAddr> UDPAddress;

	FUdpSocketReceiver *UDPReceiver;
	UNetworkSocketManager();

	int32 TCPLocalPort;
	int32 UDPLocalPort;

private:
	

	static UNetworkSocketManager* SocketManagerInstance;
};
