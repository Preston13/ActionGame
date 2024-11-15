// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/Enemy.h"
#include "Interfaces/Fighter.h"
#include "Characters/EEnemyState.h"
#include "EnemyCharacter.generated.h"

UCLASS()
class ACTIONCOMBAT_API AEnemyCharacter : public ACharacter, public IEnemy, public IFighter
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	UAnimMontage* DeathAnim;

	UPROPERTY(EditAnywhere)
	TEnumAsByte<EEnemyState> InitialState;

	class UBlackboardComponent* BlackboardComp;

	class AAIController* ControllerRef;

public:
	// Sets default values for this character's properties
	AEnemyCharacter();

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class UStatsComponent* StatsComp;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class UCombatComponent* CombatComp;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
	void HandleDeath();

	UFUNCTION()
	void FinishDeathAnim();

	UFUNCTION(BlueprintCallable)
	void DetectPawn(APawn* PawnDetected, APawn* TargetPawn);

	virtual float GetMeleeRange() override;

	virtual void Attack() override;

	virtual float GetDamage() override;

	virtual float GetAnimationDuration() override;

};
