#pragma once

#include "CoreMinimal.h"
#include "Character/AuraCharacterBase.h"
#include "Interact/EnemyInterface.h"
#include "AuraEnemy.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API AAuraEnemy : public AAuraCharacterBase,public IEnemyInterface
{
	GENERATED_BODY()
	
public:
	AAuraEnemy();

	// 通过 IEnemyInterface 继承
	void HighlightActor() override;

	void UnHightlightActor() override;

	UPROPERTY(BlueprintReadOnly)
	bool bHighLighted = false;
protected:

	virtual void BeginPlay() override;
	virtual void InitAbilityActorInfo() override;
};
