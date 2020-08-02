// Copyright Epic Games, Inc. All Rights Reserved.

#include "ServerStarterCommands.h"

#define LOCTEXT_NAMESPACE "FServerStarterModule"

void FServerStarterCommands::RegisterCommands()
{
	UI_COMMAND(OpenPluginWindow, "ServerStarter", "Bring up ServerStarter window", EUserInterfaceActionType::Button, FInputGesture());
}

#undef LOCTEXT_NAMESPACE
