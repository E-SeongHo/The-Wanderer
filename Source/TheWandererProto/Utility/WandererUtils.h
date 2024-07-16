//  

#pragma once

#include "CoreMinimal.h"

namespace WandererUtils
{
	bool SphereTrace(FHitResult& OutHit, const AActor* SrcActor, const FVector& Start, const FVector& End, const float TraceRadius, const ECollisionChannel TraceChannel = ECC_Visibility);

	TArray<AActor*> FindOverlappingActorsInViewRange(const TSubclassOf<AActor> Class, const ACharacter* SrcCharacter, const float ViewAngle, const float ViewDistance, const ECollisionChannel TraceChannel = ECC_Visibility);

	void DrawDebugViewRange(const UWorld* World, const FVector& Origin, const FVector& Direction, const float Height, const float Angle, const float Distance, const FColor Color);
}
