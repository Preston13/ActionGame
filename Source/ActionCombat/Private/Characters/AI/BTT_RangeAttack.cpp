// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/AI/BTT_RangeAttack.h"
#include "AIController.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetMathLibrary.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Characters/EEnemyState.h"
#include "Interfaces/Fighter.h"

EBTNodeResult::Type UBTT_RangeAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ACharacter* CharacterRef = OwnerComp.GetAIOwner()->GetPawn<ACharacter>();
	IFighter* FighterRef = Cast<IFighter>(CharacterRef);

	if (!IsValid(CharacterRef))
	{
		return EBTNodeResult::Failed;
	}

	float Distance = OwnerComp.GetBlackboardComponent()->GetValueAsFloat(TEXT("Distance"));
	if (Distance < FighterRef->GetMeleeRange())
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsEnum(TEXT("CurrentState"), EEnemyState::Melee);
		AbortTask(OwnerComp, NodeMemory);
		return EBTNodeResult::Aborted;
	}
	else if (Distance > FighterRef->GetIdleRange())
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsEnum(TEXT("CurrentState"), EEnemyState::Idle);
		AbortTask(OwnerComp, NodeMemory);
		return EBTNodeResult::Aborted;
	}

	CharacterRef->PlayAnimMontage(AnimMontage);

	double RandomValue = UKismetMathLibrary::RandomFloat();

	if (RandomValue > Threshold)
	{
		Threshold = 0.9;
		
		OwnerComp.GetBlackboardComponent()->SetValueAsEnum(TEXT("CurrentState"), EEnemyState::Charge);
	}
	else 
	{
		Threshold -= 0.1;
	}

	return EBTNodeResult::Succeeded;
}
