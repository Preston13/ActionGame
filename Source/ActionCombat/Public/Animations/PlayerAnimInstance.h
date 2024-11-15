// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Animations/EFootstepType.h"
#include "PlayerAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONCOMBAT_API UPlayerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void HandleUpdatedTarget(AActor* NewTargetActorRef);

	UFUNCTION(BlueprintCallable)
	void UpdatedDirection();

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bIsJumping = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bIsInAir = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bIsBlocking = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bIsInCombat = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bIsCasting = false;


protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float CurrentSpeed = 0.0f;

	UFUNCTION(BlueprintCallable)
	void UpdateSpeed();

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float CurrentDirection = 0.0f;	
	
};
