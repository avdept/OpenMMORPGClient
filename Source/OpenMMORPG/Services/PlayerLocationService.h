// Created by Alex Sinelnikov - sinelniko@gmail.com

#pragma once

#include "CoreMinimal.h"

#include "OpenMMORPG/OpenMMORPGCharacter.h"

/**
 * 
 */
class OPENMMORPG_API PlayerLocationService
{
public:
	PlayerLocationService(AOpenMMORPGCharacter*);
	~PlayerLocationService();

	bool SendNewLocation();

	AOpenMMORPGCharacter *Character;
};
