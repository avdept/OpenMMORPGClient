// Copyright Epic Games, Inc. All Rights Reserved.

#include "ServerStarter.h"

#include <string>

#include "ServerStarterStyle.h"
#include "ServerStarterCommands.h"
#include "LevelEditor.h"
#include "Widgets/Docking/SDockTab.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Input/SSpinBox.h"
#include "Widgets/Text/STextBlock.h"
#include "ToolMenus.h"
#include "Widgets/Layout/SHeader.h"

static const FName ServerStarterTabName("ServerStarter");

#define LOCTEXT_NAMESPACE "FServerStarterModule"

void FServerStarterModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	
	FServerStarterStyle::Initialize();
	FServerStarterStyle::ReloadTextures();

	FServerStarterCommands::Register();
	
	PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(
		FServerStarterCommands::Get().OpenPluginWindow,
		FExecuteAction::CreateRaw(this, &FServerStarterModule::PluginButtonClicked),
		FCanExecuteAction());

	UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FServerStarterModule::RegisterMenus));
	
	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(ServerStarterTabName, FOnSpawnTab::CreateRaw(this, &FServerStarterModule::OnSpawnPluginTab))
		.SetDisplayName(LOCTEXT("FServerStarterTabTitle", "ServerStarter"))
		.SetMenuType(ETabSpawnerMenuType::Hidden);
}

void FServerStarterModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	UToolMenus::UnRegisterStartupCallback(this);

	UToolMenus::UnregisterOwner(this);

	FServerStarterStyle::Shutdown();

	FServerStarterCommands::Unregister();

	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(ServerStarterTabName);
}

TSharedRef<SDockTab> FServerStarterModule::OnSpawnPluginTab(const FSpawnTabArgs& SpawnTabArgs)
{

	TSharedRef<SButton> displayButton = this->ListenServerHandle.IsValid() ? this->StartServerButton() : this->StopServerButton();

	auto Result = (ListenServerHandle.IsValid() ? "SERVER STARTED" : "SERVER STOPPED");
	UE_LOG(LogTemp, Warning, TEXT("TEST"));
	auto dock = SNew(SDockTab);


	return SNew(SDockTab)
        .TabRole(ETabRole::NomadTab)
        [
        	SNew(SVerticalBox) +
        		SVerticalBox::Slot().VAlign(VAlign_Top).MaxHeight(25.f)
				[
					SNew(SHeader)[
						SNew(STextBlock).Text(FText::FromString(TEXT("Server Config"))).Justification(ETextJustify::Center)
					]	
				]+

				SVerticalBox::Slot().MaxHeight(25.f).HAlign(HAlign_Fill).VAlign(VAlign_Top)[
					SNew(SHorizontalBox) + SHorizontalBox::Slot().AutoWidth() [
						SNew(STextBlock).Text(FText::FromString(TEXT("Server Port"))).Justification(ETextJustify::Center).Margin(FMargin(0.f, 0.f, 15.f, 0.f))
						] +
						SHorizontalBox::Slot().MaxWidth(250.f).VAlign(VAlign_Top) [
							SNew(SSpinBox<int32>).MinValue(1200).MaxValue(40000).OnValueChanged_Raw(this, &FServerStarterModule::ChangeValue).Value(PortNumber).MinDesiredWidth(250.f)
						]
					
				]+
				SVerticalBox::Slot().Padding(4.0f, 0.0f).VAlign(VAlign_Top).MaxHeight(25.f).HAlign(HAlign_Left)
				[
					this->StartServerButton()
					]
					+
					SVerticalBox::Slot().Padding(4.0f, 0.0f).VAlign(VAlign_Top).MaxHeight(25.f).HAlign(HAlign_Left) [
					this->StopServerButton()
					
					
					
				]
                
        ];
	

}


void FServerStarterModule::ChangeValue(const int32 Value)
{
	PortNumber = Value;
}


TSharedRef<SButton> FServerStarterModule::StartServerButton()
{
	return SNew(SButton)
                    .ButtonColorAndOpacity(FSlateColor(FLinearColor(FColor::Silver)))
                    .OnClicked_Raw(this, &FServerStarterModule::StartListenServer)
                    [
                        SNew(STextBlock).Text(FText::FromString(TEXT("Start Listen Server"))).Justification(ETextJustify::Center)
                    ];
}

TSharedRef<SButton> FServerStarterModule::StopServerButton()
{
	return SNew(SButton)
                    .ButtonColorAndOpacity(FSlateColor(FLinearColor(FColor::Red)))
                    .OnClicked_Raw(this, &FServerStarterModule::StopListenServer)
                    [
                        SNew(STextBlock).Text(FText::FromString(TEXT("Stop Listen Server"))).Justification(ETextJustify::Center)
                    ];
}

void FServerStarterModule::PluginButtonClicked()
{
	FGlobalTabmanager::Get()->InvokeTab(ServerStarterTabName);
}

const FText UrlPath = FText::FromString(FPaths::RootDir() + TEXT("Engine/Binaries/Win64/UE4Editor.exe"));
const FText ProjectLocationText = FText::FromString(FPaths::ProjectDir() + TEXT("/") + FApp::GetProjectName() + TEXT(".uproject"));

FString FServerStarterModule::ListenServerCLA()
{
	return FString::Printf(
            TEXT("\"%s\" -Port=%s -server -log -nosteam"),
            *ProjectLocationText.ToString(), ToCStr(FString::FromInt(PortNumber))
        );
}

FReply FServerStarterModule::StopListenServer()
{
	
	if (FPlatformProcess::IsProcRunning(ListenServerHandle))
	{
		FPlatformProcess::TerminateProc(ListenServerHandle);
		FPlatformProcess::CloseProc(ListenServerHandle);
	} else
	{
		{
			const FText DialogText = FText::FromString("Your listen server is not running");
			FMessageDialog::Open(EAppMsgType::Ok, DialogText);
		}
	}
	return FReply::Handled();
}

FReply FServerStarterModule::StartListenServer()
{


	if (FPlatformProcess::IsProcRunning(ListenServerHandle))
	{
		const FText DialogText = FText::FromString("You already have listen server running");
		FMessageDialog::Open(EAppMsgType::Ok, DialogText);
		return FReply::Handled();
	}
	ListenServerHandle = FPlatformProcess::CreateProc(
		*UrlPath.ToString(),
		ToCStr(FServerStarterModule::ListenServerCLA()),
		true,
		false,
		false,
		NULL,
		0,
		NULL,
		NULL,
		NULL
		);


	return FReply::Handled();
	
}



void FServerStarterModule::RegisterMenus()
{
	// Owner will be used for cleanup in call to UToolMenus::UnregisterOwner
	FToolMenuOwnerScoped OwnerScoped(this);

	{
		UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu.Window");
		{
			FToolMenuSection& Section = Menu->FindOrAddSection("WindowLayout");
			Section.AddMenuEntryWithCommandList(FServerStarterCommands::Get().OpenPluginWindow, PluginCommands);
		}
	}

	{
		UToolMenu* ToolbarMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar");
		{
			FToolMenuSection& Section = ToolbarMenu->FindOrAddSection("Settings");
			{
				FToolMenuEntry& Entry = Section.AddEntry(FToolMenuEntry::InitToolBarButton(FServerStarterCommands::Get().OpenPluginWindow));
				Entry.SetCommandList(PluginCommands);
			}
		}
	}
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FServerStarterModule, ServerStarter)
