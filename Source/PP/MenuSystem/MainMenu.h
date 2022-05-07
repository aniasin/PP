// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
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

protected:
	virtual bool Initialize();

private:
	UFUNCTION()
	void HostServer();

	UFUNCTION()
	void JoinServer();
};
