#pragma once
#include "OpenMMORPG/Network/Proto/Player.pb.h"

class UserService
{
public:
	UserService(FString Token, FString CharID)
	{
		UserToken = Token;
		CharacterID = CharID;
		
	};

	proto_messages::Player* GetCharacterProfile();

private:
	FString UserToken;
	FString CharacterID;
};
