// Fill out your copyright notice in the Description page of Project Settings.


#include "PP_GameInstance.h"
#include "Engine/Engine.h"
#include "Blueprint/UserWidget.h"
#include "PP/MenuSystem/MainMenu.h"


UPP_GameInstance::UPP_GameInstance(const FObjectInitializer& ObjectInitializer)
{
	static ConstructorHelpers::FClassFinder<UUserWidget> MainMenuBPClass(TEXT("/Game/MenuSystem/BP_MainMenu"));
	if (MainMenuBPClass.Class != NULL)
	{
		MenuClass = MainMenuBPClass.Class;
		UE_LOG(LogTemp, Warning, TEXT("Game Instance has been Constructed! Found Main Menu Widget: %s"), *MenuClass->GetName());
	}
}

void UPP_GameInstance::Init()
{
	if (!MenuClass) UE_LOG(LogTemp, Warning, TEXT("MenuClass has not been set in GameInstance!"))
	UE_LOG(LogTemp, Warning, TEXT("Game Instance has been Initialized!"))
}

void UPP_GameInstance::LoadMenu()
{
	if (!MenuClass) return;

	Menu = CreateWidget<UMainMenu>(this, MenuClass);
	if (!Menu) return;
	Menu->SetMenuInterface(this);
	Menu->SetUp();
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
