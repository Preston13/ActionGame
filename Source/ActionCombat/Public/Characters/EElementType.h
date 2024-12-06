// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EElementDesc : uint8
{
	Default UMETA(DisplayName = "None Selected"),
	Fire UMETA(DisplayName = "Fire"),
	Earth UMETA(DisplayName = "Earth"),
	Water UMETA(DisplayName = "Water"),
	Air UMETA(DisplayName = "Air")
};