// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"
#include "DrawDebugHelpers.h"
#include "Engine/Engine.h"
#include "Kismet/GameplayStatics.h"


AWeapon::AWeapon()
{
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshComp"));
	RootComponent = MeshComp;

	Player = Cast<AShooter_GameCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
}

void AWeapon::BeginPlay()
{
	Super::BeginPlay();

}

void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWeapon::Fire()
{
	GetWorld()->GetTimerManager().SetTimer(FireHandle, this, &AWeapon::AutoFire, ShootRate, true, 0.0f);
}

void AWeapon::StopFire()
{
	GetWorld()->GetTimerManager().ClearTimer(FireHandle);
}

void AWeapon::AutoFire()
{
	if (LoadedAmmo > 0) {
		if (Player)
		{
			FVector Location;
			FRotator Rotation;
			Player->GetActorEyesViewPoint(Location, Rotation);

			FVector LineEnd = Location + (Rotation.Vector() * (ShootRangeInMeters * 100.0f));

			FCollisionQueryParams QueryParams;
			QueryParams.AddIgnoredActor(Player);
			QueryParams.AddIgnoredActor(this);
			QueryParams.bTraceComplex = true;

			FHitResult Hit;
			bool isTrace = GetWorld()->LineTraceSingleByChannel(Hit, Location, LineEnd, ECC_Visibility, QueryParams);

			if (isTrace)
			{
				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, TEXT("Trace!"));
			}

			DrawDebugLine(GetWorld(), Location, LineEnd, FColor::Red, false, 5.0f);
			// PlayAnimation(FireAnimation);

			LoadedAmmo--;
			CheckAmmo();
		}
	}
}

void AWeapon::Reload()
{
	if (AmmoPool >= (MaxLoadedAmmo - LoadedAmmo)) {
		AmmoPool -= (MaxLoadedAmmo - LoadedAmmo);
		LoadedAmmo = MaxLoadedAmmo;
		bIsNeedReload = false;
		return;
	}
	LoadedAmmo += AmmoPool;
	AmmoPool = 0;
	bIsNeedReload = false;
}

void AWeapon::CheckAmmo()
{
	(LoadedAmmo <= LowAmmoMessage && AmmoPool > 0) ? bIsNeedReload = true : bIsNeedReload = false;
	(LoadedAmmo <= 0 && AmmoPool <= 0) ? bIsNoAmmo = true : bIsNoAmmo = false;
}

void AWeapon::PlayAnimation(UAnimMontage* AnimMontage)
{
	if (AnimMontage != nullptr)
	{
		UAnimInstance* AnimInstance = Player->GetMesh()->GetAnimInstance();
		if (AnimInstance != nullptr)
		{
			AnimInstance->Montage_Play(AnimMontage, 1.0f);
		}
	}
}

int AWeapon::GetLoadedAmmo() const
{
	return LoadedAmmo;
}

int AWeapon::GetMaxLoadedAmmo() const
{
	return MaxLoadedAmmo;
}

int AWeapon::GetAmmoPool() const
{
	return AmmoPool;
}

int AWeapon::GetMaxAmmoPool() const
{
	return MaxAmmoPool;
}

bool AWeapon::IsNeedReload() const
{
	return bIsNeedReload;
}

bool AWeapon::IsNoAmmo() const
{
	return bIsNoAmmo;
}