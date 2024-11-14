// Fill out your copyright notice in the Description page of Project Settings.


#include "Combat/PlayerProjectileComponent.h"
#include "Characters/PlayerCharacter.h"

// Sets default values for this component's properties
UPlayerProjectileComponent::UPlayerProjectileComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UPlayerProjectileComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UPlayerProjectileComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UPlayerProjectileComponent::SpawnProjectile(FName ComponentName, TSubclassOf<AActor> ProjectileClass)
{
	USceneComponent* SpawnPointComp = Cast<USceneComponent>(GetOwner()->GetDefaultSubobjectByName(ComponentName));

	FVector SpawnLocation = SpawnPointComp->GetComponentLocation();

	FRotator SpawnRotation = GetOwner()->GetActorRotation();

	GetWorld()->SpawnActor(ProjectileClass, &SpawnLocation, &SpawnRotation);

	FTimerHandle CastTimerHandle;
	GetOwner()->GetWorldTimerManager().SetTimer(CastTimerHandle, this, &UPlayerProjectileComponent::StopCasting, 2.0f, false);
}

void UPlayerProjectileComponent::StopCasting()
{
	GetOwner<APlayerCharacter>()->StopCasting();
}

