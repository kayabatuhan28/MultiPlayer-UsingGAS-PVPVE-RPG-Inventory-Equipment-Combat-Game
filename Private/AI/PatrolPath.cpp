// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/PatrolPath.h"

APatrolPath::APatrolPath()
{
	PrimaryActorTick.bCanEverTick = false;

	SplineComponent = CreateDefaultSubobject<USplineComponent>("SplineComp",true);
	SetRootComponent(SplineComponent);
	
}


