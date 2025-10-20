// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/TPGasUserWidget.h"

void UTPGasUserWidget::SetWidgetController(UObject* InWidgetController)
{
	WidgetController = InWidgetController;
	WidgetControllerSet();
}
