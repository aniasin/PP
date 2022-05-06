// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"
#include "MovingPlatform.generated.h"

class UBoxComponent;
 
UCLASS()
class PP_API AMovingPlatform : public AStaticMeshActor
{
	GENERATED_BODY()

public:

	AMovingPlatform(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(EditAnywhere)
	UBoxComponent* TriggerBox;

	FVector BeginLocation;
	FVector EndLocation;

	UPROPERTY(EditAnywhere)
	bool bActive;
	UPROPERTY(EditAnywhere)
	bool bTriggered;
	UPROPERTY(EditAnywhere, meta = (MakeEditWidget = true))
	FVector TargetLocation;
	UPROPERTY(EditAnywhere)
	float MovingSpeed;

protected:

	virtual void BeginPlay() override;

public:

	virtual void Tick(float DeltaSeconds) override;

	UFUNCTION()
	void OnBeginOverlap(class UPrimitiveComponent* OverlapedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnEndOverlap(class UPrimitiveComponent* OverlapedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	void MovePlatform(float DeltaSeconds);


	
};
