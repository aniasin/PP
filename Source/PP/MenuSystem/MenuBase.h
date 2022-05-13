// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PP/MenuSystem/MenuInterface.h"
#include "MenuBase.generated.h"

/**
 * 
 */
UCLASS()
class PP_API UMenuBase : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetMenuInterface(IMenuInterface* MenuInterfaceRef);

	void SetUp();
	void TearDown();

	virtual void FoundSessions(TArray<FString> Sessions);

protected:
	IMenuInterface* MenuInterface;
	
};
