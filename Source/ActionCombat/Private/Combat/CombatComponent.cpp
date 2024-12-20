// Fill out your copyright notice in the Description page of Project Settings.


#include "Combat/CombatComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Interfaces/MainPlayer.h"
#include "Characters/PlayerCharacter.h"
#include "Animations/PlayerAnimInstance.h"

// Sets default values for this component's properties
UCombatComponent::UCombatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UCombatComponent::ComboAttack()
{
	if (!bCanAttack)
	{
		return;
	}

	if (CharacterRef->Implements<UMainPlayer>())
	{
		IMainPlayer* IPlayerRef = Cast<IMainPlayer>(CharacterRef);

		if (IPlayerRef && !IPlayerRef->HasEnoughStamina(AttackStaminaCost))
		{
			return;
		}
	}
	
	bCanAttack = false;
	UPlayerAnimInstance* PlayerAnim = Cast<APlayerCharacter>(CharacterRef)->PlayerAnim;
	PlayerAnim->bIsBlocking = false;

	CharacterRef->PlayAnimMontage(AttackAnimations[ComboCounter]);

	ComboCounter++;
	int MaxCombo = AttackAnimations.Num();

	ComboCounter = UKismetMathLibrary::Wrap(ComboCounter, -1, MaxCombo - 1);

	OnAttackPerformedDelegate.Broadcast(AttackStaminaCost);

	FLatentActionInfo FunctionInfo{
		0,
		101,
		TEXT("ResetComboCounter"),
		this
	};
	UKismetSystemLibrary::RetriggerableDelay(GetWorld(), ResetComboDelay, FunctionInfo);
}

void UCombatComponent::HandleResetAttack()
{
	bCanAttack = true;
}


void UCombatComponent::ResetComboCounter()
{
	ComboCounter = 0;
}

void UCombatComponent::RandomAttack()
{
	int RandomIndex = FMath::RandRange(0, AttackAnimations.Num() - 1);
	AnimDuration = CharacterRef->PlayAnimMontage(AttackAnimations[RandomIndex]);
}

void UCombatComponent::CastSpellAttack()
{
	if (!bCanAttack)
	{
		return;
	}

	if (CharacterRef->Implements<UMainPlayer>())
	{
		IMainPlayer* IPlayerRef = Cast<IMainPlayer>(CharacterRef);

		if (IPlayerRef && !IPlayerRef->HasEnoughStamina(AttackStaminaCost))
		{
			return;
		}
	}

	bCanAttack = false;

	float Duration = CharacterRef->PlayAnimMontage(CastAnimation);

	FTimerHandle CastTimerHandle;
	CharacterRef->GetWorldTimerManager().SetTimer(CastTimerHandle, this, &UCombatComponent::StopCasting, Duration, false);
}

void UCombatComponent::StopCasting()
{
	bCanAttack = true;
}

// Called when the game starts
void UCombatComponent::BeginPlay()
{
	Super::BeginPlay();

	CharacterRef = GetOwner<ACharacter>();
	
}


// Called every frame
void UCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

