// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

/**
 * 
 */
class KOLASA_API RayProvider
{
public:
	RayProvider();
	RayProvider(UArrowComponent* arrow);
	FVector GetLocation();
	FRotator GetRotation();
	~RayProvider();
private:
	UArrowComponent* _arrowComponent;
};
