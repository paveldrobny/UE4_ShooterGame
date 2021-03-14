// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Shooter_GameCharacter.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

//class USkeletalMeshComponent;
class UAnimMontage;


UCLASS()
class SHOOTER_GAME_API AWeapon : public AActor
{
	GENERATED_BODY()

public:
	AWeapon();

	void Fire();
	void StopFire();
	void Reload();

	FTimerHandle FireHandle;
	bool isFiring;

protected:

	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		USkeletalMeshComponent* MeshComp;

	class AShooter_GameCharacter* Player;

	void AutoFire();
	void CheckAmmo();


	// Weapon Instance
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponInstance")
		float ShootRangeInMeters;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponInstance")
		float ShootRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponInstance")
		int LoadedAmmo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponInstance")
		int MaxLoadedAmmo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponInstance")
		int AmmoPool;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponInstance")
		int MaxAmmoPool;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponInstance")
		int LowAmmoMessage = -1;


	// Weapon Info
	bool bIsNeedReload;
	bool bIsNoAmmo;

public:

	UFUNCTION(BlueprintCallable, Category = WeaponInfo)
		int GetLoadedAmmo() const;

	UFUNCTION(BlueprintCallable, Category = WeaponInfo)
		int GetMaxLoadedAmmo() const;

	UFUNCTION(BlueprintCallable, Category = WeaponInfo)
		int GetAmmoPool() const;

	UFUNCTION(BlueprintCallable, Category = WeaponInfo)
		int GetMaxAmmoPool() const;

	UFUNCTION(BlueprintCallable, Category = WeaponInfo)
		bool IsNeedReload() const;

	UFUNCTION(BlueprintCallable, Category = WeaponInfo)
		bool IsNoAmmo() const;

	UFUNCTION(BlueprintCallable, Category = WeaponInfo)
		bool CanFire() const;

public:

	virtual void Tick(float DeltaTime) override;
};
