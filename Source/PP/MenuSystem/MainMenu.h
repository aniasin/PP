// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PP/MenuSystem/MenuBase.h"
#include "MainMenu.generated.h"

USTRUCT()
struct FSessionData
{
	GENERATED_BODY()

	FString Name;
	uint16 CurrentPlayers;
	uint16 MaxPlayers;
	FString HostUserName;
};

UCLASS()
class PP_API UMainMenu : public UMenuBase
{
	GENERATED_BODY()

	UMainMenu(const FObjectInitializer& ObjectInitializer);
	TSubclassOf<class UUserWidget> ServerResultClass;

public:
	UPROPERTY(meta = (BindWidget))
	class UButton* BTN_HostMenu;
	UPROPERTY(meta = (BindWidget))
	class UButton* BTN_Join;
	UPROPERTY(meta = (BindWidget))
	class UButton* BTN_Quit;
	UPROPERTY(meta = (BindWidget))
	class UButton* BTN_CancelJoinGame;
	UPROPERTY(meta = (BindWidget))
	class UButton* BTN_CancelCreateSession;
	UPROPERTY(meta = (BindWidget))
	class UButton* BTN_JoinGame;
	UPROPERTY(meta = (BindWidget))
		class UButton* BTN_Host;

	UPROPERTY(meta = (BindWidget))
	class UWidgetSwitcher* Switcher;

	UPROPERTY(meta = (BindWidget))
	class UWidget* MainMenu;
	UPROPERTY(meta = (BindWidget))
	class UWidget* HostMenu;
	UPROPERTY(meta = (BindWidget))
	class UWidget* JoinMenu;

	UPROPERTY(meta = (BindWidget))
	class UEditableTextBox* EditableTextBox_SessionName;
	UPROPERTY(meta = (BindWidget))
	class UPanelWidget* ScrollBox_ServerList;


	UPROPERTY()
	TArray<UUserWidget*> ServerResults;

	void FoundSessions(TArray<FSessionData> Sessions);

	void SetSelectedSession(uint32 Index);

protected:
	virtual bool Initialize();

private:
	UFUNCTION()
	void OpenHostMenu();
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
