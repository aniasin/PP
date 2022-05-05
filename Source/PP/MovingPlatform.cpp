// Fill out your copyright notice in the Description page of Project Settings.


#include "MovingPlatform.h"

AMovingPlatform::AMovingPlatform()
{
	PrimaryActorTick.bCanEverTick = true;

}

void AMovingPlatform::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (HasAuthority())
	{
		FVector Location = GetActorLocation();
		Location += FVector(MovingSpeed * DeltaSeconds, 0, 0);
		SetActorLocation(Location);
	}
	
}

void AMovingPlatform::BeginPlay()
{
	Super::BeginPlay();

	SetMobility(EComponentMobility::Movable);

}
