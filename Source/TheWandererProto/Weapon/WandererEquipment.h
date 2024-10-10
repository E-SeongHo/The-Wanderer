//  

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "WandererGameplayTags.h"
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

	void OnDropDown();
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Owner, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<AWandererBaseCharacter> EquipmentOwner;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Equipment, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USceneComponent> Root;

	FGameplayTag EquippedTag;
	FGameplayTag DrawnTag;
	
	TWeakObjectPtr<UStaticMeshComponent> EquipmentMesh;
	
	bool bIsEquipped;
	bool bIsDrawn;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Socket, meta = (AllowPrivateAccess = "true"))
	FName EquipSocket;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Socket, meta = (AllowPrivateAccess = "true"))
	FName DrawSocket;
};
