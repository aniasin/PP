// Fill out your copyright notice in the Description page of Project Settings.


#include "PP_GameInstance.h"
#include "Engine/Engine.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "Blueprint/UserWidget.h"
#include "PP/MenuSystem/MenuBase.h"

const static FName SESSION_NAME = TEXT("My Session");

UPP_GameInstance::UPP_GameInstance(const FObjectInitializer& ObjectInitializer)
{
	static ConstructorHelpers::FClassFinder<UUserWidget> MainMenuBPClass(TEXT("/Game/MenuSystem/BP_MainMenu"));
	if (MainMenuBPClass.Class != NULL)
	{
		MenuClass = MainMenuBPClass.Class;
		if (!MenuClass)	UE_LOG(LogTemp, Warning, TEXT("Could not find MenuClass in GameInstance!"));
	}
	static ConstructorHelpers::FClassFinder<UUserWidget> GameMenuBPClass(TEXT("/Game/MenuSystem/BP_GameMenu"));
	if (GameMenuBPClass.Class != NULL)
	{
		GameMenuClass = GameMenuBPClass.Class;
		if (!GameMenuClass)	UE_LOG(LogTemp, Warning, TEXT("Could not find GameMenuClass in GameInstance!"));
	}
}

void UPP_GameInstance::Init()
{
	IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get();
	FString LogMessage;
	OnlineSubsystem != nullptr ? LogMessage = OnlineSubsystem->GetSubsystemName().ToString() : "OnlineSubsystem not found!";
	UE_LOG(LogTemp, Warning, TEXT("Loaded subsystem: %s"), *LogMessage)

	SessionInterface = OnlineSubsystem->GetSessionInterface();
	if (SessionInterface)
	{
		SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UPP_GameInstance::SessionCreated);
		SessionInterface->OnDestroySessionCompleteDelegates.AddUObject(this, &UPP_GameInstance::SessionDestroyed);
		SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UPP_GameInstance::FoundSession);
		SessionInterface->OnCancelFindSessionsCompleteDelegates.AddUObject(this, &UPP_GameInstance::CancelSearchSession);
	}
	else { UE_LOG(LogTemp, Warning, TEXT("Session Interface not found!")) }



	GEngine->OnNetworkFailure().AddUObject(this, &UPP_GameInstance::NetworkError);
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
void UPP_GameInstance::LoadMainMenu()
{
	GEngine->AddOnScreenDebugMessage(0, 2, FColor::Green, TEXT("Main Menu... "));
	GetFirstLocalPlayerController()->ClientTravel("/Game/MenuSystem/MainMenu", ETravelType::TRAVEL_Absolute);
}

void UPP_GameInstance::QuitGame()
{
	if (SessionInterface && SessionInterface->GetNamedSession(SESSION_NAME))
	{
		DestroySession();
	}

	GEngine->AddOnScreenDebugMessage(0, 2, FColor::Green, TEXT("Quiting... "));
	GetFirstLocalPlayerController()->ConsoleCommand("quit");
}

void UPP_GameInstance::Host()
{
	if (SessionInterface && !SessionInterface->GetNamedSession(SESSION_NAME))
	{
		CreateSession();
	}
	else 
	{ 
		DestroySession();
	}
}

void UPP_GameInstance::Join(const FString& IPAddress)
{
	if (!SessionInterface || !SessionInterface->GetNamedSession(SESSION_NAME))
	{
		if (Menu) Menu->TearDown();
		GetWorld()->ServerTravel("Map01?listen");
	}
}

void UPP_GameInstance::SearchSession()
{
	if (SessionInterface && SessionInterface->GetNamedSession(SESSION_NAME))
	{
		SessionSearch = MakeShareable(new FOnlineSessionSearch);
		if (SessionSearch)
		{
			SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
			UE_LOG(LogTemp, Warning, TEXT("Searching Session..."))
		}
	}
}

void UPP_GameInstance::CancelSearchSession()
{
	if (SessionInterface && SessionSearch)
	{
		SessionInterface->CancelFindSessions();
	}
}

void UPP_GameInstance::CreateSession()
{
	if (!SessionInterface) return;

	FOnlineSessionSettings SessionSettings;
	SessionSettings.bIsLANMatch = true;
	SessionSettings.NumPublicConnections = 2;
	SessionSettings.bShouldAdvertise = true;
	SessionInterface->CreateSession(0, SESSION_NAME, SessionSettings);
}

void UPP_GameInstance::DestroySession()
{
	SessionInterface->DestroySession(SESSION_NAME);
}

void UPP_GameInstance::NetworkError(UWorld* World, UNetDriver* NetDriver, ENetworkFailure::Type FailureType, const FString& ErrorString)
{
	LoadMainMenu();
}

void UPP_GameInstance::SessionCreated(FName SessionName, bool bSuccess)
{
	if (!bSuccess) { UE_LOG(LogTemp, Warning, TEXT("Failed to create Session!"), *SessionName.ToString()) return; }

	if (Menu) Menu->TearDown();
	UWorld* World = GetWorld();
	if (!World) return;
	World->ServerTravel("Map01?listen");
}

void UPP_GameInstance::SessionDestroyed(FName SessionName, bool bSuccess)
{
	if (bSuccess)
	{
		CreateSession();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to destroy Session!"))
	}
}

void UPP_GameInstance::FoundSession(bool bSuccess)
{
	TArray<FString> Results;
	if (bSuccess && SessionSearch)
	{
		for (const FOnlineSessionSearchResult& SessionSearchResult : SessionSearch->SearchResults)
		{
			UE_LOG(LogTemp, Warning, TEXT("Found Sessions names: %s!"), *SessionSearchResult.GetSessionIdStr())
			Results.Add(SessionSearchResult.Session.OwningUserName);
			
		}
	}
	else
	{
		Results.Add("No Session Found!");
	}
	Menu->FoundSessions(Results);
}

void UPP_GameInstance::CancelSearchSession(bool bSuccess)
{
	if(bSuccess) UE_LOG(LogTemp, Warning, TEXT("Search Canceled!"))
}



