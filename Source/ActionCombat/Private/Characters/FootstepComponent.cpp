// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/FootstepComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "Components/SkeletalMeshComponent.h"

// Sets default values for this component's properties
UFootstepComponent::UFootstepComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UFootstepComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UFootstepComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UFootstepComponent::PlayFootstep(FName StepSocket)
{
	AActor* CharacterRef = GetOwner();
	if (!CharacterRef)
	{
		return;
	}

	FVector FootstepLocation = { CharacterRef->GetActorLocation().X, CharacterRef->GetActorLocation().Y, CharacterRef->GetActorLocation().Z - 100 };
	EFootstepType Type = EFootstepType::Default;
	USkeletalMeshComponent* MeshComp = CharacterRef->GetComponentByClass<USkeletalMeshComponent>();

	if (!MeshComp)
	{
		return;
	}

	FTransform SocketTransform = MeshComp->GetSocketTransform(StepSocket);
	FVector SocketLocation = SocketTransform.GetLocation();
	FVector SocketDownVector = -SocketTransform.GetRotation().GetUpVector();

	FHitResult HitResult;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(CharacterRef);
	CollisionParams.bReturnPhysicalMaterial = true;

	FVector EndLocation = SocketLocation + SocketDownVector * 300;
	bool bHit = GetWorld()->LineTraceSingleByChannel(
		HitResult,
		SocketLocation,
		EndLocation,
		ECollisionChannel::ECC_WorldStatic,
		CollisionParams
	);

	if (bHit)
	{
		UPhysicalMaterial* PhysicsMat = HitResult.PhysMaterial.Get();
		if (!IsValid(PhysicsMat))
		{
			return;
		}

		EPhysicalSurface GroundType = PhysicsMat->SurfaceType;

		switch (GroundType) {
		case EPhysicalSurface::SurfaceType1:
			Type = EFootstepType::Dirt;
			break;

		case EPhysicalSurface::SurfaceType2:
			Type = EFootstepType::Wood;
			break;

		case EPhysicalSurface::SurfaceType4:
			Type = EFootstepType::Water;
			break;

		default:
			Type = EFootstepType::Dirt;
		}
	}
	else
	{
	}
	
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), Footsteps[Type], FootstepLocation);
}

