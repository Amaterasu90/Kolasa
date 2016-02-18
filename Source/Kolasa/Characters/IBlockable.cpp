#include "Kolasa.h"
#include "IBlockable.h"

void IBlockable::ActivateMove() {
	this->bIsActive = true;
}

void IBlockable::DeactivateMove(){
	this->bIsActive = false;
}

bool IBlockable::IsActiveMove() {
	return bIsActive;
}