// Fill out your copyright notice in the Description page of Project Settings.


#include "OpenMMORPG/Public/Network/NetworkInterface.h"
#include "OpenMMORPG/Public/Network/Handlers/MessageEncoder.h"
#include "OpenMMORPG/Network/Proto/MessageModels.pb.h"

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
