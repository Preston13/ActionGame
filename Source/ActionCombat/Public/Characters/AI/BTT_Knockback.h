// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_Knockback.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONCOMBAT_API UBTT_Knockback : public UBTTaskNode
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	class UAnimMontage* KnockbackAnim;
	
public:
	virtual EBTNodeResult::Type ExecuteTask(
		UBehaviorTreeComponent& OwnerComp,
		uint8* NodeMemory
	) override;
};
