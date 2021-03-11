// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

//class USkeletalMeshComponent;

UCLASS()
class SHOOTER_GAME_API AWeapon : public AActor
{
	GENERATED_BODY()

public:
	AWeapon();

	void Fire();
	void Reload();

	void CheckAmmo();

protected:

	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		USkeletalMeshComponent* MeshComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float ShotRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int LoadedAmmo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int MaxLoadedAmmo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int AmmoPool;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int MaxAmmoPool;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int LowAmmoReload = -1;

	bool bIsNeedReload;
	bool bIsNoAmmo;

	UFUNCTION(BlueprintCallable, Category = WeaponInstance)
		int GetLoadedAmmo() const;

	UFUNCTION(BlueprintCallable, Category = WeaponInstance)
		int GetMaxLoadedAmmo() const;

	UFUNCTION(BlueprintCallable, Category = WeaponInstance)
		int GetAmmoPool() const;

	UFUNCTION(BlueprintCallable, Category = WeaponInstance)
		int GetMaxAmmoPool() const;

	UFUNCTION(BlueprintCallable, Category = WeaponInstance)
		bool IsNeedReload() const;

	UFUNCTION(BlueprintCallable, Category = WeaponInstance)
		bool IsNoAmmo() const;

public:

	virtual void Tick(float DeltaTime) override;
};
