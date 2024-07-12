//  

#pragma once

#include "CoreMinimal.h"

namespace WandererUtils
{
	bool SphereTrace(const UWorld* InWorld, FHitResult& OutHit, const AActor* SrcActor, const FVector& Start, const FVector& End, const ECollisionChannel TraceChannel = ECC_Visibility);
}
