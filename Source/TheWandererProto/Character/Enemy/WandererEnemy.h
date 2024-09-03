//  

#pragma once

#include "CoreMinimal.h"
#include "Character/WandererBaseCharacter.h"
#include "WandererEnemy.generated.h"

class UAIPerceptionStimuliSourceComponent;
class UBehaviorTree;
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

	FName GetName() const;
	
	virtual void Tick(float DeltaSeconds) override;

	virtual void Die() override;

	void SetUIRender(bool bIsTargeted);
	
protected:
	virtual void BeginPlay() override;
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = UI, meta = (AllowPrivateAccess = "true"))
	FName Name;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = UI, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UWidgetComponent> WidgetComponent;
};
