// Fill out your copyright notice in the Description page of Project Settings.


#include "PP/MenuSystem/MainMenu.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "Components/WidgetSwitcher.h"


bool UMainMenu::Initialize()
{
	bool bsuccess = Super::Initialize();
	if (!bsuccess) return false;

	if (!BTN_Host) return false;
	BTN_Host->OnClicked.AddDynamic(this, &UMainMenu::HostServer);

	if (!BTN_Join) return false;
	BTN_Join->OnClicked.AddDynamic(this, &UMainMenu::OpenJoinMenu);

	if (!BTN_JoinGame) return false;
	BTN_JoinGame->OnClicked.AddDynamic(this, &UMainMenu::JoinServer);

	if (!BTN_CancelJoinGame) return false;
	BTN_CancelJoinGame->OnClicked.AddDynamic(this, &UMainMenu::OpenMainMenu);

	return true;
}

void UMainMenu::OpenJoinMenu()
{
	if (!Switcher || !JoinMenu) return;
	Switcher->SetActiveWidget(JoinMenu);
}

void UMainMenu::OpenMainMenu()
{
	if (!Switcher || !MainMenu) return;
	Switcher->SetActiveWidget(MainMenu);
}

void UMainMenu::HostServer()
{
	if (!MenuInterface) return;
	MenuInterface->Host();
}

void UMainMenu::JoinServer()
{
	if (!MenuInterface || !IPAddressField) return;
	const FString& Address = IPAddressField->GetText().ToString();
	MenuInterface->Join(Address);

}
