#include "Kolasa.h"
#include "IBlockable.h"

void IBlockable::BlockMove() {
	bIsActive = false;
}

void IBlockable::UnlockMove() {
	bIsActive = true;
}

bool IBlockable::IsActive() {
	return bIsActive;
}