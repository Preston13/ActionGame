// Fill out your copyright notice in the Description page of Project Settings.


#include "Combat/LockonComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/SpringArmComponent.h"
#include "Interfaces/Enemy.h"

// Sets default values for this component's properties
ULockonComponent::ULockonComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void ULockonComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerRef = GetOwner<ACharacter>();
	Controller = GetWorld()->GetFirstPlayerController();
	MovementComp = OwnerRef->GetCharacterMovement();
	SpringArmComp = OwnerRef->FindComponentByClass<USpringArmComponent>();
}

void ULockonComponent::StartLockon(float Radius)
{
	if (!CheckForEnemies(Radius))
	{
		return;
	}

	Controller->SetIgnoreLookInput(true);
	MovementComp->bOrientRotationToMovement = false;
	MovementComp->bUseControllerDesiredRotation = true;
	SpringArmComp->TargetOffset = FVector{ 0.0, 0.0, 100.0 };

	IEnemy::Execute_OnSelect(CurrentTargetActor);

	OnUpdatedTargetDelegate.Broadcast(CurrentTargetActor);
}

void ULockonComponent::EndLockon()
{
	IEnemy::Execute_OnDeselect(CurrentTargetActor);
	OnUpdatedTargetDelegate.Broadcast(CurrentTargetActor);
	CurrentTargetActor = nullptr;
	Controller->ResetIgnoreLookInput();
	MovementComp->bOrientRotationToMovement = true;
	MovementComp->bUseControllerDesiredRotation = false;
	SpringArmComp->TargetOffset = FVector::ZeroVector;	
}

void ULockonComponent::ToggleLockon(float Radius)
{
	if (IsValid(CurrentTargetActor))
	{
		EndLockon();
	}
	else
	{
		StartLockon(Radius);
	}
}

bool ULockonComponent::CheckForEnemies(float Radius)
{
	TArray<FHitResult> OutResults;
	FVector CurrentLocation = OwnerRef->GetActorLocation();
	FCollisionShape Sphere = FCollisionShape::MakeSphere(Radius);
	FCollisionQueryParams IgnoreParams{
		FName{TEXT("Ignore Collision Params")},
		false,
		OwnerRef
	};

	bool bHasFoundTarget = GetWorld()->SweepMultiByChannel(
		OutResults,
		CurrentLocation,
		CurrentLocation,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel1,
		Sphere,
		IgnoreParams
	);

	for (const FHitResult& Hit : OutResults)
	{
		if (!Hit.GetActor()->Implements<UEnemy>())
		{
			continue;
		}
		else if (!IsValid(CurrentTargetActor))
		{
			CurrentTargetActor = Hit.GetActor();
			break;
		}
		else if (Hit.GetActor() != CurrentTargetActor)
		{
			EndLockon();
			CurrentTargetActor = Hit.GetActor();
			break;
		}
		else
		{
			EndLockon();
			return false;
		}
	}

	if (!bHasFoundTarget)
	{
		return false;
	}

	return true;
}


// Called every frame
void ULockonComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (IsValid(CurrentTargetActor))
	{
		FVector CurrentLocation = OwnerRef->GetActorLocation();
		FVector TargetLocation = CurrentTargetActor->GetActorLocation();

		// Check distance before lowering target location
		double Distance = FVector::Distance(CurrentLocation, TargetLocation);

		// Angle camera down
		TargetLocation.Z -= 125;

		FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(CurrentLocation, TargetLocation);
		Controller->SetControlRotation(LookAtRotation);

		
		if (Distance > BreakDistance)
		{
			EndLockon();
		}
	}
}

