// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "TPGasAssetManager.generated.h"


UCLASS()
class TPGAS_API UTPGasAssetManager : public UAssetManager
{
	GENERATED_BODY()

public:
	static UTPGasAssetManager& Get();

protected:
	virtual void StartInitialLoading() override;
	
};
