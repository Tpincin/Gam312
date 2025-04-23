// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/TextRenderComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Resource_M.generated.h"

UCLASS()
class GAM312_PROJ_PINCIN_API AResource_M : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AResource_M();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	FString resourceName = "Wood";
	// creates the parent class wood adn allows to create children classes that are assigned to the other resources

	UPROPERTY(EditAnywhere)
	int resourceAmount = 5;
	// when hitting resources you get 5 per hit

	UPROPERTY(EditAnywhere)
	int totalResource = 100;
	// total amount of resources per component

	UPROPERTY(EditAnywhere)
	FText tempText;

	UPROPERTY(EditAnywhere)
	UTextRenderComponent* ResourceNameTxt;
	// places resource name on the resource and makes it visible

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* Mesh;
	// allows for a mesh to be placed into the blueprint
};
