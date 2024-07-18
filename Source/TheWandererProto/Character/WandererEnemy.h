//  

#pragma once

#include "CoreMinimal.h"
#include "Character/WandererBaseCharacter.h"
#include "WandererEnemy.generated.h"

class UWidgetComponent;
/**
 * 
 */
UCLASS()
class THEWANDERERPROTO_API AWandererEnemy : public AWandererBaseCharacter
{
	GENERATED_BODY()

public:
	AWandererEnemy();

	virtual void Tick(float DeltaSeconds) override;
	
	void SetUIRender(bool bIsTargeting);

protected:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = UI, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UWidgetComponent> WidgetComponent;
};
