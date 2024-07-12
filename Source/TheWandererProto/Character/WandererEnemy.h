//  

#pragma once

#include "CoreMinimal.h"
#include "Character/WandererBaseCharacter.h"
#include "WandererEnemy.generated.h"

/**
 * 
 */
UCLASS()
class THEWANDERERPROTO_API AWandererEnemy : public AWandererBaseCharacter
{
	GENERATED_BODY()

public:
	AWandererEnemy();

protected:
	virtual void BeginPlay() override;

	
};
