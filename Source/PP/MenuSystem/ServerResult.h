// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ServerResult.generated.h"

/**
 * 
 */
UCLASS()
class PP_API UServerResult : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Text_ServerName;

	void SetTitleText(FString Title);

private:


};
