#include "SpartaCharacter.h"
#include "SpartaPlayerController.h"
#include "EnhancedInputComponent.h"
#include "Components/CapsuleComponent.h"      
#include "Components/SkeletalMeshComponent.h" 
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"


ASpartaCharacter::ASpartaCharacter()
{
    PrimaryActorTick.bCanEverTick = true;

    CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComp"));
    SetRootComponent(CapsuleComp);

    MeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshComp"));
    MeshComp->SetupAttachment(CapsuleComp);

    SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
    SpringArmComp->SetupAttachment(CapsuleComp);
    SpringArmComp->TargetArmLength = 300.0f;
    SpringArmComp->bUsePawnControlRotation = false; 

    CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    CameraComp->SetupAttachment(SpringArmComp, USpringArmComponent::SocketName);
    CameraComp->bUsePawnControlRotation = false;

    CapsuleComp->SetSimulatePhysics(false);
    MeshComp->SetSimulatePhysics(false);

    CurrentSpeed = NormalSpeed;
}


void ASpartaCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
    
    if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
    {
       if (ASpartaPlayerController* PlayerController = Cast<ASpartaPlayerController>(GetController()))
       {
          if (PlayerController->MoveAction)
          {
             EnhancedInput->BindAction(PlayerController->MoveAction, ETriggerEvent::Triggered, this, &ASpartaCharacter::Move);
          }
          if (PlayerController->LookAction)
          {
             EnhancedInput->BindAction(PlayerController->LookAction, ETriggerEvent::Triggered, this, &ASpartaCharacter::Look);
          }
          if (PlayerController->JumpAction)
          {
             EnhancedInput->BindAction(PlayerController->JumpAction, ETriggerEvent::Triggered, this, &ASpartaCharacter::StartJump);
             EnhancedInput->BindAction(PlayerController->JumpAction, ETriggerEvent::Completed, this, &ASpartaCharacter::StopJump);
          }
          if (PlayerController->SprintAction)
          {
             EnhancedInput->BindAction(PlayerController->SprintAction, ETriggerEvent::Triggered, this, &ASpartaCharacter::StartSprint);
             EnhancedInput->BindAction(PlayerController->SprintAction, ETriggerEvent::Completed, this, &ASpartaCharacter::StopSprint);
          }
       }
    }
}


void ASpartaCharacter::Move(const FInputActionValue& value)
{
    MoveInput = value.Get<FVector2D>();
}

void ASpartaCharacter::Look(const FInputActionValue& value)
{
    LookInput = value.Get<FVector2D>();
}


void ASpartaCharacter::StartSprint(const FInputActionValue& value)
{
    CurrentSpeed = SprintSpeed;
}

void ASpartaCharacter::StopSprint(const FInputActionValue& value)
{
    CurrentSpeed = NormalSpeed;
}


void ASpartaCharacter::StartJump(const FInputActionValue& value)
{
    if (!bIsJumping)
    {
        bIsJumping = true;
        JumpTimer = 0.0f;
        BaseZValue = GetActorLocation().Z; 
    }
}

void ASpartaCharacter::StopJump(const FInputActionValue& value)
{
    
}



FVector ASpartaCharacter::GetVelocity() const
{
    return TrackedVelocity;
}


void ASpartaCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

   
    TrackedVelocity = FVector::ZeroVector;

    if (!MoveInput.IsNearlyZero())
    {
        FVector Direction = (GetActorForwardVector() * MoveInput.X) + (GetActorRightVector() * MoveInput.Y);
        Direction.Normalize();
        
        FVector NewOffset = Direction * CurrentSpeed * DeltaTime;
        
        // [수정된 부분] 기존 AddActorLocalOffset에서 AddActorWorldOffset으로 변경 (월드 방향 벡터를 반영하기 위함)
        AddActorWorldOffset(NewOffset, true);

        
        TrackedVelocity = Direction * CurrentSpeed;
    }

    if (!LookInput.IsNearlyZero())
    {
        float TargetYaw = LookInput.X * RotationSpeed * DeltaTime;
        FRotator NewActorRotation = FRotator(0.0f, TargetYaw, 0.0f);
        AddActorLocalRotation(NewActorRotation);

        float TargetPitch = -LookInput.Y * RotationSpeed * DeltaTime; 
        FRotator NewArmRotation = SpringArmComp->GetRelativeRotation();
        
        NewArmRotation.Pitch = FMath::Clamp(NewArmRotation.Pitch + TargetPitch, -60.0f, 60.0f);
        SpringArmComp->SetRelativeRotation(NewArmRotation);
    }

    if (bIsJumping)
    {
        JumpTimer += DeltaTime;
        
        if (JumpTimer >= JumpDuration)
        {
            bIsJumping = false;
            FVector FinalLocation = GetActorLocation();
            FinalLocation.Z = BaseZValue;
            SetActorLocation(FinalLocation);
        }
        else
        {
            float NormalizedTime = JumpTimer / JumpDuration;
            float HeightOffset = FMath::Sin(NormalizedTime * PI) * JumpHeight;

            FVector NewLocation = GetActorLocation();
            NewLocation.Z = BaseZValue + HeightOffset;
            SetActorLocation(NewLocation);
        }
    }

    MoveInput = FVector2D::ZeroVector;
    LookInput = FVector2D::ZeroVector;
}