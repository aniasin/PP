// Fill out your copyright notice in the Description page of Project Settings.


#include "PP/MenuSystem/MainMenu.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "Components/WidgetSwitcher.h"
#include "PP/MenuSystem/ServerResult.h"

UMainMenu::UMainMenu(const FObjectInitializer& ObjectInitializer)
{
	static ConstructorHelpers::FClassFinder<UUserWidget> ServerResultBPClass(TEXT("/Game/MenuSystem/BP_ServerResult"));
	if (ServerResultBPClass.Class != NULL)
	{
		ServerResultClass = ServerResultBPClass.Class;
		if (!ServerResultClass)	UE_LOG(LogTemp, Warning, TEXT("Could not find ServerResultWidget in MainMenu!"));
	}
}

bool UMainMenu::Initialize()
{
	bool bsuccess = Super::Initialize();
	if (!bsuccess) return false;

	if (!BTN_Host) return false;
	BTN_Host->OnClicked.AddDynamic(this, &UMainMenu::HostServer);

	if (!BTN_Join) return false;
	BTN_Join->OnClicked.AddDynamic(this, &UMainMenu::OpenJoinMenu);

	if (!BTN_Quit) return false;
	BTN_Quit->OnClicked.AddDynamic(this, &UMainMenu::QuitGame);

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
	MenuInterface->SearchSession();
}

void UMainMenu::FoundSessions(TArray<FString> Sessions)
{
	for (const FString Session : Sessions)
	{
		UServerResult* ServerResult = CreateWidget<UServerResult>(this, ServerResultClass);
		ScrollBox_ServerList->AddChild(ServerResult);
		ServerResult->SetTitleText(Session);
	}
}

void UMainMenu::OpenMainMenu()
{
	if (!Switcher || !MainMenu || !ScrollBox_ServerList) return;
	ScrollBox_ServerList->ClearChildren();
	MenuInterface->CancelSearchSession();
	Switcher->SetActiveWidget(MainMenu);
}

void UMainMenu::QuitGame()
{
	if (!MenuInterface) return;
	MenuInterface->QuitGame();
}

void UMainMenu::HostServer()
{
	if (!MenuInterface) return;
	MenuInterface->Host();
}

void UMainMenu::JoinServer()
{
	if (!MenuInterface) return;
	const FString& Address = "";
	MenuInterface->Join(Address);

}


