// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PP/MenuSystem/MenuBase.h"
#include "MainMenu.generated.h"

/**
 * 
 */
UCLASS()
class PP_API UMainMenu : public UMenuBase
{
	GENERATED_BODY()

	UMainMenu(const FObjectInitializer& ObjectInitializer);
	TSubclassOf<class UUserWidget> ServerResultClass;

public:
	UPROPERTY(meta = (BindWidget))
	class UButton* BTN_Host;
	UPROPERTY(meta = (BindWidget))
	class UButton* BTN_Join;
	UPROPERTY(meta = (BindWidget))
	class UButton* BTN_Quit;
	UPROPERTY(meta = (BindWidget))
	class UButton* BTN_CancelJoinGame;
	UPROPERTY(meta = (BindWidget))
	class UButton* BTN_JoinGame;

	UPROPERTY(meta = (BindWidget))
	class UWidgetSwitcher* Switcher;

	UPROPERTY(meta = (BindWidget))
	class UWidget* MainMenu;
	UPROPERTY(meta = (BindWidget))
	class UWidget* JoinMenu;

	UPROPERTY(meta = (BindWidget))
	class UPanelWidget* ScrollBox_ServerList;


	UPROPERTY()
	TArray<UUserWidget*> ServerResults;

	virtual void FoundSessions(TArray<FString> Sessions) override;

	void SetSelectedSession(uint32 Index);

protected:
	virtual bool Initialize();

private:
	UFUNCTION()
	void OpenJoinMenu();
	UFUNCTION()
	void OpenMainMenu();
	UFUNCTION()
	void QuitGame();
	UFUNCTION()
	void HostServer();
	UFUNCTION()
	void JoinServer();

	uint32 SelectedSession;
};
