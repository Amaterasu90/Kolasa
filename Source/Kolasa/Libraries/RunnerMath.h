// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

/**
 * 
 */
class KOLASA_API RunnerMath
{
public:
	static FVector GetCleared(FVector vector, float tolerance);
	static FRotator Align(FRotator value);
};
