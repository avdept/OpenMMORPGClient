// Created by Alex Sinelnikov - sinelniko@gmail.com


#include "OpenMMORPGPlayerController.h"

#include "Net/UnrealNetwork.h"

void AOpenMMORPGPlayerController::GetLifetimeReplicatedProps( TArray< FLifetimeProperty > & OutLifetimeProps ) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME( AOpenMMORPGPlayerController, Name );
}
