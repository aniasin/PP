// Fill out your copyright notice in the Description page of Project Settings.


#include "MovingPlatform.h"

AMovingPlatform::AMovingPlatform(const FObjectInitializer& ObjectInitializer):Super(ObjectInitializer)
{	
	PrimaryActorTick.bCanEverTick = true;

}

void AMovingPlatform::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (HasAuthority())
	{
		MovePlatform(DeltaSeconds);

	}	
}

void AMovingPlatform::BeginPlay()
{
	Super::BeginPlay();

	BeginLocation = GetActorLocation();
	EndLocation = GetTransform().TransformPosition(TargetLocation);

	if (HasAuthority())
	{
		SetReplicates(true);
		SetReplicatingMovement(true);
	}

}

void AMovingPlatform::MovePlatform(float DeltaSeconds)
{
	FVector CurrentDestination = EndLocation;
	FVector Location = GetActorLocation();
	FVector Direction = (CurrentDestination - Location).GetSafeNormal();
	Location += Direction * MovingSpeed * DeltaSeconds;
	SetActorLocation(Location);

	if (FVector::Dist(Location, CurrentDestination) < 10)
	{
		Swap(BeginLocation, EndLocation);
	}
}
