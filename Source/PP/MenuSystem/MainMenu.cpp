// Fill out your copyright notice in the Description page of Project Settings.


#include "PP/MenuSystem/MainMenu.h"
#include "Components/Button.h"

bool UMainMenu::Initialize()
{
	bool bsuccess = Super::Initialize();
	if (!bsuccess) return false;

	if (!BTN_Host) return false;
	BTN_Host->OnClicked.AddDynamic(this, &UMainMenu::HostServer);

	if (!BTN_Join) return false;
	BTN_Join->OnClicked.AddDynamic(this, &UMainMenu::JoinServer);

	return true;
}

void UMainMenu::HostServer()
{
	UE_LOG(LogTemp, Warning, TEXT("Hosting Server! ..."));
}

void UMainMenu::JoinServer()
{
	UE_LOG(LogTemp, Warning, TEXT("Joining Server! ..."));
}
