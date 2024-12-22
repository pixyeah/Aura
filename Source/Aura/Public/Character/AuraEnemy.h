#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "Character/AuraCharacterBase.h"
#include "Interact/EnemyInterface.h"
#include "UI/WidgetController/OverlayWidgetController.h"
#include "AuraEnemy.generated.h"

class UWidgetComponent;
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
	virtual void HighlightActor() override;

	virtual void UnHightlightActor() override;

	virtual void Death() override;

	UPROPERTY(BlueprintReadOnly)
	bool bHighLighted = false;

	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnHealthChanged;
	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnMaxHealthChanged;

	void HitReactTagChanged(FGameplayTag GameplayTag, int I);

	UPROPERTY(BlueprintReadOnly, Category="Combat")
	bool bHitReacting = false;

	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	float BaseWalkSpeed = 250.f;

	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "Combat")
	float DeathLifeSpan = 5.f;
protected:
	virtual void BeginPlay() override;
	virtual void InitAbilityActorInfo() override;
	virtual void InitDefaultAttribute() override;

	/* from combat interface */
	virtual int32 GetPlayerLevel() override;



	UPROPERTY(EditAnyWhere, BlueprintReadOnly,Category = "Combat");
	int32 Level = 1;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "Combat");
	ECharacterClass CharacterClass = ECharacterClass::Warrior;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UWidgetComponent> HealthBar;

private:
	void BindAttributeChange();
};
