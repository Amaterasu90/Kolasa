#include "Kolasa.h"
#include "IBlockable.h"

void IBlockable::ActivateMove() {
	bIsActive = true;
}

void IBlockable::DeactivateMove() {
	bIsActive = false;
}

bool IBlockable::IsActiveMove() {
	return bIsActive;
}