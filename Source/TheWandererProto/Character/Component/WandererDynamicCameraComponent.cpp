//  


#include "WandererDynamicCameraComponent.h"

#include "Character/WandererBaseCharacter.h"
#include "WandererCombatComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"


UWandererDynamicCameraComponent::UWandererDynamicCameraComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UWandererDynamicCameraComponent::SetupReferences(USpringArmComponent* InSpringArmComponent, UCameraComponent* InCameraComponent)
{
	SpringArmComponent = InSpringArmComponent;
	CameraComponent = InCameraComponent;
}

void UWandererDynamicCameraComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	const float Distance = Subject.Get() ? GetOwner()->GetDistanceTo(Subject.Get()) : 400.0f;
	const float Theta = FMath::DegreesToRadians(CameraComponent->FieldOfView) * 0.5f;

	const float GoalDistance = FMath::Min(FMath::Max(MinSpringArmLength, Distance / FMath::Tan(Theta)), MaxSpringArmLength);
	
	SpringArmComponent->TargetArmLength = FMath::FInterpTo(SpringArmComponent->TargetArmLength, GoalDistance, DeltaTime, 5.0f);
}

void UWandererDynamicCameraComponent::BeginPlay()
{
	Super::BeginPlay();

	UWandererCombatComponent* CombatComponent = GetOwner()->FindComponentByClass<UWandererCombatComponent>();
	CombatComponent->OnTargetChanged.AddLambda([this](AWandererBaseCharacter* NewTarget)
	{
		Subject = NewTarget;
	});
}

