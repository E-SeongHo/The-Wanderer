//  


#include "Utility/WandererUtils.h"

namespace WandererUtils
{
	bool SphereTrace(const UWorld* InWorld, FHitResult& OutHit, const AActor* SrcActor, const FVector& Start, const FVector& End, const ECollisionChannel TraceChannel)
	{
		FCollisionQueryParams Param;
		Param.AddIgnoredActor(SrcActor);

		constexpr float TraceRadius = 5.0f;
		const bool bHit = InWorld->SweepSingleByChannel(OutHit, Start, End, FQuat::Identity, TraceChannel, FCollisionShape::MakeSphere(TraceRadius), Param);
	
		const FColor TraceColor = bHit ? FColor::Green : FColor::Red;
	
		DrawDebugSphere(InWorld, Start, TraceRadius, 12, TraceColor, false, 1.0f);
		DrawDebugSphere(InWorld, End, TraceRadius, 12, TraceColor, false, 1.0f);
		DrawDebugLine(InWorld, Start, End, TraceColor, false, 1.0f);

		return bHit;
	}
	
}
