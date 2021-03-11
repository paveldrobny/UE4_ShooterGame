// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Shooter_GameCharacter.generated.h"

UCLASS(config = Game)
class AShooter_GameCharacter : public ACharacter
{
	GENERATED_BODY()

		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* FollowCamera;

public:

	AShooter_GameCharacter();

	// Camera
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseTurnRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseLookUpRate;

	float VDeltaTime;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Camera)
		float SpeedCameraPositon = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
		FVector NormalCameraPosition;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
		FVector RunningCameraPosition;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
		FVector CrouchCameraPosition;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
		FVector TargetingCameraPosition;

	// Character
	float CurrentHealth;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	float MaxHealth = 100.0f;

	UFUNCTION(BlueprintCallable, Category = Character)
		float PercentHealth() const;

	// Movement
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement)
		float CrouchSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement)
		float RunningSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement)
		float NormalSpeed;

	bool bIsRunning;
	bool bIsCrouch;

	UFUNCTION(BlueprintCallable, Category = Movement)
		bool IsRunning() const;

	UFUNCTION(BlueprintCallable, Category = Movement)
		bool IsCrouch() const;

	// Weapon
	class AWeapon* CurrentWeapon;

	int CurrentWeaponIndex = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<AWeapon*> Inventory;

	void AddToInventory(class AWeapon* Weapon);

	UFUNCTION(BlueprintCallable, Category = Weapon)
		void CreateWeapon(TSubclassOf<AWeapon> WeaponClass, FName SocketName);

	UFUNCTION(BlueprintCallable, Category = Weapon)
		void SelectWeapon(int index);

	UFUNCTION(BlueprintCallable, Category = Weapon)
		int GetCurrentWeaponIndex() const;


protected:

	void MoveForward(float Value);
	void MoveRight(float Value);
	void TurnAtRate(float Rate);
	void LookUpAtRate(float Rate);

	void CheckCameraPosition();
	void SetCameraPosition(FVector Position);
	void SetMaxSpeed(float value);
	void SetRunning(bool value);
	void SetCrouching(bool value);

	void OnStartRunning();
	void OnStopRunning();
	void OnStartCrouch();
	void OnStopCrouch();
	void OnStartFire();
	void OnStopFire();
	void OnStartTargeting();
	void OnStopTargeting();
	void OnReload();
	void EquipPrimaryWeapon();
	void EquipSecondaryWeapon();


protected:

	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};

