// Fill out your copyright notice in the Description page of Project Settings.


#include "Animations/BossAnimInstance.h"

void UBossAnimInstance::UpdateSpeed()
{
	APawn* PawnRef = TryGetPawnOwner();

	if (!IsValid(PawnRef))
	{
		return;
	}

	CurrentSpeed = static_cast<float>(PawnRef->GetVelocity().Length());
}
