// Fill out your copyright notice in the Description page of Project Settings.


#include "Network/FTCPSocketListeningTh.h"
#include "Network/Handlers/MessageDecoder.h"
#include "OpenMMORPG/Network/SocketObject.h"
#include "OpenMMORPG/Network/Proto/MessageModels.pb.h"

FTCPSocketListeningTh *FTCPSocketListeningTh::Runnable = nullptr;
bool FTCPSocketListeningTh::bIsThreadRunning = false;


FTCPSocketListeningTh::FTCPSocketListeningTh()
{
    Thread = FRunnableThread::Create(this, TEXT("TCP RECIEVER"), 0, TPri_BelowNormal);
}

FTCPSocketListeningTh::~FTCPSocketListeningTh()
{

    delete Thread;
    Thread = nullptr;
}

uint32 FTCPSocketListeningTh::Run()
{
    while(bIsThreadRunning && USocketObject::TCPSocket)
    {
        if (USocketObject::bIsConnected == false)
        {
            FPlatformProcess::Sleep(1.f);
        } else
        {
            FPlatformProcess::Sleep(0.03f);

            if (USocketObject::TCPSocket) return 0;

            TArray<uint8> ReceivedData;

            uint32 Size;

            while(USocketObject::TCPSocket->HasPendingData(Size))
            {
                ReceivedData.Init(FMath::Min(Size, 65507u), Size);

                int32 Read = 0;
                USocketObject::TCPSocket->Recv(ReceivedData.GetData(), ReceivedData.Num(), Read);
            }

            if (ReceivedData.Num() > 0)
            {
                GLog->Log(FString::Printf(TEXT("Data Read! %d"), ReceivedData.Num()) + " | FTCPSocketListeningTh::Run");

                uint8_t *Buffer = ReceivedData.GetData();
                size_t size = ReceivedData.Num();
                google::protobuf::io::ArrayInputStream arr(Buffer, size);
                google::protobuf::io::CodedInputStream input(&arr);

                bool protosize = true;

                while(protosize)
                {
                    std::shared_ptr<Wrapper> wrapper(new Wrapper);
                    protosize = USocketObject::ReadDelimitedFrom(&input, wrapper.get());

                    AsyncTask(ENamedThreads::GameThread, [wrapper]()
                    {
                        UMessageDecoder *Handler = NewObject<UMessageDecoder>(UMessageDecoder::StaticClass());
                        Handler->SendProtoToDecoder(wrapper.get());
                    });
                }
            }
        }
    }
    return 0;
}

bool FTCPSocketListeningTh::Init()
{
    bIsThreadRunning = true;
    return true;
}

FTCPSocketListeningTh* FTCPSocketListeningTh::RunSocketListener()
{
    if (!Runnable && FPlatformProcess::SupportsMultithreading())
    {
        Runnable = new FTCPSocketListeningTh();
    }
    return Runnable;
}

void FTCPSocketListeningTh::Shutdown()
{
    bIsThreadRunning = false;
    GLog->Log("FTCPSocketListeningTh::Shutdown()");

    if (Runnable)
    {
        delete Runnable;
        Runnable = nullptr;
    }
}




