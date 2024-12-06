// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/PlayerActionsComponent.h"
#include "GameFramework/Character.h"
#include "Interfaces/MainPlayer.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Characters/PlayerCharacter.h"
#include "Kismet/KismetMathLibrary.h"
#include "Interfaces/Interactable.h"

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

void UPlayerActionsComponent::Interact()
{
	// Trace for interactable actors		
	if (IPlayerRef)
	{
		TArray<FHitResult> OutResults;
		FVector CurrentLocation = GetOwner()->GetActorLocation();
		FVector EndLocation = { CurrentLocation.X, CurrentLocation.Y, CurrentLocation.Z + 1 };
		FCollisionShape Sphere = FCollisionShape::MakeSphere(100);
		FCollisionQueryParams IgnoreParams{
			FName{TEXT("Ignore Collision Params")},
			false,
			GetOwner()
		};

		bool bHasFoundTarget = GetWorld()->SweepMultiByChannel(
			OutResults,
			CurrentLocation,
			EndLocation,
			FQuat::Identity,
			ECollisionChannel::ECC_GameTraceChannel2,
			Sphere,
			IgnoreParams
		);

		if (!bHasFoundTarget)
		{
			UE_LOG(LogTemp, Warning, TEXT("Nothing"));
			return;
		}

		for (const FHitResult& Hit : OutResults)
		{
			UE_LOG(LogTemp, Warning, TEXT("interactable?"));
			IInteractable* InteractableActor = Cast<IInteractable>(Hit.GetActor());
			if (!InteractableActor)
			{
				UE_LOG(LogTemp, Warning, TEXT("Not interactable"));
				continue;
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Interact"));
				InteractableActor->Interact(Cast<APlayerCharacter>(GetOwner()));
			}
		}

		/*FVector OwnerLocation = GetOwner()->GetActorLocation();
		FVector StartLocation = { OwnerLocation.X, OwnerLocation.Y, OwnerLocation.Z - 50 };
		FVector EndLocation = { OwnerLocation.X, OwnerLocation.Y, OwnerLocation.Z + 100 };
		FQuat TraceRotation = GetOwner()->GetActorRotation().Quaternion();

		float TraceBoxLength = FVector::Distance(StartLocation, EndLocation);

		FVector TraceBoxSize = FVector::ZeroVector;
		TraceBoxSize = FVector(200, 100, TraceBoxLength);
		TraceBoxSize /= 2;
		FCollisionShape Box = FCollisionShape::MakeBox(TraceBoxSize);
		FHitResult HitResult;
		FCollisionQueryParams CollisionParams;
		CollisionParams.AddIgnoredActor(GetOwner());
		bool bHit = GetWorld()->SweepSingleByChannel(
			HitResult,
			StartLocation,
			EndLocation,
			TraceRotation,
			ECollisionChannel::ECC_GameTraceChannel2,
			Box,
			CollisionParams
		);

		if (bDebugMode)
		{
			FVector CenterPoint = UKismetMathLibrary::VLerp(StartLocation, EndLocation, 0.5f);
			UKismetSystemLibrary::DrawDebugBox(
				GetWorld(),
				CenterPoint,
				Box.GetExtent(),
				bHit ? FLinearColor::Green : FLinearColor::Red,
				TraceRotation.Rotator(),
				.0167f,
				2.0f
			);
		}

		if (!bHit)
		{
			UE_LOG(LogTemp, Warning, TEXT("No hit"));
			return;
		}

		UE_LOG(LogTemp, Warning, TEXT("%s"), *HitResult.GetActor()->GetName());

		IInteractable* InteractableActor = Cast<IInteractable>(HitResult.GetActor());
		if (!InteractableActor)
		{
			return;
		}

		InteractableActor->Interact(Cast<APlayerCharacter>(GetOwner())); */
	}
}

