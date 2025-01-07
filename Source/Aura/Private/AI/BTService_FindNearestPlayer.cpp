// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTService_FindNearestPlayer.h"
#include "AIController.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BTFunctionLibrary.h"


void UBTService_FindNearestPlayer::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);


	APawn* OwningPawn = AIOwner->GetPawn();

	const FName TargetTag = OwningPawn->ActorHasTag(FName("Player")) ? FName("Enemy") : FName("Player");

	TArray<AActor*> ActorsWithTag;
	UGameplayStatics::GetAllActorsWithTag(OwningPawn, TargetTag, ActorsWithTag);

	AActor* NearestActor = nullptr;
	float NearestDistance = TNumericLimits<float>::Max();

	for (AActor* actor : ActorsWithTag)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Red, actor->GetActorNameOrLabel());

		if (IsValid(actor) && IsValid(OwningPawn))
		{
			const float Distance = OwningPawn->GetDistanceTo(actor);
			if (NearestDistance > Distance)
			{
				NearestDistance = Distance;
				NearestActor = actor;
			}

		}

	}

	UBTFunctionLibrary::SetBlackboardValueAsObject(this, TargetToFollowSelector, NearestActor);
	UBTFunctionLibrary::SetBlackboardValueAsFloat(this, DistanceToTargetSelector, NearestDistance);
	//ActorsWithTag
}
