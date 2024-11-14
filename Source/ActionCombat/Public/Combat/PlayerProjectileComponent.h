// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Interfaces/Projectile.h"
#include "PlayerProjectileComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ACTIONCOMBAT_API UPlayerProjectileComponent : public UActorComponent, public IProjectile
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPlayerProjectileComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	virtual void SpawnProjectile(FName ComponentName, TSubclassOf<AActor> ProjectileClass) override;

	UFUNCTION()
	void StopCasting();
		
};
