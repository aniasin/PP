// Fill out your copyright notice in the Description page of Project Settings.


#include "PP_GameInstance.h"
#include "Engine/Engine.h"
#include "OnlineSubsystem.h"
#include "Blueprint/UserWidget.h"
#include "PP/MenuSystem/MenuBase.h"


UPP_GameInstance::UPP_GameInstance(const FObjectInitializer& ObjectInitializer)
{
	static ConstructorHelpers::FClassFinder<UUserWidget> MainMenuBPClass(TEXT("/Game/MenuSystem/BP_MainMenu"));
	if (MainMenuBPClass.Class != NULL)
	{
		MenuClass = MainMenuBPClass.Class;
		UE_LOG(LogTemp, Warning, TEXT("Found Main Menu Widget: %s"), *MenuClass->GetName());
	}
	static ConstructorHelpers::FClassFinder<UUserWidget> GameMenuBPClass(TEXT("/Game/MenuSystem/BP_GameMenu"));
	if (GameMenuBPClass.Class != NULL)
	{
		GameMenuClass = GameMenuBPClass.Class;
		UE_LOG(LogTemp, Warning, TEXT("Found Game Menu Widget: %s"), *GameMenuClass->GetName());
	}
}

void UPP_GameInstance::Init()
{
	IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get();
	FString message;
	OnlineSubsystem != nullptr ? message = OnlineSubsystem->GetSubsystemName().ToString() : "OnlineSubsystem not found!";
	UE_LOG(LogTemp, Warning, TEXT("Loaded subsystem: %s"), *message)

	if (OnlineSubsystem->GetSessionInterface().IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("Found OnlineSession"))
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("OnlineSession not found!"))
	}
	
	GEngine->OnNetworkFailure().AddUObject(this, &UPP_GameInstance::NetworkError);
}

void UPP_GameInstance::LoadMenu()
{
	if (!MenuClass) return;

	Menu = CreateWidget<UMenuBase>(this, MenuClass);
	if (!Menu) return;
	Menu->SetMenuInterface(this);
	Menu->SetUp();
}

void UPP_GameInstance::LoadGameMenu()
{
	if (!GameMenuClass) return;

	UMenuBase* GameMenu = CreateWidget<UMenuBase>(this, GameMenuClass);
	if (!GameMenu) return;
	GameMenu->SetMenuInterface(this);
	GameMenu->SetUp();
}

void UPP_GameInstance::Host()
{
	if (Menu) Menu->TearDown();

	GEngine->AddOnScreenDebugMessage(0, 2, FColor::Green, TEXT("HOSTING..."));
	GetWorld()->ServerTravel("Map01?listen");
}

void UPP_GameInstance::Join(const FString& IPAddress)
{
	if (Menu) Menu->TearDown();

	GEngine->AddOnScreenDebugMessage(0, 2, FColor::Green, TEXT("JOINING " + IPAddress + "..."));
	GetFirstLocalPlayerController()->PreClientTravel(IPAddress, ETravelType::TRAVEL_Absolute, false);
	GetFirstLocalPlayerController()->ClientTravel(IPAddress, ETravelType::TRAVEL_Absolute);
}

void UPP_GameInstance::LoadMainMenu()
{	
	GEngine->AddOnScreenDebugMessage(0, 2, FColor::Green, TEXT("Main Menu... " ));

	GetFirstLocalPlayerController()->ClientTravel("/Game/MenuSystem/MainMenu", ETravelType::TRAVEL_Absolute);
}

void UPP_GameInstance::QuitGame()
{
	GEngine->AddOnScreenDebugMessage(0, 2, FColor::Green, TEXT("Quiting... "));
	GetFirstLocalPlayerController()->ConsoleCommand("quit");
}

void UPP_GameInstance::NetworkError(UWorld* World, UNetDriver* NetDriver, ENetworkFailure::Type FailureType, const FString& ErrorString)
{
	LoadMainMenu();
}
