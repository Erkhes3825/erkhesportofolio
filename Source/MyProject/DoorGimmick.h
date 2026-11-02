#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/TimelineComponent.h"
#include "SwitchableInterface.h"
#include "DoorGimmick.generated.h"

class UStaticMeshComponent;
class UCameraComponent;
class UCurveFloat;

UCLASS()
class MYPROJECT_API ADoorGimmick : public AActor, public ISwitchableInterface
{
	GENERATED_BODY()

public:
	ADoorGimmick();

protected:
	virtual void BeginPlay() override;

	virtual void OnActivate_Implementation() override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USceneComponent> RootComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> FrameMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> DoorMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> CutsceneCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UTimelineComponent> DoorTimeline;

	UPROPERTY(EditAnywhere, Category = "Animation")
	TObjectPtr<UCurveFloat> DoorCurve;

	UPROPERTY(EditAnywhere, Category = "Cinematic")
	float CameraBlendTime = 0.75f;

	UPROPERTY(EditAnywhere, Category = "Cinematic")
	float CameraHoldTime = 1.0f;

	UPROPERTY(EditAnywhere, Category = "Animation")
	float MaxYawAngle = 90.0f;
	//UPROPERTY(EditInstanceOnly, Category="Gimmick")
	//TArray<AActor*>RequiredSwitches;
 

	bool bIsOpen = false;

	UFUNCTION()
	void UpdateDoorRotation(float Output);

	UFUNCTION()
	void OnTimelineFinished();

	void StartDoorAnimation();
	void ReturnCameraToPlayer();

	FTimerHandle CameraDelayTimerHandle;
	FTimerHandle ReturnCameraTimerHandle;
	FTimerHandle EnableInputTimerHandle;
};

