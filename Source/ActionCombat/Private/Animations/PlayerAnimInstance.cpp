// Fill out your copyright notice in the Description page of Project Settings.


#include "Animations/PlayerAnimInstance.h"

void UPlayerAnimInstance::HandleUpdatedTarget(AActor* NewTargetActorRef)
{
	bIsInCombat = IsValid(NewTargetActorRef);
}

void UPlayerAnimInstance::UpdatedDirection()
{
	APawn* PawnRef = TryGetPawnOwner();

	if (!IsValid(PawnRef)) 
	{
		return;
	}

	if (!bIsInCombat)
	{
		return;
	}

	CurrentDirection = CalculateDirection(PawnRef->GetVelocity(), PawnRef->GetActorRotation());
}

void UPlayerAnimInstance::UpdateSpeed()
{
	APawn* PawnRef = TryGetPawnOwner();

	if (!IsValid(PawnRef))
	{
		return;
	}

	CurrentSpeed = static_cast<float>(PawnRef->GetVelocity().Length());
}
