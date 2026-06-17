// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "SpartaCharacter.generated.h"


class UCapsuleComponent;
class USkeletalMeshComponent;
class USpringArmComponent;
class UCameraComponent;
struct FInputActionValue;

UCLASS()
class SPARTAPROJECT_API ASpartaCharacter : public APawn
{
	GENERATED_BODY()

public:

	ASpartaCharacter();
	
	virtual FVector GetVelocity() const override;
	
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components|Collision")
	UCapsuleComponent* CapsuleComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components|Mesh")
	USkeletalMeshComponent* MeshComp;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	USpringArmComponent* SpringArmComp;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	UCameraComponent* CameraComp;

protected:
	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	UFUNCTION()
	void Move(const FInputActionValue& value);
	UFUNCTION()
	void StartJump(const FInputActionValue& value);
	UFUNCTION()
	void StopJump(const FInputActionValue& value);
	UFUNCTION()
	void Look(const FInputActionValue& value);
	UFUNCTION()
	void StartSprint(const FInputActionValue& value);
	UFUNCTION()
	void StopSprint(const FInputActionValue& value);
	
private:
	
	FVector2D MoveInput;
	FVector2D LookInput;
	
	FVector TrackedVelocity = FVector::ZeroVector;
	
	float CurrentSpeed;
    
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float NormalSpeed = 600.0f;
    
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float SprintSpeed = 1000.0f;
    
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float RotationSpeed = 90.0f;

	
	bool bIsJumping = false;
	float JumpTimer = 0.0f;
    
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement|Jump", meta = (AllowPrivateAccess = "true"))
	float JumpDuration = 0.5f; 
    
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement|Jump", meta = (AllowPrivateAccess = "true"))
	float JumpHeight = 150.0f; 
    
	float BaseZValue = 0.0f;
};