// Fill out your copyright notice in the Description page of Project Settings.

#include "Kolasa.h"
#include "RayProvider.h"

RayProvider::RayProvider() {
	_arrowComponent = NULL;
}

RayProvider::RayProvider(UArrowComponent* arrow){
	_arrowComponent = arrow;
}

FVector RayProvider::GetLocation(){
	return _arrowComponent ? _arrowComponent->GetComponentLocation() : FVector::ZeroVector;
}

FRotator RayProvider::GetRotation() {
	return _arrowComponent ? _arrowComponent->GetComponentRotation() : FRotator::ZeroRotator;
}

FVector RayProvider::GetRelativeLocation(){
	return _arrowComponent->RelativeLocation;
}

RayProvider::~RayProvider(){
}
