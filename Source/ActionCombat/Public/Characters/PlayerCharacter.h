// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/MainPlayer.h"
#include "Interfaces/Fighter.h"
#include "Animations/EFootstepType.h"
#include "PlayerCharacter.generated.h"

UCLASS()
class ACTIONCOMBAT_API APlayerCharacter : public ACharacter, public IMainPlayer, public IFighter
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	UAnimMontage* DeathAnimMontage;

	UPROPERTY(EditAnywhere)
	UAnimMontage* HurtAnimMontage;

public:
	// Sets default values for this character's properties
	APlayerCharacter();

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class UStatsComponent* StatsComp;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class ULockonComponent* LockonComp;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class UCombatComponent* CombatComp;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class UTraceComponent* TraceComp;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class UBlockComponent* BlockComp;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class UPlayerActionsComponent* PlayerActionsComp;

	UPROPERTY(BlueprintReadOnly)
	class UPlayerAnimInstance* PlayerAnim;

	UPROPERTY(EditAnywhere)
	TMap<TEnumAsByte<EFootstepType>, USoundBase*> Footsteps;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;



public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual float GetDamage() override;

	virtual bool HasEnoughStamina(float StaminaCost) override;

	virtual void UpdateIsInAir(bool IsInAir) override;

	UFUNCTION(BlueprintCallable)
	void HandleDeath();

	virtual void EndLockonWithActor(AActor* ActorRef) override;

	virtual bool CanTakeDamage(AActor* Opponent) override;
	
	UFUNCTION(BlueprintCallable)
	void PlayHurtAnim(TSubclassOf<UCameraShakeBase> CameraShakeTemplate);

	virtual bool GetIsBlocking() override;

	void StopCasting();

	UFUNCTION(BlueprintCallable)
	void PlayFootstep();
};
