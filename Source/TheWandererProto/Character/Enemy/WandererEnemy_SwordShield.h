//  

#pragma once

#include "CoreMinimal.h"
#include "Character/Enemy/WandererEnemy.h"
#include "WandererEnemy_SwordShield.generated.h"

/**
 * 
 */
UCLASS()
class THEWANDERERPROTO_API AWandererEnemy_SwordShield : public AWandererEnemy
{
	GENERATED_BODY()

public:
	AWandererEnemy_SwordShield();

protected:
	virtual void BeginPlay() override;
};
