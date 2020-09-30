// Fill out your copyright notice in the Description page of Project Settings.


//#include "MessageEncoder.h"
#include "Network/Handlers/MessageEncoder.h"


#include "google/protobuf/io/zero_copy_stream_impl_lite.h"
#include "OpenMMORPG/Network/SocketObject.h"
#include "OpenMMORPG/Network/Proto/MessageModels.pb.h"


const std::string UMessageEncoder::UtilityMessageName = "Utility";

bool UMessageEncoder::Send(google::protobuf::Message* Message, bool bCrypt, bool bTCP)
{

    Wrapper wrapper;

    if (bCrypt)
    {
        
    } else
    {
        if(Message->GetTypeName() == "Utility")
        {
            Utility *mes = static_cast<Utility*>(Message);
            wrapper.set_allocated_utility(mes);
        } else if (Message->GetTypeName() == "Registration")
        {
           // Registration *mes = static_cast<Registration*>(Message);
           // wrapper.set_allocated_registration(mes);
        }
    }

    size_t size = wrapper.ByteSizeLong() + 5;

    uint8_t *buffer = new uint8_t[size];

    google::protobuf::io::ArrayOutputStream Arr(buffer, size);
    google::protobuf::io::CodedOutputStream Output(&Arr);

    Output.WriteVarint32(wrapper.ByteSizeLong());

    wrapper.SerializeToCodedStream(&Output);

    if (wrapper.has_utility())
    {
        wrapper.release_utility();
    }

    //if (wrapper.has_registration())
    //{
    //    wrapper.release_registration();
    //}

    uint8_t *DelimitedBuffer = Output.GetDirectBufferForNBytesAndAdvance(size);

    if (DelimitedBuffer != NULL)
    {
        wrapper.SerializeWithCachedSizesToArray(DelimitedBuffer);
        
    } else
    {
        wrapper.SerializeWithCachedSizes(&Output);
        if (Output.HadError()) return false;
    }

    int32 BytesSent = 0;
    bool SentState;

    if (bTCP && USocketObject::TCPSocket)
    {
        
        SentState = USocketObject::TCPSocket->Send(buffer, Output.ByteCount(), BytesSent);
    } else
    {
        GLog->Log("Sending UDP connection request");
        SentState = USocketObject::UDPSocket->SendTo(buffer, Output.ByteCount(), BytesSent, *USocketObject::UDPAddress);
    }

    delete[] buffer;
    return SentState;
}

