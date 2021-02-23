#include "UserService.h"


#include "Network/GrpcService.h"
#include "OpenMMORPG/Network/Proto/Player.grpc.pb.h"


proto_messages::Player* UserService::GetCharacterProfile()
{
	auto params = new proto_messages::PlayerParams;
	auto Response = new proto_messages::Player;

	auto lambda = [](std::unique_ptr<proto_messages::PlayerService::Stub> &Stub,
                     grpc::ClientContext *context,
                     proto_messages::PlayerParams Request,
                     proto_messages::Player *Response) ->grpc::Status
	{
		return Stub->GetPlayerInfo(context, Request, Response);
	};

	GLog->Log("Before request");
	grpc::Status const status = SendServerRequest<proto_messages::PlayerService,
                proto_messages::PlayerParams,
                proto_messages::Player>(*params, *Response, lambda, this->UserToken, this->CharacterID);
	GLog->Log("After request");
	if (status.ok())
	{
		return Response;
	}

	return nullptr;
}
