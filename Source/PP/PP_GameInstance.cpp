// Fill out your copyright notice in the Description page of Project Settings.


#include "PP_GameInstance.h"
#include "Engine/Engine.h"

void UPP_GameInstance::Init()
{
	Super::Init();

	UE_LOG(LogTemp, Warning, TEXT("Game Instance has been initialized!"));
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
