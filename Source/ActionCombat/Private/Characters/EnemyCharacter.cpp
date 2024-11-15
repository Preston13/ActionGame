// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/EnemyCharacter.h"
#include "AIController.h"
#include "BrainComponent.h"
#include "Components/CapsuleComponent.h"
#include "Interfaces/MainPlayer.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Characters/StatsComponent.h"
#include "Combat/CombatComponent.h"


// Sets default values
AEnemyCharacter::AEnemyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StatsComp = CreateDefaultSubobject<UStatsComponent>(TEXT("Stats Component"));
	CombatComp = CreateDefaultSubobject<UCombatComponent>(TEXT("Combat Component"));
}

// Called when the game starts or when spawned
void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	ControllerRef = GetController<AAIController>();

	BlackboardComp = ControllerRef->GetBlackboardComponent();

	BlackboardComp->SetValueAsEnum(TEXT("CurrentState"), InitialState);
	
}

// Called every frame
void AEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEnemyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemyCharacter::HandleDeath()
{
	float Duration = PlayAnimMontage(DeathAnim);
	ControllerRef->GetBrainComponent()->StopLogic("Defeated");
	FindComponentByClass<UCapsuleComponent>()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	FTimerHandle DeathTimerHandle;
	GetWorldTimerManager().SetTimer(DeathTimerHandle, this, &AEnemyCharacter::FinishDeathAnim, Duration + 5.0f, false);

	IMainPlayer* PlayerRef = GetWorld()->GetFirstPlayerController()->GetPawn<IMainPlayer>();

	if (PlayerRef)
	{
		PlayerRef->EndLockonWithActor(this);
	}
}

void AEnemyCharacter::FinishDeathAnim()
{
	Destroy();
}

void AEnemyCharacter::DetectPawn(APawn* PawnDetected, APawn* TargetPawn)
{
	EEnemyState CurrentState = static_cast<EEnemyState>(BlackboardComp->GetValueAsEnum(TEXT("CurrentState")));
	if (PawnDetected != TargetPawn || (CurrentState != EEnemyState::Idle && CurrentState != EEnemyState::Range))
	{
		return;
	}

	BlackboardComp->SetValueAsEnum(TEXT("CurrentState"), EEnemyState::Melee);
}

float AEnemyCharacter::GetMeleeRange()
{
	return StatsComp->Stats[EStat::MeleeRange];
}

void AEnemyCharacter::Attack()
{
	CombatComp->RandomAttack();
}

float AEnemyCharacter::GetDamage()
{
	return StatsComp->Stats[EStat::Strength];
}

float AEnemyCharacter::GetAnimationDuration()
{
	return CombatComp->AnimDuration;
}

