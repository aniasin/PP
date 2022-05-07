// Fill out your copyright notice in the Description page of Project Settings.


#include "PP_GameInstance.h"
#include "Engine/Engine.h"
#include "Blueprint/UserWidget.h"


UPP_GameInstance::UPP_GameInstance(const FObjectInitializer& ObjectInitializer)
{
	static ConstructorHelpers::FClassFinder<UUserWidget> MainMenuBPClass(TEXT("/Game/MenuSystem/WBP_MainMenu"));
	if (MainMenuBPClass.Class != NULL)
	{
		MenuClass = MainMenuBPClass.Class;
		UE_LOG(LogTemp, Warning, TEXT("Game Instance has been initialized! Found Main Menu Widget: %s"), *MenuClass->GetName());
	}
}

void UPP_GameInstance::Init()
{

}

void UPP_GameInstance::LoadMenu()
{
	UUserWidget* Menu = CreateWidget<UUserWidget>(this, MenuClass);
	if (!Menu) return;
	Menu->AddToViewport();

	APlayerController* PC = GetFirstLocalPlayerController();
	if (!PC) return;

	FInputModeUIOnly InputMode;
	InputMode.SetWidgetToFocus(Menu->TakeWidget());
	PC->SetInputMode(InputMode);
	PC->bShowMouseCursor = true;
}

void UPP_GameInstance::Host()
{
	GEngine->AddOnScreenDebugMessage(0, 2, FColor::Green, TEXT("HOSTING..."));
	GetWorld()->ServerTravel("Map01?listen");
}

void UPP_GameInstance::Join(const FString& IPAddress)
{
	GEngine->AddOnScreenDebugMessage(0, 2, FColor::Green, TEXT("JOINING " + IPAddress + "..."));
	GetFirstLocalPlayerController()->ClientTravel(IPAddress, ETravelType::TRAVEL_Absolute);
}
