#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Item.generated.h"



UCLASS()
class SPARTAPROJECT_API AItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItem();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly,Category = "Item|Components")
	USceneComponent* SceneRoot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Components")
	UStaticMeshComponent* StaticMeshComp;

	UPROPERTY(EditAnywhere ,BlueprintReadWrite, Category="Item|Properties")
	float RotationSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Components")
	FVector MoveDirection;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Components")
	float MoveSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Components")
	float MaxRange;

	FVector StartLocation;
	


	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	
};
