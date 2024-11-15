// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/PlayerCharacter.h"
#include "Animations/PlayerAnimInstance.h"
#include "Characters/StatsComponent.h"
#include "Characters/EStat.h"
#include "Combat/LockonComponent.h"
#include "Combat/CombatComponent.h"
#include "Combat/TraceComponent.h"
#include "Combat/BlockComponent.h"
#include "Characters/PlayerActionsComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StatsComp = CreateDefaultSubobject<UStatsComponent>(TEXT("Stats Component"));
	LockonComp = CreateDefaultSubobject<ULockonComponent>(TEXT("Lockon Component"));
	CombatComp = CreateDefaultSubobject<UCombatComponent>(TEXT("Combat Component"));
	TraceComp = CreateDefaultSubobject<UTraceComponent>(TEXT("Trace Component"));
	BlockComp = CreateDefaultSubobject<UBlockComponent>(TEXT("Block Component"));
	PlayerActionsComp = CreateDefaultSubobject<UPlayerActionsComponent>(TEXT("Player Actions Component"));
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	PlayerAnim = Cast<UPlayerAnimInstance>(GetMesh()->GetAnimInstance());
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

float APlayerCharacter::GetDamage()
{
	return StatsComp->Stats[EStat::Strength];
}

bool APlayerCharacter::HasEnoughStamina(float StaminaCost)
{
	return StaminaCost <= StatsComp->Stats[EStat::Stamina];
}

void APlayerCharacter::UpdateIsInAir(bool IsInAir)
{
	PlayerAnim->bIsInAir = IsInAir;
}

void APlayerCharacter::HandleDeath()
{
	PlayAnimMontage(DeathAnimMontage);
	DisableInput(GetController<APlayerController>());
}

void APlayerCharacter::EndLockonWithActor(AActor* ActorRef)
{
	if (ActorRef != LockonComp->CurrentTargetActor)
	{
		return;
	}

	LockonComp->EndLockon();
}

bool APlayerCharacter::CanTakeDamage(AActor* Opponent)
{
	if (PlayerActionsComp->bIsDodgeActive)
	{
		return false;
	}
	if (PlayerAnim->bIsBlocking)
	{
		return BlockComp->Check(Opponent);
	}
	return true;
}

void APlayerCharacter::PlayHurtAnim(TSubclassOf<UCameraShakeBase> CameraShakeTemplate)
{
	PlayAnimMontage(HurtAnimMontage);

	if (CameraShakeTemplate)
	{
		GetController<APlayerController>()->ClientStartCameraShake(CameraShakeTemplate);
	}
}

bool APlayerCharacter::GetIsBlocking()
{
	return PlayerAnim->bIsBlocking;
}

void APlayerCharacter::StopCasting()
{
	PlayerAnim->bIsCasting = false;
}

void APlayerCharacter::PlayFootstep()
{
	FVector FootstepLocation = { GetActorLocation().X, GetActorLocation().Y, GetActorLocation().Z - 100 };
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), Footsteps[EFootstepType::Dirt], FootstepLocation);
}

