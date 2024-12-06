// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum EFootstepType
{
	Default UMETA(DisplayName = "None Selected"),
	Dirt UMETA(DisplayName = "Dirt"),
	Wood UMETA(DisplayName = "Wood"),
	Water UMETA(DisplayName = "Water")
};
