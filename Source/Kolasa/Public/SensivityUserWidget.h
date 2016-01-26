// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "SensivityUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class KOLASA_API USensivityUserWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual FReply NativeOnAnalogValueChanged(const FGeometry & InGeometry, const FAnalogInputEvent & InAnalogEvent) override;
};
