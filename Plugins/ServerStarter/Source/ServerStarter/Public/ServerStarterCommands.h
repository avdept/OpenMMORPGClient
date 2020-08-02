// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "ServerStarterStyle.h"

class FServerStarterCommands : public TCommands<FServerStarterCommands>
{
public:

	FServerStarterCommands()
		: TCommands<FServerStarterCommands>(TEXT("ServerStarter"), NSLOCTEXT("Contexts", "ServerStarter", "ServerStarter Plugin"), NAME_None, FServerStarterStyle::GetStyleSetName())
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

public:
	TSharedPtr< FUICommandInfo > OpenPluginWindow;
};