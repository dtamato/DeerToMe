// Fill out your copyright notice in the Description page of Project Settings.

#include "DeerToMe.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"
#include "Engine.h"
#include "HunterAIController.h"
#include "HunterAI.h"
#include "HunterDistanceToDeerCheckTask.h"

EBTNodeResult::Type UHunterDistanceToDeerCheckTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	UBlackboardComponent* BlackBoard = OwnerComp.GetBlackboardComponent();
	FBlackboard::FKey PlayerKey = BlackBoard->GetKeyID(Player);

	//Create AIController for hunter?
	AHunterAIController* NewHunter = Cast<AHunterAIController>(OwnerComp.GetAIOwner());
	if (NewHunter)
	{
		AHunterAI* HunterController = Cast<AHunterAI>(NewHunter->GetCharacter());
		OwnerComp.GetBlackboardComponent()->SetValueAsObject(PlayerKey, HunterController->PlayerCharacter);

		if (HunterController && HunterController->GetDistanceTo(HunterController->PlayerCharacter) <= 5000.0f)
		{	
			//play sound here
			HunterController->PlayerCharacter->SetStamina(0.0f);
			return EBTNodeResult::Succeeded;
		}
	}

	return EBTNodeResult::Succeeded;
}

/*bool UHunterDistanceToDeerCheckTask::InLineOfSight(AHunterAI* Target1, ADeerToMeCharacter* Target2)
{
	FCollisionQueryParams TraceParams;
	TraceParams.bTraceAsyncScene = true;
	TraceParams.bReturnPhysicalMaterial = true;
	TraceParams.AddIgnoredActor(Target1);

	FHitResult Hit(ForceInit);
	GetWorld()->LineTraceSingleByChannel(Hit, Target1->GetActorLocation(), Target1->PlayerCharacter->GetActorLocation(), ECC_GameTraceChannel4, TraceParams);

	if (Hit.GetActor())
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("In range for the kill!"));
		return true;
	}
	return false;
}*/
