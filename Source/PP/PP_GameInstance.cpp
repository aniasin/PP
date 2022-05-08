// Fill out your copyright notice in the Description page of Project Settings.


#include "PP_GameInstance.h"
#include "Engine/Engine.h"
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
	if (!MenuClass) UE_LOG(LogTemp, Warning, TEXT("MenuClass has not been set in GameInstance!"))
	if (!GameMenuClass) UE_LOG(LogTemp, Warning, TEXT("GameMenuClass has not been set in GameInstance!"))
	UE_LOG(LogTemp, Warning, TEXT("Game Instance has been Initialized!"))
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
