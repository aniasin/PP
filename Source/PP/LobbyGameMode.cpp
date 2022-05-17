// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyGameMode.h"

void ALobbyGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);


}

void ALobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	
	PlayersCount++;
	if (PlayersCount >= 2)
	{
		FTimerHandle Timer;
		UWorld* World = GetWorld();
		if (!World) return;
		World->GetTimerManager().SetTimer(Timer, this, &ALobbyGameMode::StartGame, 10);
	}
	if (PlayersCount >= 3)
	{
		StartGame();
	}
	UE_LOG(LogTemp, Warning, TEXT("Players Count: %d"), PlayersCount)
}

void ALobbyGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);
	PlayersCount--;
	if (PlayersCount < 2)
	{
		GetWorldTimerManager().ClearAllTimersForObject(this);
	}
}

void ALobbyGameMode::StartGame()
{
	UWorld* World = GetWorld();
	if (!World) return;
	bUseSeamlessTravel = true;
	World->ServerTravel("Map01?listen");
}
