// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/PlayerActionsComponent.h"
#include "GameFramework/Character.h"
#include "Interfaces/MainPlayer.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Characters/PlayerCharacter.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values for this component's properties
UPlayerActionsComponent::UPlayerActionsComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	CharacterRef = GetOwner<ACharacter>();

	if (!IsValid(CharacterRef))
	{
		return;
	}
	MovementComp = CharacterRef->GetCharacterMovement();

	if (CharacterRef->Implements<UMainPlayer>())
	{
		IPlayerRef = Cast<IMainPlayer>(CharacterRef);
	}
}


// Called when the game starts
void UPlayerActionsComponent::BeginPlay()
{
	Super::BeginPlay();

	MovementComp->MaxWalkSpeed = WalkSpeed;
	
}


// Called every frame
void UPlayerActionsComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	APlayerCharacter* PlayerCharacterRef = Cast<APlayerCharacter>(CharacterRef);
	if (!PlayerCharacterRef)
	{
		return;
	}
	PlayerCharacterRef->UpdateIsInAir(MovementComp->IsFalling());
}

void UPlayerActionsComponent::Sprint()
{
	if (!IPlayerRef->HasEnoughStamina(SprintCost))
	{
		Walk();
		return;
	}

	if (MovementComp->Velocity.Equals(FVector::ZeroVector, 1))
	{
		return;
	}

	MovementComp->MaxWalkSpeed = SprintSpeed;

	OnSprintDelegate.Broadcast(SprintCost);
}

void UPlayerActionsComponent::Walk()
{
	MovementComp->MaxWalkSpeed = WalkSpeed;
}

void UPlayerActionsComponent::Dodge()
{
	if (bIsDodgeActive || !IPlayerRef->HasEnoughStamina(DodgeCost))
	{
		return;
	}

	bIsDodgeActive = true;

	OnDodgeDelegate.Broadcast(DodgeCost);

	FVector Direction = CharacterRef->GetCharacterMovement()->Velocity.Length() < 1 ? CharacterRef->GetActorForwardVector() : CharacterRef->GetLastMovementInputVector();
	FRotator NewRot = UKismetMathLibrary::MakeRotFromX(Direction);

	CharacterRef->SetActorRotation(NewRot);
	float Duration = CharacterRef->PlayAnimMontage(DodgeAnimMontage);

	FTimerHandle DodgeTimerHandle;
	CharacterRef->GetWorldTimerManager().SetTimer(DodgeTimerHandle, this, &UPlayerActionsComponent::FinishDodgeAnim, Duration, false);
}

void UPlayerActionsComponent::FinishDodgeAnim()
{
	bIsDodgeActive = false;
}

