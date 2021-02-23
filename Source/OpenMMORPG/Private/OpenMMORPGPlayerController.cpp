// Created by Alex Sinelnikov - sinelniko@gmail.com


#include "OpenMMORPGPlayerController.h"

#include "Net/UnrealNetwork.h"

void AOpenMMORPGPlayerController::GetLifetimeReplicatedProps( TArray< FLifetimeProperty > & OutLifetimeProps ) const
{
	DOREPLIFETIME( AOpenMMORPGPlayerController, Name );
}
