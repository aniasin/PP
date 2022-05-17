// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyGameMode.h"

void ALobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	
	PlayersCount++;
	if (PlayersCount >= 3)
	{
		UWorld* World = GetWorld();
		if (!World) return;
		bUseSeamlessTravel = true;
		World->ServerTravel("Map01?listen");
	}
	UE_LOG(LogTemp, Warning, TEXT("Players Count: %d"), PlayersCount)
}

void ALobbyGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);
	PlayersCount--;
}
