#include "DoorGimmick.h"
#include "SwitchActor1.h"
#include "Components/StaticMeshComponent.h"
#include "Camera/CameraComponent.h"
#include "Curves/CurveFloat.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Character.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetBlueprintLibrary.h" 

ADoorGimmick::ADoorGimmick()
{
	PrimaryActorTick.bCanEverTick = false;

	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComp"));
	SetRootComponent(RootComp);

	FrameMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FrameMesh"));
	FrameMesh->SetupAttachment(RootComp);

	DoorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorMesh"));
	DoorMesh->SetupAttachment(RootComp);

	CutsceneCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("CutsceneCamera"));
	CutsceneCamera->SetupAttachment(RootComp);

	DoorTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("DoorTimeline"));
}

void ADoorGimmick::BeginPlay()
{
	Super::BeginPlay();

	if (DoorCurve)
	{
		FOnTimelineFloat ProgressCallback;
		ProgressCallback.BindUFunction(this, FName("UpdateDoorRotation"));

		FOnTimelineEvent FinishedCallback;
		FinishedCallback.BindUFunction(this, FName("OnTimelineFinished"));

		DoorTimeline->AddInterpFloat(DoorCurve, ProgressCallback);
		DoorTimeline->SetTimelineFinishedFunc(FinishedCallback);
	}
}

void ADoorGimmick::OnActivate_Implementation()
{
	if (RequiredSwitches.Num() > 0)
	{
		bool bAllSwitchesOn = true;
		for (AActor* SwitchActor : RequiredSwitches)
		{
			ASwitchActor1* Switch = Cast<ASwitchActor1>(SwitchActor);
			if (Switch && !Switch->IsSwitchedOn())
			{
				bAllSwitchesOn = false;
				break;
			}
		}

		if (!bAllSwitchesOn)
		{
			return;
		}
	}

	APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
	if (!PC) return;

	
	SetHUDVisibility(false);

	PC->DisableInput(PC);
	PC->SetViewTargetWithBlend(this, CameraBlendTime, EViewTargetBlendFunction::VTBlend_Cubic);

	GetWorldTimerManager().SetTimer(
		CameraDelayTimerHandle,
		this,
		&ADoorGimmick::StartDoorAnimation,
		CameraBlendTime,
		false
	);
}

void ADoorGimmick::StartDoorAnimation()
{
	if (!bIsOpen)
	{
		DoorTimeline->Play();
		bIsOpen = true;
	}
	else
	{
		DoorTimeline->Reverse();
		bIsOpen = false;
	}
}

void ADoorGimmick::UpdateDoorRotation(float Output)
{
	FRotator NewRotation = FRotator(0.0f, Output * MaxYawAngle, 0.0f);
	DoorMesh->SetRelativeRotation(NewRotation);
}

void ADoorGimmick::OnTimelineFinished()
{
	GetWorldTimerManager().SetTimer(
		ReturnCameraTimerHandle,
		this,
		&ADoorGimmick::ReturnCameraToPlayer,
		CameraHoldTime,
		false
	);
}

void ADoorGimmick::ReturnCameraToPlayer()
{
	APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
	ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(this, 0);
	if (!PC || !PlayerCharacter) return;

	
	SetHUDVisibility(true);

	PC->SetViewTargetWithBlend(PlayerCharacter, CameraBlendTime, EViewTargetBlendFunction::VTBlend_Cubic);

	TWeakObjectPtr<APlayerController> WeakPC = PC;
	GetWorldTimerManager().SetTimer(
		EnableInputTimerHandle,
		FTimerDelegate::CreateLambda([WeakPC]()
			{
				if (WeakPC.IsValid())
				{
					WeakPC->EnableInput(WeakPC.Get());
				}
			}),
		CameraBlendTime,
		false
	);
}

void ADoorGimmick::SetHUDVisibility(bool bVisible)
{
	TArray<UUserWidget*> FoundWidgets;
	
	UWidgetBlueprintLibrary::GetAllWidgetsOfClass(GetWorld(), FoundWidgets, UUserWidget::StaticClass(), false);

	ESlateVisibility NewVisibility = bVisible ? ESlateVisibility::Visible : ESlateVisibility::Hidden;

	for (UUserWidget* Widget : FoundWidgets)
	{
		if (Widget)
		{
			Widget->SetVisibility(NewVisibility);
		}
	}
}