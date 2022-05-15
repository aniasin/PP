// Fill out your copyright notice in the Description page of Project Settings.


#include "PP/MenuSystem/ServerResult.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"

void UServerResult::SetUp(UMainMenu* Parent, uint32 Index, FSessionData Session)
{
	MainMenu = Parent;
	ButtonIndex = Index;
	Text_UserName->SetText(FText::FromString(Session.HostUserName));
	Text_ServerName->SetText(FText::FromString(Session.Name));
	Text_Fraction->SetText(FText::FromString(FString::Printf(TEXT("%d/%d"), Session.CurrentPlayers, Session.MaxPlayers)));
	if (!BTN) return;
	BTN->OnClicked.AddDynamic(this, &UServerResult::Clicked);
	BTN->OnHovered.AddDynamic(this, &UServerResult::Hover);
	BTN->OnUnhovered.AddDynamic(this, &UServerResult::UnHover);
}

void UServerResult::SetTextColor(FLinearColor Color)
{
	Text_ServerName->SetColorAndOpacity(Color);
	Text_UserName->SetColorAndOpacity(Color);
	Text_Fraction->SetColorAndOpacity(Color);
}

void UServerResult::SetButtonSelected(bool Value)
{
	bSelected = Value;
}

void UServerResult::Clicked()
{
	if (!MainMenu) return;
	SetTextColor(FLinearColor::Green);
	bSelected = true;
	MainMenu->SetSelectedSession(ButtonIndex);
}

void UServerResult::Hover()
{
	if (bSelected) return;
	SetTextColor(FLinearColor::Yellow);
}

void UServerResult::UnHover()
{
	if (bSelected) return;
	SetTextColor(FLinearColor::White);
}
