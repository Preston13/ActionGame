// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/AI/BTT_RangeAttack.h"
#include "AIController.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetMathLibrary.h"

EBTNodeResult::Type UBTT_RangeAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ACharacter* CharacterRef = OwnerComp.GetAIOwner()->GetPawn<ACharacter>();

	if (!IsValid(CharacterRef))
	{
		return EBTNodeResult::Failed;
	}

	CharacterRef->PlayAnimMontage(AnimMontage);

	double RandomValue = UKismetMathLibrary::RandomFloat();

	if (RandomValue > Threshold)
	{
		Threshold = 0.9;
		UE_LOG(LogTemp, Warning, TEXT("Chaaaaaarge!"));
	}
	else 
	{
		Threshold -= 0.1;
	}

	return EBTNodeResult::Succeeded;
}
