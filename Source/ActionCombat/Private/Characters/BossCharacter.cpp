// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/BossCharacter.h"
#include "Characters/StatsComponent.h"
#include "Combat/CombatComponent.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Characters/PlayerCharacter.h"
#include "BrainComponent.h"
#include "Components/CapsuleComponent.h"
#include "Interfaces/MainPlayer.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/AmbientSound.h"
#include "Components/AudioComponent.h"

// Sets default values
ABossCharacter::ABossCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StatsComp = CreateDefaultSubobject<UStatsComponent>(TEXT("Stats Component"));
	CombatComp = CreateDefaultSubobject<UCombatComponent>(TEXT("Combat Component"));
}

// Called when the game starts or when spawned
void ABossCharacter::BeginPlay()
{
	Super::BeginPlay();

	ControllerRef = GetController<AAIController>();

	BlackboardComp = ControllerRef->GetBlackboardComponent();

	BlackboardComp->SetValueAsEnum(TEXT("CurrentState"), InitialState);

	GetWorld()->GetFirstPlayerController()->GetPawn<APlayerCharacter>()
		->StatsComp->OnZeroHealthDelegate.AddDynamic(this, &ABossCharacter::HandlePlayerDeath);

	TSubclassOf<AAmbientSound> AmbientSound;
	AmbientSound = AAmbientSound::StaticClass();
	TArray<AActor*> MusicArray;
	UGameplayStatics::GetAllActorsOfClassWithTag(GetWorld(), AmbientSound, "Music", MusicArray);

	if (!MusicArray.IsEmpty())
	{
		BackgroundMusic = Cast<AAmbientSound>(MusicArray[0])->GetComponentByClass<UAudioComponent>();
		PreviousMusic = BackgroundMusic->GetSound();
	}
}

// Called every frame
void ABossCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABossCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ABossCharacter::DetectPawn(APawn* PawnDetected, APawn* TargetPawn)
{
	EEnemyState CurrentState = static_cast<EEnemyState>(BlackboardComp->GetValueAsEnum(TEXT("CurrentState")));
	if (PawnDetected != TargetPawn || CurrentState != EEnemyState::Idle)
	{
		return;
	}

	BlackboardComp->SetValueAsEnum(TEXT("CurrentState"), EEnemyState::Range);

	if (BackgroundMusic)
	{
		BackgroundMusic->Stop();
		BackgroundMusic->SetSound(BossMusic);
		BackgroundMusic->Play();
	}
}

float ABossCharacter::GetDamage()
{
	return StatsComp->Stats[EStat::Strength];
}

void ABossCharacter::Attack()
{
	CombatComp->RandomAttack();
}

float ABossCharacter::GetAnimationDuration()
{
	return CombatComp->AnimDuration;
}

float ABossCharacter::GetMeleeRange()
{
	return StatsComp->Stats[EStat::MeleeRange];
}

float ABossCharacter::GetIdleRange()
{
	return StatsComp->Stats[EStat::IdleRange];
}

void ABossCharacter::HandlePlayerDeath()
{
	ControllerRef->GetBlackboardComponent()->SetValueAsEnum(TEXT("CurrentState"), EEnemyState::GameOver);
}

void ABossCharacter::HandleDeath()
{
	float Duration = PlayAnimMontage(DeathAnim);
	ControllerRef->GetBrainComponent()->StopLogic("Defeated");
	FindComponentByClass<UCapsuleComponent>()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	FTimerHandle DeathTimerHandle;
	GetWorldTimerManager().SetTimer(DeathTimerHandle, this, &ABossCharacter::FinishDeathAnim, Duration + 5.0f, false);

	IMainPlayer* PlayerRef = GetWorld()->GetFirstPlayerController()->GetPawn<IMainPlayer>();

	if (PlayerRef)
	{
		PlayerRef->EndLockonWithActor(this);
	}	
}

void ABossCharacter::FinishDeathAnim()
{
	StopMusic();
	Destroy();
}

void ABossCharacter::LeaveCombat()
{
	StopMusic();
	StatsComp->LeaveCombat();
}

void ABossCharacter::StopMusic()
{
	if (BackgroundMusic)
	{
		BackgroundMusic->Stop();
		BackgroundMusic->SetSound(PreviousMusic);
		BackgroundMusic->Play();
	}
}

