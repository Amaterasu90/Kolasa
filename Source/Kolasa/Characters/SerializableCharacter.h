// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Characters/MyCharacter.h"
#include "SerializableCharacter.generated.h"

/**
 * 
 */
UCLASS()
class KOLASA_API ASerializableCharacter : public AMyCharacter
{
	GENERATED_BODY()
	
public:

	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;
};
