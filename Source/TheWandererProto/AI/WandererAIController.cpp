//  


#include "AI/WandererAIController.h"

#include "AbilitySystemComponent.h"
#include "WandererGameplayTags.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/WandererCharacter.h"
#include "Character/WandererCombatComponent.h"
#include "Character/WandererEnemy.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"

AWandererAIController::AWandererAIController()
{
	Blackboard = CreateDefaultSubobject<UBlackboardComponent>("Blackboard Component");
	BrainComponent = CreateDefaultSubobject<UBehaviorTreeComponent>("BehaviorTree Component");

	PerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>("Perception Component");
	{
		UAISenseConfig_Sight* SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>("Sight Config");
		// TODO: check out Enemy's combat component?
		SightConfig->SightRadius = 500.0f;
		SightConfig->LoseSightRadius = 550.0f;
		SightConfig->PeripheralVisionAngleDegrees = 120.0f;
		SightConfig->SetMaxAge(1.0f);
		SightConfig->AutoSuccessRangeFromLastSeenLocation = 900.0f;

		// all actors are neutrals by default
		SightConfig->DetectionByAffiliation.bDetectEnemies = true;
		SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
		SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
		
		PerceptionComponent->ConfigureSense(*SightConfig);
		PerceptionComponent->SetDominantSense(SightConfig->GetSenseImplementation());
		PerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &AWandererAIController::OnTargetDetected);
	}
}

void AWandererAIController::BeginPlay()
{
	Super::BeginPlay();

	AbilitySystemComponent = Cast<AWandererEnemy>(GetPawn())->GetAbilitySystemComponent();
}

void AWandererAIController::OnTargetDetected(AActor* Actor, FAIStimulus Stimulus)
{
	AWandererCharacter* Target = Cast<AWandererCharacter>(Actor); 
	if(!Target) return;

	AWandererEnemy* ControllingEnemy = Cast<AWandererEnemy>(GetPawn());
	check(ControllingEnemy);
	
	if(Stimulus.WasSuccessfullySensed())
	{
		GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Green, FString::Printf(TEXT("Detected")));
		ControllingEnemy->GetCombatComponent()->SetCombatTarget(Target); 
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Green, FString::Printf(TEXT("UnDetected")));
		ControllingEnemy->GetCombatComponent()->SetCombatTarget(nullptr); 
	}
}
