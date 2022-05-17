// Fill out your copyright notice in the Description page of Project Settings.


#include "PP_GameInstance.h"
#include "Engine/Engine.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "Blueprint/UserWidget.h"
#include "PP/MenuSystem/MenuBase.h"
#include "PP/MenuSystem/MainMenu.h"
#include "MoviePlayer.h"


const static FName SERVER_NAME_SETTINGS_KEY = TEXT("SessionName");

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
	// Displays a simple loading widget while any level loads
	FCoreUObjectDelegates::PreLoadMap.AddUObject(this, &UPP_GameInstance::BeginLoadingScreen);
	FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(this, &UPP_GameInstance::EndLoadingScreen);


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
		SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &UPP_GameInstance::JoinSessionComplete);
	}
	else { UE_LOG(LogTemp, Warning, TEXT("Session Interface not found!")) }
	GEngine->OnNetworkFailure().AddUObject(this, &UPP_GameInstance::NetworkError);
}

void UPP_GameInstance::BeginLoadingScreen(const FString& InMapName)
{
	if (!IsRunningDedicatedServer())
	{
		FLoadingScreenAttributes LoadingScreen;
		LoadingScreen.bAutoCompleteWhenLoadingCompletes = false;
		LoadingScreen.WidgetLoadingScreen = FLoadingScreenAttributes::NewTestLoadingScreenWidget();

		GetMoviePlayer()->SetupLoadingScreen(LoadingScreen);
	}
}

void UPP_GameInstance::EndLoadingScreen(UWorld* InLoadedWorld)
{
	/// can be overriden to have custom effects
}

void UPP_GameInstance::NetworkError(UWorld* World, UNetDriver* NetDriver, ENetworkFailure::Type FailureType, const FString& ErrorString)
{
	LoadMainMenu();
}

void UPP_GameInstance::LoadMenu()
{
	if (!MenuClass) return;

	Menu = CreateWidget<UMainMenu>(this, MenuClass);
	if (!Menu) return;
	Menu->SetMenuInterface(this);
	Menu->SetUp();
	GEngine->AddOnScreenDebugMessage(0, 30, FColor::Green, TEXT("Version ... " + CurrentVersion));
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
	GetFirstLocalPlayerController()->ClientTravel("/Game/MenuSystem/MainMenu", ETravelType::TRAVEL_Absolute);
}

void UPP_GameInstance::QuitGame()
{
	GEngine->AddOnScreenDebugMessage(0, 2, FColor::Green, TEXT("Quiting... "));
	GetFirstLocalPlayerController()->ConsoleCommand("quit");
}

void UPP_GameInstance::Host(FString SessionName)
{
	DesiredSessionName = SessionName;
	if (SessionInterface)
	{
		if (SessionInterface->GetNamedSession(NAME_GameSession))
		{
			DestroySession();
		}
		else
		{
			CreateSession();
		}		
	}
}

void UPP_GameInstance::Join(const int32& SessionIndex)
{
	if (SessionIndex >= 0)
	{
		if (Menu) Menu->TearDown();
		SessionInterface->JoinSession(0, NAME_GameSession, SessionSearch->SearchResults[SessionIndex]);
	}
}

void UPP_GameInstance::SearchSession()
{
	if (SessionInterface)
	{
		SessionSearch = MakeShareable(new FOnlineSessionSearch);
		if (SessionSearch)
		{
			SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);
			SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
			GEngine->AddOnScreenDebugMessage(0, 2, FColor::Green, TEXT("Searching Session... "));
		}
	}
}

void UPP_GameInstance::CreateSession()
{
	if (!SessionInterface) return;

	FOnlineSessionSettings SessionSettings;
	IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get();
	OnlineSubsystem->GetSubsystemName() == "NULL" ? SessionSettings.bIsLANMatch = true : SessionSettings.bIsLANMatch = false;
	SessionSettings.NumPublicConnections = 2;
	SessionSettings.bShouldAdvertise = true;
	SessionSettings.bUsesPresence = true;
	SessionSettings.bUseLobbiesIfAvailable = true;
	SessionSettings.Set(SERVER_NAME_SETTINGS_KEY, DesiredSessionName, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
	SessionInterface->CreateSession(0, NAME_GameSession, SessionSettings);

	GEngine->AddOnScreenDebugMessage(0, 2, FColor::Green, TEXT("Session Created!"));
}

void UPP_GameInstance::DestroySession()
{
	SessionInterface->DestroySession(NAME_GameSession);
}

void UPP_GameInstance::SessionCreated(FName SessionName, bool bSuccess)
{
	if (!bSuccess) { UE_LOG(LogTemp, Warning, TEXT("Failed to create Session!"), *SessionName.ToString()) return; }

	if (Menu) Menu->TearDown();
	UWorld* World = GetWorld();
	if (!World) return;
	World->ServerTravel("Lobby?listen");
}

void UPP_GameInstance::SessionDestroyed(FName SessionName, bool bSuccess)
{
	if (bSuccess)
	{
		CreateSession();
	}
}

void UPP_GameInstance::FoundSession(bool bSuccess)
{
	TArray<FSessionData> Results;
	if (bSuccess && SessionSearch)
	{
		for (const FOnlineSessionSearchResult& SessionSearchResult : SessionSearch->SearchResults)
		{
			UE_LOG(LogTemp, Warning, TEXT("Found Sessions names: %s!"), *SessionSearchResult.GetSessionIdStr())
			FSessionData SessionData;
			
			SessionData.HostUserName = SessionSearchResult.Session.OwningUserName;
			SessionData.MaxPlayers = SessionSearchResult.Session.SessionSettings.NumPublicConnections;
			SessionData.CurrentPlayers = SessionData.MaxPlayers - SessionSearchResult.Session.NumOpenPublicConnections;
			FString SessionName;
			if (SessionSearchResult.Session.SessionSettings.Get(SERVER_NAME_SETTINGS_KEY, SessionName))
			{
				SessionData.Name = SessionName;
			}
			else
			{
				SessionData.Name = "Could not find name.";
			}
			Results.Add(SessionData);
		}
	}
	Menu->FoundSessions(Results);
}

void UPP_GameInstance::JoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	FString Address;
	if (!SessionInterface) return;
	if (!SessionInterface->GetResolvedConnectString(SessionName, Address))
	{
		UE_LOG(LogTemp, Warning, TEXT("Could not get connect String!"))
	}
	APlayerController* PC = GetFirstLocalPlayerController();
	if (!PC) return;

	PC->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
}




