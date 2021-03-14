// Copyright Epic Games, Inc. All Rights Reserved.

#include "Shooter_GameCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Public/Weapon.h"
#include "Engine/Engine.h"

AShooter_GameCharacter::AShooter_GameCharacter()
{
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f;
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	CurrentHealth = MaxHealth;
}

void AShooter_GameCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAction("Running", IE_Pressed, this, &AShooter_GameCharacter::OnStartRunning);
	PlayerInputComponent->BindAction("Running", IE_Released, this, &AShooter_GameCharacter::OnStopRunning);

	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &AShooter_GameCharacter::OnStartCrouch);
	PlayerInputComponent->BindAction("Crouch", IE_Released, this, &AShooter_GameCharacter::OnStopCrouch);

	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AShooter_GameCharacter::OnStartFire);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &AShooter_GameCharacter::OnStopFire);

	PlayerInputComponent->BindAction("Targeting", IE_Pressed, this, &AShooter_GameCharacter::OnStartTargeting);
	PlayerInputComponent->BindAction("Targeting", IE_Released, this, &AShooter_GameCharacter::OnStopTargeting);

	PlayerInputComponent->BindAction("Reload", IE_Pressed, this, &AShooter_GameCharacter::OnReload);

	PlayerInputComponent->BindAction("PrimaryWeapon", IE_Pressed, this, &AShooter_GameCharacter::EquipPrimaryWeapon);
	PlayerInputComponent->BindAction("SecondaryWeapon", IE_Pressed, this, &AShooter_GameCharacter::EquipSecondaryWeapon);

	PlayerInputComponent->BindAxis("MoveForward", this, &AShooter_GameCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AShooter_GameCharacter::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AShooter_GameCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AShooter_GameCharacter::LookUpAtRate);
}

void AShooter_GameCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	VDeltaTime = DeltaTime;
	CheckCameraPosition();
}

void AShooter_GameCharacter::OnStartFire()
{
	if (CurrentWeapon)
	{
		SetFiring(true);
		CurrentWeapon->Fire();
	}
}

void AShooter_GameCharacter::OnStopFire()
{
	if (CurrentWeapon)
	{
		SetFiring(false);
		CurrentWeapon->StopFire();
	}
}

void AShooter_GameCharacter::OnStartTargeting()
{
	SetTargeting(true);
	SetMaxSpeed(TargetingSpeed);
}

void AShooter_GameCharacter::OnStopTargeting()
{
	if (bIsCrouched)
	{
		SetTargeting(false);
		return;
	}
	SetTargeting(false);
	SetMaxSpeed(NormalSpeed);
}

void AShooter_GameCharacter::OnReload()
{
	if (CurrentWeapon)
	{
		CurrentWeapon->Reload();
	}
}

void AShooter_GameCharacter::EquipPrimaryWeapon()
{
	SelectWeapon(0);
}

void AShooter_GameCharacter::EquipSecondaryWeapon()
{
	if (Inventory.Num() >= 1)
	{
		SelectWeapon(1);
	}
}

void AShooter_GameCharacter::OnStartRunning()
{
	if (!bIsCrouch)
	{
		if (!GetVelocity().IsZero())
		{
			SetRunning(true);
			SetMaxSpeed(RunningSpeed);
		}
	}
}

void AShooter_GameCharacter::OnStopRunning()
{
	SetMaxSpeed(NormalSpeed);
	SetRunning(false);
}

void AShooter_GameCharacter::OnStartCrouch()
{
	if (bIsRunning) {
		SetRunning(false);
	}
	SetCrouching(true);
	SetMaxSpeed(CrouchSpeed);
}

void AShooter_GameCharacter::OnStopCrouch()
{
	SetCrouching(false);
	SetMaxSpeed(NormalSpeed);
}

void AShooter_GameCharacter::CheckCameraPosition()
{
	if (bIsCrouch)
	{
	
		SetCameraPosition(CrouchCameraPosition);
	}
	else if (bIsRunning)
	{
		SetCameraPosition(RunningCameraPosition);
	}
	else if (bIsTargeting)
	{

		SetCameraPosition(TargetingCameraPosition);
	}
	else
	{
		SetCameraPosition(NormalCameraPosition);
	}

	if (bIsCrouch && bIsTargeting)
	{
		SetCameraPosition(CrouchTargetingCameraPosition);
	}
}

void AShooter_GameCharacter::SetCameraPosition(FVector Position)
{
	CameraBoom->SocketOffset = FMath::VInterpTo(this->CameraBoom->SocketOffset, Position, VDeltaTime, SpeedCameraPositon);
}

void AShooter_GameCharacter::SetMaxSpeed(float value)
{
	GetCharacterMovement()->MaxWalkSpeed = value;
}

void AShooter_GameCharacter::SetRunning(bool value)
{
	bIsRunning = value;
}

void AShooter_GameCharacter::SetCrouching(bool value)
{
	bIsCrouch = value;
}

void AShooter_GameCharacter::SetFiring(bool value)
{
	bIsFiring = value;
}

void AShooter_GameCharacter::SetTargeting(bool value)
{
	bIsTargeting = value;
}

bool AShooter_GameCharacter::IsRunning() const
{
	return bIsRunning;
}

bool AShooter_GameCharacter::IsCrouch() const
{
	return bIsCrouch;
}

bool AShooter_GameCharacter::IsFiring() const
{
	return bIsFiring;
}

bool AShooter_GameCharacter::IsTargeting() const
{
	return bIsTargeting;
}

void AShooter_GameCharacter::TurnAtRate(float Rate)
{
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AShooter_GameCharacter::LookUpAtRate(float Rate)
{
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AShooter_GameCharacter::MoveForward(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void AShooter_GameCharacter::MoveRight(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(Direction, Value);
	}
}


// Character
float AShooter_GameCharacter::GetPercentHealth() const
{
	return CurrentHealth / MaxHealth;
}


// Weapon
void AShooter_GameCharacter::CreateWeapon(TSubclassOf<AWeapon> WeaponClass, FName SocketName)
{
	FVector Location = FVector(0.0f, 0.0f, 0.0f);
	FRotator Rotation = FRotator(0.0f, 0.0f, 0.0f);
	FActorSpawnParameters SpawnParams;

	AWeapon* Weapon = GetWorld()->SpawnActor<AWeapon>(WeaponClass, Location, Rotation, SpawnParams);
	Weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, SocketName);

	AddToInventory(Weapon);
}

void AShooter_GameCharacter::AddToInventory(class AWeapon* Weapon)
{
	Inventory.AddUnique(Weapon);
}

void AShooter_GameCharacter::SelectWeapon(int index)
{
	CurrentWeaponIndex = index;
	for (int i = 0; i < Inventory.Num(); i++)
	{
		Inventory[i]->SetActorHiddenInGame(true);
		CurrentWeapon = Inventory[CurrentWeaponIndex];
	}
	Inventory[CurrentWeaponIndex]->SetActorHiddenInGame(false);
}

AWeapon* AShooter_GameCharacter::GetCurrentWeapon() const
{
	return CurrentWeapon;
}

int AShooter_GameCharacter::GetCurrentWeaponIndex() const
{
	return CurrentWeaponIndex;
}