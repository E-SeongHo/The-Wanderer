//  

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IRetriggerable.generated.h"

UINTERFACE(MinimalAPI)
class URetriggerable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class THEWANDERERPROTO_API IRetriggerable
{
	GENERATED_BODY()

public:
	virtual bool CanRetrigger() const = 0;

	// optional implementation
	virtual void SaveCurrentContext();
};
