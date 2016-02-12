#pragma once
class IBlockable
{
public:
	void ActivateMove();
	void DeactivateMove();
	bool IsActiveMove();
private:
	bool bIsActive;
};

