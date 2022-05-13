// Fill out your copyright notice in the Description page of Project Settings.


#include "PP/MenuSystem/ServerResult.h"
#include "Components/TextBlock.h"

void UServerResult::SetTitleText(FString Title)
{
	Text_ServerName->SetText(FText::FromString(Title));
}
