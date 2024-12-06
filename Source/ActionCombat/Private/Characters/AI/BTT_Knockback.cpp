// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/AI/BTT_Knockback.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "Characters/EEnemyState.h"
#include "GameFramework/Character.h"

EBTNodeResult::Type UBTT_Knockback::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ACharacter* CharacterRef = OwnerComp.GetAIOwner()->GetPawn<ACharacter>();
	if (!IsValid(CharacterRef))
	{
		return EBTNodeResult::Failed;
	}

	CharacterRef->PlayAnimMontage(KnockbackAnim);

	OwnerComp.GetBlackboardComponent()->SetValueAsEnum(TEXT("CurrentState"), EEnemyState::Melee);

	return EBTNodeResult::Succeeded;
}
