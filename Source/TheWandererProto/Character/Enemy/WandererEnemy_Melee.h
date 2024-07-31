//  

#pragma once

#include "CoreMinimal.h"
#include "WandererEnemy.h"
#include "WandererEnemy_Melee.generated.h"

class AWandererSword;
/**
 * 
 */
UCLASS()
class THEWANDERERPROTO_API AWandererEnemy_Melee : public AWandererEnemy
{
	GENERATED_BODY()

public:
	AWandererEnemy_Melee();

private:
	// Set in BluePrint
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AWandererSword> SwordType;
};
