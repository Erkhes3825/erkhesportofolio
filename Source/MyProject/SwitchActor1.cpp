#include "SwitchActor1.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Materials/MaterialInterface.h"
#include "SwitchableInterface.h"

ASwitchActor1::ASwitchActor1()
{
	PrimaryActorTick.bCanEverTick = false;

	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComp"));
	SetRootComponent(RootComp);

	SwitchMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SwitchMesh"));
	SwitchMesh->SetupAttachment(RootComp);

	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	TriggerBox->SetupAttachment(SwitchMesh);
	TriggerBox->SetCollisionProfileName(TEXT("Trigger"));
}

void ASwitchActor1::BeginPlay()
{
	Super::BeginPlay();

	
	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &ASwitchActor1::OnOverlapBegin);

	
	UpdateSwitchVisuals();
}

void ASwitchActor1::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
	if (!OtherActor || OtherActor == this) return;

	bIsOn = !bIsOn;

	
	UpdateSwitchVisuals();

	
	for (AActor* Gimmick : TargetGimmicks)
	{
		if (Gimmick && Gimmick->GetClass()->ImplementsInterface(USwitchableInterface::StaticClass()))
		{
			ISwitchableInterface::Execute_OnActivate(Gimmick);
		}
	}
}

void ASwitchActor1::UpdateSwitchVisuals()
{

	UMaterialInterface* SelectedMaterial = bIsOn ? OnMaterial : OffMaterial;
	if (SelectedMaterial)
	{
		SwitchMesh->SetMaterial(0, SelectedMaterial);
	}

	FVector NewLocation = FVector::ZeroVector;
	if (bIsOn)
	{
		NewLocation.Z = PressedZOffset;
	}
	SwitchMesh->SetRelativeLocation(NewLocation);
}
