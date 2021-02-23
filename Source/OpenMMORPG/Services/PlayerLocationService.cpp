// Created by Alex Sinelnikov - sinelniko@gmail.com


#include "PlayerLocationService.h"

#include "Network/GrpcService.h"
#include "OpenMMORPG/Network/Proto/LocationPersistence.grpc.pb.h"
#include "OpenMMORPG/Network/Proto/LocationPersistence.pb.h"


PlayerLocationService::PlayerLocationService(AOpenMMORPGCharacter *Char)
{
	Character = Char;
}

PlayerLocationService::~PlayerLocationService()
{
}

bool PlayerLocationService::SendNewLocation()
{
	if (!Character->IsPendingKill())
	{
		auto params = new utility_messages::LocationPersistenceParams;
		params->set_allocated_location(Character->GetProtoPlayerLocation());
		auto Response = new utility_messages::LocationPersistenceResult;

		auto lambda = [](std::unique_ptr<utility_messages::LocationPersistenceService::Stub> &Stub,
                         grpc::ClientContext *context,
                         utility_messages::LocationPersistenceParams Request,
                         utility_messages::LocationPersistenceResult *Response) ->grpc::Status
		{
			return Stub->PersistLocation(context, Request, Response);
		};

		grpc::Status const status = SendRequest<utility_messages::LocationPersistenceService,
                    utility_messages::LocationPersistenceParams,
                    utility_messages::LocationPersistenceResult>(*params, *Response, lambda);
		return status.ok();
	}

	return true;
	
	
}
