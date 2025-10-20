// Fill out your copyright notice in the Description page of Project Settings.


#include "TPGasAssetManager.h"

#include "TPGasGameplayTags.h"

UTPGasAssetManager& UTPGasAssetManager::Get()
{
	check(GEngine)
	
	UTPGasAssetManager* TPGasAssetManager = Cast<UTPGasAssetManager>(GEngine->AssetManager);
	return *TPGasAssetManager;
}

void UTPGasAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();

	FTpGasGameplayTags::InitializeNativeGameplayTags();
	
}
