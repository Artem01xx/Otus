// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpeedBoost.generated.h"

UCLASS()
class THIRDPERSONSHOOTER_API ASpeedBoost : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpeedBoost(const FObjectInitializer& objectInitializer);
	
	void BindOverlapTimerFinished();
	UFUNCTION(BlueprintCallable, Category = "Ability")

	virtual void BeginPlay();
	virtual void Tick(float DeltatSeconds);
	
private:
	UPROPERTY(EditAnyWhere)
	class USphereComponent* SphereComponent;
	UPROPERTY(EditAnyWhere)
	class UStaticMeshComponent* StaticMesh;
	UPROPERTY(EditDefaultsOnly)
	float BaseTurnRate = 45.0f;
	UPROPERTY(EditDefaultsOnly)
	class USoundCue* PickUpSound;
	UPROPERTY(EditAnyWhere)
	class UNiagaraComponent* PickUpEffectComponent;
	UPROPERTY(EditAnyWhere)
	class UNiagaraSystem* PickUpEffect;

	FTimerHandle BindOverlapTimer;
	float BindOverlapTime = 0.25f;

	

protected:
	UFUNCTION()
	virtual void BeginOverlap(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);
	virtual void Destroyed();

	UFUNCTION()
	virtual void EndOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, 
		class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
