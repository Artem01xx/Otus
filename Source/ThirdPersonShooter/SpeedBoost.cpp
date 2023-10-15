// Fill out your copyright notice in the Description page of Project Settings.


#include "SpeedBoost.h"
#include "Components/SphereComponent.h"
#include "Pick_UpInterface.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Sound/SoundCue.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"



// Sets default values
ASpeedBoost::ASpeedBoost(const FObjectInitializer& objectInitializer) : Super(objectInitializer)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SphereComponent->SetupAttachment(RootComponent);
	SphereComponent->SetSphereRadius(100.0f);
	SphereComponent->AddLocalOffset(FVector(0.0f, 0.0f, 50.0f));
	SphereComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	SphereComponent->SetCollisionResponseToChannel(ECC_Pawn, ECollisionResponse::ECR_Overlap);
	SphereComponent->SetHiddenInGame(true);

	StaticMesh = CreateDefaultSubobject <UStaticMeshComponent> (TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(SphereComponent);
	StaticMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	StaticMesh->SetRelativeScale3D(FVector(5.0f, 5.0f, 5.0f));

	

	PickUpEffectComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("PickUpEffectComponent"));
	PickUpEffectComponent->SetupAttachment(RootComponent);
}

//___________________________________________________________________________________

void ASpeedBoost::BeginPlay()
{
	Super::BeginPlay();
	GetWorldTimerManager().SetTimer(BindOverlapTimer, this, &ASpeedBoost::BindOverlapTimerFinished, BindOverlapTime);
}


//___________________________________________________________________________________
void ASpeedBoost::Tick( float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (StaticMesh != nullptr)
	{
		StaticMesh->AddWorldRotation(FRotator(0.0f, BaseTurnRate * DeltaSeconds, 0.0f));
	}
}
//___________________________________________________________________________________

void ASpeedBoost::BindOverlapTimerFinished()
{
	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ASpeedBoost::BeginOverlap);
	SphereComponent->OnComponentEndOverlap.AddDynamic(this, &ASpeedBoost::EndOverlap);
}


//___________________________________________________________________________________

void ASpeedBoost::Destroyed()
{
	StaticMesh->DestroyComponent();
	if (PickUpSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, PickUpSound, GetActorLocation());
	}
	if (PickUpEffect)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, PickUpEffect, GetActorLocation(), GetActorRotation());
	}
}

//___________________________________________________________________________________

void ASpeedBoost::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Overlap"));
	Destroyed();
}

void ASpeedBoost::EndOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("EndOverlap"));
	SphereComponent->DestroyComponent();
}

//___________________________________________________________________________________

