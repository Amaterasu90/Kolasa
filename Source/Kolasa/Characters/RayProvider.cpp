// Fill out your copyright notice in the Description page of Project Settings.

#include "Kolasa.h"
#include "RayProvider.h"

RayProvider::RayProvider() {
	arrowComponent = NULL;
}

RayProvider::RayProvider(UArrowComponent* arrow){
	arrowComponent = arrow;
}

FVector RayProvider::GetLocation(){
	return arrowComponent ? arrowComponent->GetComponentLocation() : FVector::ZeroVector;
}

FRotator RayProvider::GetRotation() {
	return arrowComponent ? arrowComponent->GetComponentRotation() : FRotator::ZeroRotator;
}

FVector RayProvider::GetRelativeLocation(){
	return arrowComponent->RelativeLocation;
}

RayProvider::~RayProvider(){
}
