// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
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
	UFUNCTION()
	void Join(const FString& IPAddress) override;
	UFUNCTION()
	void LoadMainMenu() override;
	UFUNCTION()
	void QuitGame() override;

	void NetworkError(UWorld* World, UNetDriver* NetDriver, ENetworkFailure::Type FailureType, const FString& ErrorString);

private:
	TSubclassOf<class UUserWidget> MenuClass;
	TSubclassOf<class UUserWidget> GameMenuClass;
	class UMenuBase* Menu;
	
};
