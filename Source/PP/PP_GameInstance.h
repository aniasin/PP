// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "PP_GameInstance.generated.h"

/**
 * 
 */
UCLASS()
class PP_API UPP_GameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UPP_GameInstance(const FObjectInitializer& ObjectInitializer);

	TSubclassOf<UUserWidget> MenuClass;

	virtual void Init();

	UFUNCTION(BlueprintCallable)
	void LoadMenu();

	UFUNCTION(Exec)
	void Host();

	UFUNCTION(Exec)
	void Join(const FString& IPAddress);
	
};
