// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PP/MenuSystem/MenuBase.h"
#include "PP/MenuSystem/MenuInterface.h"
#include "GameMenu.generated.h"

/**
 * 
 */
UCLASS()
class PP_API UGameMenu : public UMenuBase
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
	class UButton* BTN_Back;
	UPROPERTY(meta = (BindWidget))
	class UButton* BTN_MainMenu;

protected:
	virtual bool Initialize();


private:
	UFUNCTION()
	void ResponseNo();
	UFUNCTION()
	void ResponseYes();

	
};
