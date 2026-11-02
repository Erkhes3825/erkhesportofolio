#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SwitchActor1.generated.h"

class UBoxComponent;
class UStaticMeshComponent;
class UMaterialInterface;

UCLASS()
class MYPROJECT_API ASwitchActor1 : public AActor
{
	GENERATED_BODY()

public:
	ASwitchActor1();

bool IsSwitchedOn() const {return bIsOn}

protected:
	virtual void BeginPlay() override;

private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USceneComponent> RootComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> SwitchMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UBoxComponent> TriggerBox;

	
	UPROPERTY(EditInstanceOnly, Category = "Gimmick")
	TArray<TObjectPtr<AActor>> TargetGimmicks;

	UPROPERTY(EditAnywhere, Category = "Visual")
	TObjectPtr<UMaterialInterface> OnMaterial;

	UPROPERTY(EditAnywhere, Category = "Visual")
	TObjectPtr<UMaterialInterface> OffMaterial;

	UPROPERTY(EditAnywhere, Category = "Visual")
	float PressedZOffset = -5.0f;

	bool bIsOn = false;


	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void UpdateSwitchVisuals();
};

