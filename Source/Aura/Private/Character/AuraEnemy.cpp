// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/AuraEnemy.h"

#include "AuraGameplayTags.h"
#include "Aura/Aura.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "UI/Widget/AuraUserWidget.h"
#include "AI/AuraAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"

AAuraEnemy::AAuraEnemy()
{
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	GetMesh()->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
	Weapon->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bUseControllerDesiredRotation = true;


	AbilitySystemComponent = CreateDefaultSubobject<UAuraAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);
	
	AttributeSet = CreateDefaultSubobject<UAuraAttributeSet>("AttributeSet");
	HealthBar = CreateDefaultSubobject<UWidgetComponent>("HealthBar");
	HealthBar->SetupAttachment(RootComponent);
}



void AAuraEnemy::BeginPlay()
{
	Super::BeginPlay();
	InitAbilityActorInfo();
	GetCharacterMovement()->MaxWalkSpeed = bHitReacting ? 0.f : BaseWalkSpeed;
	if (HasAuthority())
	{
		UAuraAbilitySystemLibrary::GiveStartupAbilities(this, AbilitySystemComponent,CharacterClass);
	}

	if (UAuraUserWidget* HealthWidget = Cast<UAuraUserWidget>(HealthBar->GetUserWidgetObject()))
	{
		HealthWidget->SetWidgetController(this);
	}
	
	UAuraAttributeSet* AuraAS = CastChecked<UAuraAttributeSet>(AttributeSet);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAS->GetHealthAttribute())
		.AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
				OnHealthChanged.Broadcast(Data.NewValue);
		}
		);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAS->GetMaxHealthAttribute())
		.AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnMaxHealthChanged.Broadcast(Data.NewValue);
			}
		);

	AbilitySystemComponent->RegisterGameplayTagEvent(FAuraGameplayTags::Get().Effects_HitReact, EGameplayTagEventType::NewOrRemoved)
		.AddUObject(
			this,
			&AAuraEnemy::HitReactTagChanged
		);
	
	FTimerHandle Timer; 
	GetWorld()->GetTimerManager().SetTimer(
		Timer,            // 定时器句柄
		this,                   // 调用函数的对象
		&AAuraEnemy::BindAttributeChange, // 被调用的函数
		0.5f,                   // 延迟时间（秒）
		false                   // 是否循环
	);
	//GetWorld()->GetTimerManager().SetTimerForNextTick([this,AuraAS]()
	//	{
	//		OnMaxHealthChanged.Broadcast(AuraAS->GetMaxHealth());
	//		OnHealthChanged.Broadcast(AuraAS->GetHealth());
	//	});
}

void AAuraEnemy::BindAttributeChange()
{
	UAuraAttributeSet* AuraAS = CastChecked<UAuraAttributeSet>(AttributeSet);
	OnMaxHealthChanged.Broadcast(AuraAS->GetMaxHealth());
	OnHealthChanged.Broadcast(AuraAS->GetHealth());
}

void AAuraEnemy::HitReactTagChanged(FGameplayTag CallbackTag, int NewCount)
{
	bHitReacting = NewCount > 0;

	GetCharacterMovement()->MaxWalkSpeed = bHitReacting ? 0.f : BaseWalkSpeed;
	if (AuraAIController && AuraAIController->GetBlackboardComponent())
	{
		AuraAIController->GetBlackboardComponent()->SetValueAsBool(FName("HitReacting"), bHitReacting);
	}
}

void AAuraEnemy::InitAbilityActorInfo()
{
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent)->AbilityActorInfoSet();

	if (HasAuthority())
	{
		InitDefaultAttribute();
	}
}

void AAuraEnemy::InitDefaultAttribute()
{
	UAuraAbilitySystemLibrary::InitializeDefaultAttributes(this, CharacterClass, Level, AbilitySystemComponent);
}

void AAuraEnemy::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	AuraAIController = Cast<AAuraAIController>(NewController);

	AuraAIController->GetBlackboardComponent()->InitializeBlackboard(*BehaviorTree->GetBlackboardAsset());
	AuraAIController->RunBehaviorTree(BehaviorTree);

	AuraAIController->GetBlackboardComponent()->SetValueAsBool(FName("HitReacting"), false);
	AuraAIController->GetBlackboardComponent()->SetValueAsBool(FName("RangedAttacker"), CharacterClass == ECharacterClass::Ranger);
}

int32 AAuraEnemy::GetPlayerLevel()
{
	return Level;
}

void AAuraEnemy::HighlightActor()
{
	bHighLighted = true;

	GetMesh()->SetRenderCustomDepth(true);

	Weapon->SetRenderCustomDepth(true);
}

void AAuraEnemy::UnHightlightActor()
{
	bHighLighted = false;
	GetMesh()->SetRenderCustomDepth(false);
	//GetMesh()->SetCustomDepthStencilValue(250);

	Weapon->SetRenderCustomDepth(false);
	//Weapon->SetCustomDepthStencilValue(250);
}

void AAuraEnemy::Death()
{
	SetLifeSpan(DeathLifeSpan);
	Super::Death();
}

void AAuraEnemy::SetCombatTarget_Implementation(AActor* InCombatTarget)
{
	CombatTarget = InCombatTarget;
}

AActor* AAuraEnemy::GetCombatTarget_Implementation() const
{
	return CombatTarget;
}

