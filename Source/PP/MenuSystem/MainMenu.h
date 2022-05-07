// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PP/MenuSystem/MenuInterface.h"
#include "MainMenu.generated.h"

/**
 * 
 */
UCLASS()
class PP_API UMainMenu : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta = (BindWidget))
	class UButton* BTN_Host;
	UPROPERTY(meta = (BindWidget))
	class UButton* BTN_Join;

	void SetMenuInterface(IMenuInterface* MenuInterfaceRef);

	void SetUp();
	void TearDown();

protected:
	virtual bool Initialize();

private:
	IMenuInterface* MenuInterface;

	UFUNCTION()
	void HostServer();

	UFUNCTION()
	void JoinServer();
};
