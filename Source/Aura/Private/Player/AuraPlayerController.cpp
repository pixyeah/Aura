// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/AuraPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "GameplayTagContainer.h"
#include "Input/AuraInputComponent.h"
#include "Interact/EnemyInterface.h"
#include "Player/AuraPlayerState.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "Components/SplineComponent.h"
#include "AuraGameplayTags.h"
#include "AbilitySystemBlueprintLibrary.h"
#include <NavigationPath.h>
#include <NavigationSystem.h>

#include "GameFramework/Character.h"
#include "UI/Widget/DamageTextComponent.h"

AAuraPlayerController::AAuraPlayerController()
{
	bReplicates = true;

	Spline = CreateDefaultSubobject<USplineComponent>("SplineComp");


}

UAuraAbilitySystemComponent* AAuraPlayerController::GetASC()
{
	if (AuraAbilitySystemComponent == nullptr)
	{
		AuraAbilitySystemComponent = CastChecked<UAuraAbilitySystemComponent>
			(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn<APawn>()));
		//AuraAbilitySystemComponent = CastChecked<UAuraAbilitySystemComponent>(GetPlayerState<AAuraPlayerState>()->AbilitySystemComponent);
	}
	return AuraAbilitySystemComponent;
}

void AAuraPlayerController::ShowDamageNumber_Implementation(float Damage,ACharacter* TargetCharacter,bool bBlockedHit,bool bCriticalHit)
{
	if (IsValid(TargetCharacter) && DamageTextComponentClass && IsLocalController())
	{
		UDamageTextComponent* DamageText = NewObject<UDamageTextComponent>(TargetCharacter,DamageTextComponentClass);
		DamageText->RegisterComponent();
		DamageText->AttachToComponent(TargetCharacter->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
		DamageText->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
		DamageText->SetDamage(Damage,bBlockedHit,bCriticalHit);
	}
}

void AAuraPlayerController::BeginPlay()
{
	Super::BeginPlay();

	UEnhancedInputLocalPlayerSubsystem* Subsystem =
		ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	check(AuraContext);
	if (Subsystem)
	{
		Subsystem->AddMappingContext(AuraContext, 0);
	}

	//Set Mouse Cursor
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;
	FInputModeGameAndUI InputModeData;
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputModeData.SetHideCursorDuringCapture(false);
	SetInputMode(InputModeData);
}

void AAuraPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UAuraInputComponent* AuraInputComponent = CastChecked<UAuraInputComponent>(InputComponent);
	AuraInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AAuraPlayerController::Move);
	AuraInputComponent->BindAction(ShiftAction, ETriggerEvent::Started, this, &AAuraPlayerController::ShiftPressed);
	AuraInputComponent->BindAction(ShiftAction, ETriggerEvent::Completed, this, &AAuraPlayerController::ShiftReleased);
	AuraInputComponent->BindAbilityActions(InputConfig, this, &ThisClass::AbilityInputTagPressed,
		&ThisClass::AbilityInputTagReleased, &ThisClass::AbilityInputTagHeld);
}

void AAuraPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	CursorTrace();
	AutoRun();
}


void AAuraPlayerController::AutoRun()
{
	if (!bAutoRunning) return;
	APawn* PlayerPawn = GetPawn();
	if (PlayerPawn)
	{
		FVector NextSplineLocation = Spline->FindLocationClosestToWorldLocation(PlayerPawn->GetActorLocation(),
			ESplineCoordinateSpace::World);
		FVector NextSplineDirction = Spline->FindDirectionClosestToWorldLocation(PlayerPawn->GetActorLocation(),
			ESplineCoordinateSpace::World);
		PlayerPawn->AddMovementInput(NextSplineDirction);

		const float DistanceToDestination = (NextSplineLocation - CachedDestination).Length();
		if (DistanceToDestination <= AutoRunAcceptanceRadius)
		{
			bAutoRunning = false;
		}
	}

}


void AAuraPlayerController::Move(const FInputActionValue& InputActionValue)
{
	const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0.0, Rotation.Yaw, 0.0);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	if (APawn* ControllerPawn = GetPawn<APawn>())
	{
		ControllerPawn->AddMovementInput(ForwardDirection, InputAxisVector.Y);
		ControllerPawn->AddMovementInput(RightDirection, InputAxisVector.X);
	}
}

void AAuraPlayerController::CursorTrace()
{
	GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);
	if (!CursorHit.bBlockingHit) return;

	ThisActor = Cast<IEnemyInterface>(CursorHit.GetActor());

	/**
	A. LastActor is null && ThisActor is null.
		-Do Nothing
	B. LastActor is null && ThisActor is valid.
		-Hightlight ThisActor
	C. Both actors is valid and same.
		-DoNothing
	D. Both actors is valid but not same.
		-UnHighlight LastActor and Hightlight This Actor.
	E. LastActor is valid && ThisActor is null.
		-UnHighlight LastActor
	**/

	if (LastActor == nullptr)
	{
		if (ThisActor != nullptr)
		{
			ThisActor->HighlightActor();
		}
	}
	else
	{
		if (ThisActor == nullptr)
		{
			LastActor->UnHightlightActor();
		}
		else if (LastActor != ThisActor)
		{
			LastActor->UnHightlightActor();
			ThisActor->HighlightActor();
		}
	}
	LastActor = ThisActor;
}

void AAuraPlayerController::AbilityInputTagPressed(FGameplayTag InputTag)
{
	//UE_LOG(LogTemp, Display, TEXT("[%s] pressed...."),*InputTag.ToString());

	if (InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_LMB))
	{
		bTargeting = ThisActor ? true : false;
		bAutoRunning = false;
	}
}

void AAuraPlayerController::AbilityInputTagReleased(FGameplayTag InputTag)
{
	UE_LOG(LogTemp, Display, TEXT("[%s] Released...."), *InputTag.ToString());


	UAuraAbilitySystemComponent* ASC = GetASC();
	if (ASC)
	{
		ASC->AbilityInputTagReleased(InputTag);
	}

	if (!InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_LMB))
	{
		//UAuraAbilitySystemComponent* ASC = GetASC();
		//if (ASC)
		//{
		//	ASC->AbilityInputTagReleased(InputTag);
		//}
		return;
	}

	if (!bTargeting && !bShiftDown)
	{

		APawn* ControllerPawn = GetPawn();
		if (FollowTime <= ShortPressThreshold && ControllerPawn)
		{
			if (UNavigationPath* NavPath = UNavigationSystemV1::FindPathToLocationSynchronously
			(this, ControllerPawn->GetActorLocation(), CachedDestination))
			{
				Spline->ClearSplinePoints();
				for (const FVector& PointLoc : NavPath->PathPoints)
				{
					Spline->AddSplinePoint(PointLoc, ESplineCoordinateSpace::World);
					//DrawDebugSphere(GetWorld(), PointLoc, 10, 8, FColor::Green, false, 5.f);
				}

				CachedDestination = NavPath->PathPoints[NavPath->PathPoints.Num() - 1];

				bAutoRunning = true;
			}
		}
		bTargeting = false;
		FollowTime = 0.f;
	}
}

void AAuraPlayerController::AbilityInputTagHeld(FGameplayTag InputTag)
{
	//UE_LOG(LogTemp, Display, TEXT("[%s] Held...."), *InputTag.ToString());

	if (!InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_LMB))
	{
		UAuraAbilitySystemComponent* ASC = GetASC();
		if (ASC)
		{
			ASC->AbilityInputTagHeld(InputTag);
		}
		return;
	}

	if (bTargeting || bShiftDown)
	{
		UAuraAbilitySystemComponent* ASC = GetASC();
		if (ASC)
		{
			ASC->AbilityInputTagHeld(InputTag);
		}

	}
	else
	{
		FollowTime += GetWorld()->DeltaTimeSeconds;

		if (!CursorHit.bBlockingHit) 
		{
			return;
		}
		CachedDestination = CursorHit.ImpactPoint;
		DrawDebugSphere(GetWorld(), CachedDestination, 10, 8, FColor::Red, false, 5.f);

		if (APawn* ControllerPawn = GetPawn())
		{
			const FVector WorldDirection = (CachedDestination - ControllerPawn->GetActorLocation()).GetSafeNormal();
			ControllerPawn->AddMovementInput(WorldDirection);
		}
	}
}
