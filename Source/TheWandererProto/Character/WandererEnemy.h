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

	virtual void PossessedBy(AController* NewController) override;
	virtual void Tick(float DeltaSeconds) override;
	
	void SetUIRender(bool bIsTargeting);
	
protected:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = UI, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UWidgetComponent> WidgetComponent;

	UPROPERTY(EditAnywhere, Category = AI, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UBehaviorTree> BehaviorTree;

	// testing
public:
	UPROPERTY()
	TObjectPtr<AWandererBaseCharacter> CombatTarget;
};
