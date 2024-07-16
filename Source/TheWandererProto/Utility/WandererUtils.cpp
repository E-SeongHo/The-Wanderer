//  


#include "Utility/WandererUtils.h"

#include "Components/CapsuleComponent.h"
#include "Engine/OverlapResult.h"
#include "GameFramework/Character.h"

namespace WandererUtils
{
	bool SphereTrace(FHitResult& OutHit, const AActor* SrcActor, const FVector& Start, const FVector& End, const float TraceRadius, const ECollisionChannel TraceChannel)
	{
		FCollisionQueryParams Param;
		Param.AddIgnoredActor(SrcActor);

		const bool bHit = SrcActor->GetWorld()->SweepSingleByChannel(OutHit, Start, End, FQuat::Identity, TraceChannel, FCollisionShape::MakeSphere(TraceRadius), Param);
	
		const FColor TraceColor = bHit ? FColor::Green : FColor::Red;
	
		DrawDebugSphere(SrcActor->GetWorld(), Start, TraceRadius, 12, TraceColor, false, 1.0f);
		DrawDebugSphere(SrcActor->GetWorld(), End, TraceRadius, 12, TraceColor, false, 1.0f);
		DrawDebugLine(SrcActor->GetWorld(), Start, End, TraceColor, false, 1.0f);

		return bHit;
	}
	
	TArray<AActor*> FindOverlappingActorsInViewRange(const TSubclassOf<AActor> Class, const ACharacter* SrcCharacter, const float ViewAngle, const float ViewDistance, const ECollisionChannel TraceChannel)
	{
		const FVector Location = SrcCharacter->GetActorLocation();
		const FVector Forward = SrcCharacter->GetControlRotation().Vector().GetSafeNormal2D();
		const float HalfHeight = SrcCharacter->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
		
		FCollisionQueryParams Params;
		Params.AddIgnoredActor(SrcCharacter);
		
		TArray<FOverlapResult> OutResults;
		SrcCharacter->GetWorld()->OverlapMultiByChannel(OutResults, Location, FQuat::Identity, TraceChannel, FCollisionShape::MakeSphere(FMath::Max(ViewDistance, HalfHeight * 2.0f)), Params);

		const float LimitCosine = FMath::Cos(FMath::DegreesToRadians(ViewAngle * 0.5f));
		TArray<AActor*> ActorsInViewRange;
		for(FOverlapResult& Result : OutResults)
		{
			if(Result.GetActor()->IsA(Class))
			{
				const FVector TargetLocation = Result.GetActor()->GetActorLocation();
				const FVector2D SourceXY(Location.X, Location.Y);
				const FVector2D TargetXY(TargetLocation.X, TargetLocation.Y);

				const float Cosine = FVector2D::DotProduct((TargetXY - SourceXY).GetSafeNormal(), FVector2D(Forward.X, Forward.Y));

				// Target Z should within Src Character's capsule height and within the given view angle
				if(Cosine > LimitCosine && TargetLocation.Z < Location.Z + HalfHeight && TargetLocation.Z > Location.Z - HalfHeight)
				{
					ActorsInViewRange.Add(Result.GetActor());
				}
			}
		}

		FColor Color = ActorsInViewRange.IsEmpty() ? FColor::Red : FColor::Green;
		DrawDebugViewRange(SrcCharacter->GetWorld(), Location, Forward, HalfHeight*2.0f, ViewAngle, ViewDistance, Color);

		return ActorsInViewRange;
	}

	void DrawDebugViewRange(const UWorld* World, const FVector& Origin, const FVector& Direction, const float Height, const float Angle, const float Distance, const FColor Color)
	{
		const float HalfHeight = Height * 0.5f;

		const FVector LeftDir = Direction.RotateAngleAxis(-Angle / 2, FVector::UpVector);
		const FVector RightDir = Direction.RotateAngleAxis(Angle / 2, FVector::UpVector);

		const FVector MiddleLeft = Origin + LeftDir * Distance;
		const FVector MiddleRight = Origin + RightDir * Distance;
		DrawDebugLine(World, Origin, MiddleLeft, Color, false, 1.0f);
		DrawDebugLine(World, Origin, MiddleRight, Color, false, 1.0f);
		DrawDebugLine(World, Origin, Origin + Direction * Distance, FColor::Blue, false, 1.0f);

		const FVector BottomLeft(MiddleLeft.X, MiddleLeft.Y, MiddleLeft.Z - HalfHeight);
		const FVector BottomRight(MiddleRight.X, MiddleRight.Y, MiddleRight.Z - HalfHeight);
		DrawDebugLine(World, BottomLeft, BottomLeft + FVector::UpVector * Height, Color, false, 1.0f);
		DrawDebugLine(World, BottomRight, BottomRight + FVector::UpVector * Height, Color, false, 1.0f);

		constexpr int32 NumSegments = 32;
		for(int32 i = 0; i < NumSegments; i++)
		{
			const FVector CurrentStairOrigin(Origin.X, Origin.Y, Height * i / NumSegments);
			DrawDebugLine(World, CurrentStairOrigin, CurrentStairOrigin + LeftDir * Distance, Color, false, 1.0f);
			DrawDebugLine(World, CurrentStairOrigin, CurrentStairOrigin + RightDir * Distance, Color, false, 1.0f);
			DrawDebugLine(World, CurrentStairOrigin, CurrentStairOrigin + Direction * Distance, FColor::Blue, false, 1.0f);
			
			for(int32 j = 0; j < NumSegments; j++)
			{
				const float Angle1 = -Angle / 2 + (Angle * j / NumSegments);
				const float Angle2 = -Angle / 2 + (Angle * (j + 1) / NumSegments);
        
				FVector Start = Direction.RotateAngleAxis(Angle1, FVector::UpVector) * Distance;
				FVector End = Direction.RotateAngleAxis(Angle2, FVector::UpVector) * Distance;
				
				DrawDebugLine(World, CurrentStairOrigin + Start, CurrentStairOrigin + End, Color, false, 1.0f);
			}
		}
	}
}
