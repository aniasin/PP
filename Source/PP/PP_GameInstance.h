// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "Engine/GameInstance.h"
#include "PP/MenuSystem/MenuInterface.h"
#include "PP_GameInstance.generated.h"


UCLASS()
class PP_API UPP_GameInstance : public UGameInstance, public IMenuInterface
{
	GENERATED_BODY()

public:
	UPP_GameInstance(const FObjectInitializer& ObjectInitializer);

	virtual void Init();

	UFUNCTION(BlueprintCallable)
	void LoadMenu();
	UFUNCTION(BlueprintCallable)
	void LoadGameMenu();

	UFUNCTION()
	void Host() override;

	void CreateSession();

	UFUNCTION()
	void Join(const int32& SessionIndex) override;
	UFUNCTION()
	void SearchSession() override;
	UFUNCTION()
	void CancelSearchSession() override;
	UFUNCTION()
	void LoadMainMenu() override;
	UFUNCTION()
	void QuitGame() override;

	void DestroySession();

	void NetworkError(UWorld* World, UNetDriver* NetDriver, ENetworkFailure::Type FailureType, const FString& ErrorString);

private:
	TSubclassOf<class UUserWidget> MenuClass;
	TSubclassOf<class UUserWidget> GameMenuClass;

	class UMenuBase* Menu;

	IOnlineSessionPtr SessionInterface;
	TSharedPtr<class FOnlineSessionSearch> SessionSearch;

	void SessionCreated(FName SessionName, bool bSuccess);
	void SessionDestroyed(FName SessionName, bool bSuccess);
	void FoundSession(bool bSuccess);
	void CancelSearchSession(bool bSuccess);
	void JoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);
	
};
