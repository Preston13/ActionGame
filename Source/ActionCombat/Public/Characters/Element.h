// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/Interactable.h"
#include "Characters/EElementType.h"
#include "Characters/PlayerCharacter.h"
#include "Element.generated.h"

UCLASS()
class ACTIONCOMBAT_API AElement : public AActor, public IInteractable
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AElement();

	UPROPERTY(EditAnywhere)
	TEnumAsByte<EElementDesc> Type;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	virtual void Interact(APlayerCharacter* PlayerRef) override;
};
