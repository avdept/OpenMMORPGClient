// Created by Alex Sinelnikov - sinelniko@gmail.com


#include "PlayerLocationService.h"


#include "OpenMMORPGPlayerController.h"
#include "Network/GrpcService.h"
#include "OpenMMORPG/Network/Proto/LocationPersistence.grpc.pb.h"
#include "OpenMMORPG/Network/Proto/LocationPersistence.pb.h"


PlayerLocationService::PlayerLocationService(AOpenMMORPGCharacter *Char)
{
	Character = Char;
	// At some point this needs to be refactored in more safe-typed way
	
	auto const Controller = Cast<AOpenMMORPGPlayerController>(Character->GetController());
	if (Controller)
	{
		Token = Controller->AuthToken;
		PlayerID = Controller->PlayerID;
	}
}

PlayerLocationService::~PlayerLocationService()
{
}

bool PlayerLocationService::SendNewLocation()
{
	if (!Character->IsPendingKill())
	{
		auto params = Character->GetProtoPlayerLocation();
		auto Response = new utility_messages::LocationPersistenceResult;

		auto lambda = [](std::unique_ptr<utility_messages::LocationPersistenceService::Stub> &Stub,
                         grpc::ClientContext *context,
                         utility_messages::LocationPersistenceParams Request,
                         utility_messages::LocationPersistenceResult *Response) ->grpc::Status
		{
			return Stub->PersistLocation(context, Request, Response);
		};

		grpc::Status const status = SendServerRequest<utility_messages::LocationPersistenceService,
                    utility_messages::LocationPersistenceParams,
                    utility_messages::LocationPersistenceResult>(*params, *Response, lambda, Token, PlayerID);
		return status.ok();
	}
	return true;
	
	
}
