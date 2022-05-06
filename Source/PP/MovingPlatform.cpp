// Fill out your copyright notice in the Description page of Project Settings.


#include "MovingPlatform.h"
#include "Components/BoxComponent.h"

AMovingPlatform::AMovingPlatform(const FObjectInitializer& ObjectInitializer):Super(ObjectInitializer)
{	
	PrimaryActorTick.bCanEverTick = true;

	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerComp"));
	TriggerBox->SetupAttachment(GetStaticMeshComponent());

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

	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &AMovingPlatform::OnBeginOverlap);
	TriggerBox->OnComponentEndOverlap.AddDynamic(this, &AMovingPlatform::OnEndOverlap);

	BeginLocation = GetActorLocation();
	EndLocation = GetTransform().TransformPosition(TargetLocation);

	if (HasAuthority())
	{
		SetReplicates(true);
		SetReplicatingMovement(true);
	}

}

void AMovingPlatform::OnBeginOverlap(class UPrimitiveComponent* OverlapedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("BEGIN OVERLAP!"));
	if (!bTriggered) return;
	bActive = true;
}

void AMovingPlatform::OnEndOverlap(class UPrimitiveComponent* OverlapedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	UE_LOG(LogTemp, Warning, TEXT("END OVERLAP!"));
	if (!bTriggered) return;
	bActive = false;
	
}

void AMovingPlatform::MovePlatform(float DeltaSeconds)
{
	if (!bActive) return;

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
