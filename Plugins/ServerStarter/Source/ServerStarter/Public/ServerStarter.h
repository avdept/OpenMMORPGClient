// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include "Widgets/Input/SSpinBox.h"


class FToolBarBuilder;
class FMenuBuilder;

class FServerStarterModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	
	/** This function will be bound to Command (by default it will bring up plugin window) */
	void PluginButtonClicked();

	FString ListenServerCLA();
	FReply StopListenServer();
	FReply StartListenServer();
	void ChangeValue(int32);
	
private:

	void RegisterMenus();

	FProcHandle ListenServerHandle;
	TSharedRef<class SButton> StartServerButton();
	TSharedRef<class SButton> StopServerButton();
	int32 PortNumber = 9001;


	TSharedRef<class SDockTab> OnSpawnPluginTab(const class FSpawnTabArgs& SpawnTabArgs);

private:
	TSharedPtr<class FUICommandList> PluginCommands;
};
