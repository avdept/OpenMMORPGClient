// Created by Alex Sinelnikov - sinelniko@gmail.com

#pragma once
#include "WorldStaticManager.h"
#include "grpcpp/impl/codegen/client_context.h"
#include "OpenMMORPG/Network/SocketObject.h"
#include "OpenMMORPG/Network/Proto/Server.grpc.pb.h"


template <typename  ReqStub, typename Request, typename  Response, typename Lambda>
grpc::Status SendRequest(Request &Req, Response &Resp, Lambda &lamb)
{
    std::unique_ptr<ReqStub::Stub> ServiceStub = ReqStub::NewStub(USocketObject::GRPCChannel);

    grpc::ClientContext context;
    std::string const Bearer = "Bearer " + std::string(TCHAR_TO_UTF8(*WorldStaticManager::GameInstance->AuthToken));
    context.AddMetadata("authorization", Bearer);
    context.AddMetadata("character_id", "120");
    return lamb(ServiceStub, &context, Req, &Resp);
}


template <typename  ReqStub, typename Request, typename  Response, typename Lambda>
grpc::Status SendServerRequest(Request &Req, Response &Resp, Lambda &lamb, FString Bearer, FString CharacterID)
{
    std::unique_ptr<ReqStub::Stub> ServiceStub = ReqStub::NewStub(USocketObject::GRPCChannel);

    grpc::ClientContext context;
    std::string const BearerStr = "Bearer " + std::string(TCHAR_TO_UTF8(&Bearer));
    context.AddMetadata("authorization", BearerStr);
    context.AddMetadata("character_id", std::string(TCHAR_TO_UTF8(&CharacterID)));
    return lamb(ServiceStub, &context, Req, &Resp);
}
