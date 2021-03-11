// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"
#include "DrawDebugHelpers.h"

AWeapon::AWeapon()
{
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshComp"));
	RootComponent = MeshComp;
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
	AActor* MyOwner = GetOwner();
	if (MyOwner)
	{
		FVector Location;
		FRotator Rotation;
		MyOwner->GetActorEyesViewPoint(Location, Rotation);

		FVector LineEnd = Location + (Rotation.Vector() * ShotRange);

		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(MyOwner);
		QueryParams.AddIgnoredActor(this);
		QueryParams.bTraceComplex = true;

		FHitResult Hit;

		bool isTrace = GetWorld()->LineTraceSingleByChannel(Hit, Location, LineEnd, ECC_Visibility, QueryParams);

		if (isTrace)
		{
			DrawDebugLine(GetWorld(), Location, LineEnd, FColor::Red, false, 1.0f);
		}
	}

}

void AWeapon::Reload()
{
	if (AmmoPool >= (MaxLoadedAmmo - LoadedAmmo)) {
		AmmoPool -= (MaxLoadedAmmo - LoadedAmmo);
		LoadedAmmo = MaxLoadedAmmo;
		return;
	}

	LoadedAmmo += AmmoPool;
	AmmoPool = 0;
}

void AWeapon::CheckAmmo()
{
	LoadedAmmo <= LowAmmoReload && AmmoPool > 0 ? bIsNeedReload = true : bIsNeedReload = false;
	LoadedAmmo <= 0 && AmmoPool <= 0 ? bIsNoAmmo = true : bIsNoAmmo = false;
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