// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "Resource_M.h"
#include "Kismet/GameplayStatics.h"
#include "BuildingPart.h"
#include "PlayerWidget.h"
#include "PlayerChar.generated.h"

UCLASS()
class GAM312_PROJ_PINCIN_API APlayerChar : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerChar();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
UFUNCTION()
	void MoveForward(float axisValue);
	// this calls the UFunction in the player cpp file that allows the movement

UFUNCTION()
	void MoveRight(float axisValue);

UFUNCTION()
	void StartJump();

UFUNCTION()
	void StopJump();

UFUNCTION()
	void FindObject();
	
UPROPERTY(VisibleAnywhere)
	UCameraComponent* PlayerCamComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Stats")
	float Health = 100.0f;
	// blueprint read write allows for us to edit it inside of a blueprint incase we want to do simple adjustments. 
	// edit anywhere gives us visibility to change this variable in the class
	// player stats is the category it will go inside the player class ex: transform on the right side

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Stats")
	float Hunger = 100.0f;
	// sets starting hunger

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Stats")
	float Stamina = 100.0f;
	// creates your starting stamina

	UFUNCTION(BlueprintCallable)
	void SetHealth(float amount);
	// callable variable for health and enables you to set your health parameters
	UPROPERTY(EditAnywhere, Category = "Resources")
	int Wood;
	// initiates the integer to collect Wood

	UPROPERTY(EditAnywhere, Category = "Resources")
	int Stone;
	// initiates the integer to collect stone

	UPROPERTY(EditAnywhere, Category = "Resources")
	int Berry;
	// initiates the integer to collect Berries

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resources")
	TArray<int> ResourcesArray;
	// allows you to edit the resources and use them as variables in the class settings

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resources")
	TArray<FString> ResourcesNameArray;

	UPROPERTY(EditAnywhere, Category = "HitMarker")
	UMaterialInterface* hitDecal;
	// allows for you to see a visual hit marker when hitting each resource

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building Supplies")
	TArray<int> BuildingArray;

	UPROPERTY()
	bool isBuilding;
	// checks if you are building

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<ABuildingPart> BuildPartClass;

	UPROPERTY()
	ABuildingPart* spawnedPart;
	// allows you to spawn a part

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UPlayerWidget* playerUI;

	UFUNCTION(BlueprintCallable)
	void SetHunger(float amount);
	// callable variable for hunger and enables you to set your hunger parameters
	UFUNCTION(BlueprintCallable)
	void SetStamina(float amount);
	// callable variable for stamina and enables you to set your stamina parameters
	UFUNCTION()
	void DecreaseStats();
	// sets up the function to decrease your stats in the player.h file

	UFUNCTION()
	void GiveResource(float amount, FString resourceType);
	//allows you to set up how man resources will increase per hit when you are farming them
	UFUNCTION(BlueprintCallable)
	void UpdateResources(float woodAmount, float stoneAmount, FString buildingObject);
	// this updates our resources in our widget as we utilize them

	UFUNCTION(BlueprintCallable)
	void SpawnBuilding(int buildingID, bool& isSuccess);
	// this allows a building part to be created

	UFUNCTION()
	void RotateBuilding();
	// allows building part ot be rotated
};
