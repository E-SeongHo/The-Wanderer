//  


#include "AI/WandererAIController.h"

#include "AbilitySystemComponent.h"
#include "WandererGameplayTags.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Bool.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Enum.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "Character/WandererCharacter.h"
#include "Character/Component/WandererCombatComponent.h"
#include "Character/Enemy/WandererEnemy.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "Perception/AISenseConfig_Sight.h"

AWandererAIController::AWandererAIController()
{
	Blackboard = CreateDefaultSubobject<UBlackboardComponent>("Blackboard Component");
	BrainComponent = CreateDefaultSubobject<UBehaviorTreeComponent>("BehaviorTree Component");

	PerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>("Perception Component");
	
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

	UAISenseConfig_Hearing* HearingConfig = CreateDefaultSubobject<UAISenseConfig_Hearing>("Hearing Config");
	HearingConfig->HearingRange = 500.0f;
	HearingConfig->SetMaxAge(1.0f);
    
	HearingConfig->DetectionByAffiliation.bDetectEnemies = true;
	HearingConfig->DetectionByAffiliation.bDetectFriendlies = true;
	HearingConfig->DetectionByAffiliation.bDetectNeutrals = true;
    
	PerceptionComponent->ConfigureSense(*SightConfig);
	PerceptionComponent->ConfigureSense(*HearingConfig);
    
	PerceptionComponent->SetDominantSense(SightConfig->GetSenseImplementation());
	PerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &AWandererAIController::OnTargetDetected);
}

AWandererEnemy* AWandererAIController::GetControllingEnemy() const
{
	return ControllingEnemy.Get();
}

void AWandererAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	ControllingEnemy = CastChecked<AWandererEnemy>(InPawn);
	
	InitializeBlackboard(*Blackboard, *BehaviorTree->BlackboardAsset);
	RunBehaviorTree(BehaviorTree);
}

bool AWandererAIController::InitializeBlackboard(UBlackboardComponent& BlackboardComp, UBlackboardData& BlackboardAsset)
{
	BBSightTargetKey = BlackboardAsset.GetKeyID(TEXT("SightTarget"));
	BBSoundLocationKey = BlackboardAsset.GetKeyID(TEXT("SoundLocation"));
	BBCombatTargetLocationKey = BlackboardAsset.GetKeyID(TEXT("CombatTargetLocation"));
	BBBehaviorTypeKey = BlackboardAsset.GetKeyID(TEXT("BehaviorType"));
	BBActionTypeKey = BlackboardAsset.GetKeyID(TEXT("ActionType"));
	
	return Super::InitializeBlackboard(BlackboardComp, BlackboardAsset);
}

void AWandererAIController::OnTargetDetected(AActor* Actor, FAIStimulus Stimulus)
{
	// only tracks hero
	const AWandererCharacter* Target = Cast<AWandererCharacter>(Actor); 
	if(!Target) return;
	
	if(Stimulus.Type.Index == UAISense::GetSenseID<UAISense_Sight>().Index)
	{
		HandleSight(Actor, Stimulus);
	}
	else if(Stimulus.Type.Index == UAISense::GetSenseID<UAISense_Hearing>().Index)
	{
		HandleHearing(Actor, Stimulus);			
	}
}

void AWandererAIController::HandleSight(AActor* Actor, FAIStimulus Stimulus)
{
	if(Stimulus.WasSuccessfullySensed())
	{
		GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Green, FString::Printf(TEXT("Detected")));
		//ControllingEnemy->GetCombatComponent()->SetCombatTarget(Target);
		Blackboard->SetValue<UBlackboardKeyType_Object>(BBSightTargetKey, Actor);
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Green, FString::Printf(TEXT("UnDetected")));
		//ControllingEnemy->GetCombatComponent()->SetCombatTarget(nullptr);
		Blackboard->ClearValue(BBSightTargetKey);
	}
}

void AWandererAIController::HandleHearing(AActor* Actor, FAIStimulus Stimulus)
{
	if(Blackboard->GetValue<UBlackboardKeyType_Object>(BBSightTargetKey)) return;
	
	if(Stimulus.WasSuccessfullySensed())
	{
		Blackboard->SetValue<UBlackboardKeyType_Vector>(BBSoundLocationKey, Stimulus.StimulusLocation);
	}
	else
	{
		Blackboard->ClearValue(BBSoundLocationKey);
	}
}
