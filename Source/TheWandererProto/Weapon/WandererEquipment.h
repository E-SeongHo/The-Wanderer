//  

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/Actor.h"
#include "WandererEquipment.generated.h"

class AWandererBaseCharacter;

UCLASS(Abstract)
class THEWANDERERPROTO_API AWandererEquipment : public AActor
{
	GENERATED_BODY()
	
public:	
	AWandererEquipment();

	bool IsDrawn() const;
	virtual void InitializeWithOwner(AWandererBaseCharacter* InOwner);

	virtual void OnDraw();
	virtual void OnSheath();
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Owner, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<AWandererBaseCharacter> EquipmentOwner;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Equipment, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USceneComponent> Root;

	TWeakObjectPtr<UStaticMeshComponent> EquipmentMesh;

	FGameplayTag EquippedTag;
	FGameplayTag DrawnTag;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Socket, meta = (AllowPrivateAccess = "true"))
	FName EquipSocket;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Socket, meta = (AllowPrivateAccess = "true"))
	FName DrawSocket;
};
