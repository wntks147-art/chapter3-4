#include "Item.h"



AItem::AItem()
{
    SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
    SetRootComponent(SceneRoot);

    StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
    StaticMeshComp->SetupAttachment(SceneRoot);

    

    static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("/Game/Resources/Props/SM_Star_B.SM_Star_B"));
    if (MeshAsset.Succeeded()) {
        StaticMeshComp->SetStaticMesh(MeshAsset.Object);
    } 

    static ConstructorHelpers::FObjectFinder<UMaterial> MaterialAsset(TEXT("/Game/Resources/Materials/M_Star.M_Star"));
    if (MaterialAsset.Succeeded())
    {
        StaticMeshComp->SetMaterial(0,MaterialAsset.Object);
    }

    PrimaryActorTick.bCanEverTick = true;
    RotationSpeed = 90.0f;

    MoveSpeed = 200.0f;
    MaxRange = 300.0f;
    MoveDirection = FVector(1.0f, 0.0f, 0.0f);
    
}


void AItem::BeginPlay()
{
    Super::BeginPlay();

   
    StartLocation = GetActorLocation();

}

void AItem::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (!FMath::IsNearlyZero(RotationSpeed))
    {
        AddActorLocalRotation(FRotator(0.0f, RotationSpeed * DeltaTime, 0.0f));
    }

    //이동구현기능

    FVector CurrentLocation = GetActorLocation();
    CurrentLocation += MoveDirection * MoveSpeed * DeltaTime;
    SetActorLocation(CurrentLocation);

    float DistanceMoved = FVector::Dist(StartLocation, CurrentLocation);
    if (DistanceMoved >= MaxRange)
    {
        FVector MoveDirNorm = MoveDirection.GetSafeNormal();
        CurrentLocation = StartLocation + (MoveDirection * MaxRange);
        SetActorLocation(CurrentLocation);

        MoveDirection = -MoveDirection;
    }
    
}




