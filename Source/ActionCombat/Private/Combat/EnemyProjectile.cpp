// Fill out your copyright notice in the Description page of Project Settings.


#include "Combat/EnemyProjectile.h"
#include "Particles/ParticleSystemComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AEnemyProjectile::AEnemyProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AEnemyProjectile::BeginPlay()
{
	Super::BeginPlay();
	
	FTimerHandle DeathTimerHandle;
	GetWorldTimerManager().SetTimer(DeathTimerHandle, this, &AEnemyProjectile::DestroyProjectile, 5.0f);
}

// Called every frame
void AEnemyProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnemyProjectile::HandleBeginOverlap(AActor* OtherActor)
{
	APawn* PawnRef = Cast<APawn>(OtherActor);

	if (!IsValid(PawnRef))
	{
		FindComponentByClass<UParticleSystemComponent>()->SetTemplate(HitTemplate);
		FindComponentByClass<UProjectileMovementComponent>()->StopMovementImmediately();

		FTimerHandle DeathTimerHandle;
		GetWorldTimerManager().SetTimer(DeathTimerHandle, this, &AEnemyProjectile::DestroyProjectile, 0.5f);

		FindComponentByClass<USphereComponent>()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		UGameplayStatics::PlaySoundAtLocation(GetWorld(), ImpactSound, OtherActor->GetActorLocation());
	}

	if ((!PawnRef->IsPlayerControlled() && !bIsPlayerProjectile) || (PawnRef->IsPlayerControlled() && bIsPlayerProjectile))
	{
		return;
	}

	FindComponentByClass<UParticleSystemComponent>()->SetTemplate(HitTemplate);
	FindComponentByClass<UProjectileMovementComponent>()->StopMovementImmediately();	

	FTimerHandle DeathTimerHandle;
	GetWorldTimerManager().SetTimer(DeathTimerHandle, this, &AEnemyProjectile::DestroyProjectile, 0.5f);

	FindComponentByClass<USphereComponent>()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	FDamageEvent TargetAttackedEvent;
	PawnRef->TakeDamage(Damage, TargetAttackedEvent, PawnRef->GetController(), this);
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), ImpactSound, OtherActor->GetActorLocation());
}

void AEnemyProjectile::DestroyProjectile()
{
	Destroy();
}

