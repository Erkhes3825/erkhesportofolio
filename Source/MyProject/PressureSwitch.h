#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "DoorGimmick.h"
#include "PressureSwitch.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSwitchStateChanged, bool, bIsPressed);

UCLASS()
class MYPROJECT_API APressureSwitch : public AActor
{
	GENERATED_BODY()

public:
	APressureSwitch();

protected:
	virtual void BeginPlay() override;

public:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* BaseMesh;

	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* ButtonMesh;

	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UBoxComponent* TriggerBox;

	
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnSwitchStateChanged OnSwitchStateChanged;

	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Switch State")
	bool bIsActivated = false;

	UPROPERTY(EditInstanceOnly,Category="Switch Settings")
	ADoorGimmick* TargetDoor;
private:
	
	int32 OverlappingCount = 0;

	
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	
	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	
	bool IsValidTriggerActor(AActor* Actor, UPrimitiveComponent* Component);
};

