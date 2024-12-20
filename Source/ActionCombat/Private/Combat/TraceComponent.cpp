// Fill out your copyright notice in the Description page of Project Settings.


#include "Combat/TraceComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Math/Color.h"
#include "Interfaces/Fighter.h"
#include "Interfaces/MainPlayer.h"
#include "Interfaces/Enemy.h"
#include "Kismet/GameplayStatics.h"
#include "Combat/BlockComponent.h"
#include "Interfaces/Interactable.h"
#include "Characters/EEnemyState.h"
#include "BehaviorTree/BlackboardComponent.h"


// Sets default values for this component's properties
UTraceComponent::UTraceComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UTraceComponent::BeginPlay()
{
	Super::BeginPlay();

	SkeletalComp = GetOwner()->FindComponentByClass<USkeletalMeshComponent>();
	
}


// Called every frame
void UTraceComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);	
	
	if (!bIsAttacking)
	{
		return;
	}

	IMainPlayer* MainPlayerRef = Cast<IMainPlayer>(GetOwner());

	// Trace for attacks
	TArray<FHitResult> AllResults;

	for (const FTraceSockets Socket : Sockets)
	{
		FVector StartSocketLocation = SkeletalComp->GetSocketLocation(Socket.Start);
		FVector EndSocketLocation = SkeletalComp->GetSocketLocation(Socket.End);
		FQuat ShapeRotation = SkeletalComp->GetSocketQuaternion(Socket.Rotation);

		float BoxCollisionLength = FVector::Distance(StartSocketLocation, EndSocketLocation);

		FVector BoxCollisionSize = FVector::ZeroVector;
		BoxCollisionSize = FVector(BoxCollisionHeight, BoxCollisionWidth, BoxCollisionLength);
		BoxCollisionSize /= 2;
		FCollisionShape Box = FCollisionShape::MakeBox(BoxCollisionSize);
		TArray<FHitResult> OutResults;
		FCollisionQueryParams IgnoreParams{
			FName { TEXT("Ignore Params") },
			false,
			GetOwner()
		};
		bool bHasFoundTargets = GetWorld()->SweepMultiByChannel(
			OutResults,
			StartSocketLocation,
			EndSocketLocation,
			ShapeRotation,
			ECollisionChannel::ECC_GameTraceChannel1,
			Box,
			IgnoreParams
		);

		for (FHitResult Hit : OutResults)
		{
			AllResults.Add(Hit);
		}

		if (bDebugMode)
		{
			FVector CenterPoint = UKismetMathLibrary::VLerp(StartSocketLocation, EndSocketLocation, 0.5f);
			UKismetSystemLibrary::DrawDebugBox(
				GetWorld(),
				CenterPoint,
				Box.GetExtent(),
				bHasFoundTargets ? FLinearColor::Green : FLinearColor::Red,
				ShapeRotation.Rotator(),
				1.0f,
				2.0f
			);
		}
	}

	if (AllResults.Num() == 0)
	{
		return;
	}

	float CharacterDamage = 0.0f;

	IFighter* FighterRef = Cast<IFighter>(GetOwner());

	if (FighterRef)
	{
		CharacterDamage = FighterRef->GetDamage();
	}

	FDamageEvent TargetAttackedEvent;
	for (const FHitResult& Hit : AllResults)
	{
		AActor* TargetActor = Hit.GetActor();
		UBlockComponent* BlockComp = TargetActor->GetComponentByClass<UBlockComponent>();

		//Enemies can't attack other enemies
		if (!Cast<IMainPlayer>(TargetActor) && !Cast<IMainPlayer>(GetOwner()))
		{
			TargetsToIgnore.Add(TargetActor);
		}

		if (!TargetsToIgnore.Contains(TargetActor))
		{
			TargetActor->TakeDamage(CharacterDamage, TargetAttackedEvent, GetOwner()->GetInstigatorController(), GetOwner());

			TargetsToIgnore.Add(TargetActor);

			if (GetOwner()->Implements<UMainPlayer>())
			{
				UGameplayStatics::PlaySoundAtLocation(GetWorld(), HitSound, Hit.ImpactPoint);
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitParticlesTemplate, Hit.ImpactPoint);
			}
			else if (BlockComp->Check(GetOwner<AActor>()))
			{				
				UGameplayStatics::PlaySoundAtLocation(GetWorld(), HitSound, Hit.ImpactPoint);
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitParticlesTemplate, Hit.ImpactPoint);
			}
			else
			{
				IEnemy* EnemyRef = Cast<IEnemy>(GetOwner());
				if (EnemyRef)
				{
					class UBlackboardComponent* BlackboardComp = EnemyRef->BlackboardComp;
					BlackboardComp->SetValueAsEnum(TEXT("CurrentState"), EEnemyState::Knockback);
				}
			}
		}
	}
}

void UTraceComponent::HandleResetAttack()
{
	TargetsToIgnore.Empty();
}

