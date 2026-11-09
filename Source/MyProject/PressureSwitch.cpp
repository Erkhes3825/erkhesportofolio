#include "PressureSwitch.h"
#include "GameFramework/Character.h"
#include "DoorGimmick.h"

APressureSwitch::APressureSwitch()
{
	PrimaryActorTick.bCanEverTick = false;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	
	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	BaseMesh->SetupAttachment(RootComponent);

	
	ButtonMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ButtonMesh"));
	ButtonMesh->SetupAttachment(BaseMesh);

	
	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	TriggerBox->SetupAttachment(BaseMesh);
	TriggerBox->SetCollisionProfileName(TEXT("Trigger"));
}

void APressureSwitch::BeginPlay()
{
	Super::BeginPlay();

	
	if (TriggerBox)
	{
		TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &APressureSwitch::OnOverlapBegin);
		TriggerBox->OnComponentEndOverlap.AddDynamic(this, &APressureSwitch::OnOverlapEnd);
	}
}

bool APressureSwitch::IsValidTriggerActor(AActor* Actor, UPrimitiveComponent* Component)
{
	if (!Actor || Actor == this) return false;

	
	if (Cast<ACharacter>(Actor))
	{
		return true;
	}

	
	if (Component && Component->IsSimulatingPhysics())
	{
		return true;
	}

	return false;
}

void APressureSwitch::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (IsValidTriggerActor(OtherActor, OtherComp))
	{
		OverlappingCount++;

		if (!bIsActivated && OverlappingCount > 0)
		{
			bIsActivated = true;
			UE_LOG(LogTemp, Warning, TEXT("Pressure Switch ACTIVATED!"));

			
			OnSwitchStateChanged.Broadcast(true);
		}
	}
}

void APressureSwitch::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (IsValidTriggerActor(OtherActor, OtherComp))
	{
		OverlappingCount = FMath::Max(0, OverlappingCount - 1);

		if (bIsActivated && OverlappingCount == 0)
		{
			bIsActivated = false;
			UE_LOG(LogTemp, Warning, TEXT("Pressure Switch DEACTIVATED!"));

			
			OnSwitchStateChanged.Broadcast(false);
		}
	}
}

