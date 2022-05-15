// Fill out your copyright notice in the Description page of Project Settings.


#include "PP/MenuSystem/MenuBase.h"

void UMenuBase::SetMenuInterface(IMenuInterface* MenuInterfaceRef)
{
	MenuInterface = MenuInterfaceRef;
}

void UMenuBase::SetUp()
{
	this->AddToViewport();

	UWorld* World = GetWorld();
	if (!World) return;

	APlayerController* PC = World->GetFirstPlayerController();
	if (!PC) return;

	FInputModeUIOnly InputMode;
	PC->SetInputMode(InputMode);
	PC->SetShowMouseCursor(true);
}

void UMenuBase::TearDown()
{
	this->RemoveFromViewport();

	UWorld* World = GetWorld();
	if (!World) return;

	APlayerController* PC = World->GetFirstPlayerController();
	if (!PC) return;

	FInputModeGameOnly InputMode;
	PC->SetInputMode(InputMode);
	PC->SetShowMouseCursor(false);
}

