// Copyright Ludens Studio. All Rights Reserved.


#include "Actor/ArenaEffectActor.h"

#include "AbilitySystemInterface.h"
#include "AbilitySystem/ArenaHealthSet.h"
#include "Components/SphereComponent.h"

AArenaEffectActor::AArenaEffectActor()
{
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;

	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	Sphere->SetupAttachment(Mesh);
}

void AArenaEffectActor::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (IAbilitySystemInterface* ASCInterface = Cast<IAbilitySystemInterface>(OtherActor))
	{
		const UArenaHealthSet* HealthSet = ASCInterface->GetAbilitySystemComponent()->GetSet<UArenaHealthSet>();
		if (UArenaHealthSet* MutableHealthSet = const_cast<UArenaHealthSet*>(HealthSet))
		{
			MutableHealthSet->SetHealth(HealthSet->GetHealth() - 10.0f);
			Destroy();
		}
	}
}

void AArenaEffectActor::EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}

// Called when the game starts or when spawned
void AArenaEffectActor::BeginPlay()
{
	Super::BeginPlay();

	Sphere->OnComponentBeginOverlap.AddDynamic(this, &AArenaEffectActor::OnOverlap);
	Sphere->OnComponentEndOverlap.AddDynamic(this, &AArenaEffectActor::EndOverlap);
}

