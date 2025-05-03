// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerChar.h"

// Sets default values
APlayerChar::APlayerChar()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PlayerCamComp = CreateDefaultSubobject<UCameraComponent>(TEXT("First Person Cam"));
	// creates the camera component

	PlayerCamComp->SetupAttachment(GetMesh(), "Head");
	// attaches camera to the player mesh's head

	PlayerCamComp->bUsePawnControlRotation = true;
	// allows the camera to rotate with the players head movement
	BuildingArray.SetNum(3);
	ResourcesArray.SetNum(3);
	// sets the resource array and sets how many items will be inside of the array
	ResourcesNameArray.Add(TEXT("Wood"));
	// Adds the link "Wood" to array space (0)
	ResourcesNameArray.Add(TEXT("Stone"));
	// stone for array (1)
	ResourcesNameArray.Add(TEXT("Berry"));
	// berry for array (2)

}

// Called when the game starts or when spawned
void APlayerChar::BeginPlay()
{
	Super::BeginPlay();
	FTimerHandle StatsTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(StatsTimerHandle, this, &APlayerChar::DecreaseStats, 2.0f, true);
	// sets the time for the players stats to decrease and recover for stamina hunger and health
	if (objWidget)
	{
		objWidget->UpdatebuildObj(0.0f);
		objWidget->UpdatematOBJ(0.0f);
	}
}

// Called every frame
void APlayerChar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	playerUI->UpdateBars(Health, Hunger, Stamina);

	if (isBuilding)
		// checks if the player is building
	{
		if (spawnedPart)
		{ // location of the building component when spawned into the player's viewport
			FVector StartLocation = PlayerCamComp->GetComponentLocation();
			FVector Direction = PlayerCamComp->GetForwardVector() * 400.0f;
			FVector EndLocation = StartLocation + Direction;
			spawnedPart->SetActorLocation(EndLocation);
		}
	}

}

// Called to bind functionality to input
void APlayerChar::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis("MoveForward", this, &APlayerChar::MoveForward);
	// Allows the player to move forward and backwards
	PlayerInputComponent->BindAxis("MoveRight", this, &APlayerChar::MoveRight);
	// Allows the player to move right and left
	PlayerInputComponent->BindAxis("LookUp", this, &APlayerChar::AddControllerPitchInput);
	// Allows the player to look up and down with the mouse Y axis
	PlayerInputComponent->BindAxis("Turn", this, &APlayerChar::AddControllerYawInput);
	// Allows the player to look left and right with the mouse X axis
	PlayerInputComponent->BindAction("JumpEvent", IE_Pressed, this, &APlayerChar::StartJump);
	// Allow player to jump with the space bar pressed
	PlayerInputComponent->BindAction("JumpEvent", IE_Released, this, &APlayerChar::StopJump);
	// Forces stop jump when the space bar is released
	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &APlayerChar::FindObject);
	// allows the player to interact with objects within the parameters set up for distance and viewport
	PlayerInputComponent->BindAction("RotPart", IE_Pressed, this, &APlayerChar::RotateBuilding);
	// input that allows us to rotate our piece 90 degrees

}
void APlayerChar::MoveForward(float axisValue)
{
	FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::X);
	AddMovementInput(Direction, axisValue);
	// allows moovement forward and backward
}

void APlayerChar::MoveRight(float axisValue)
{
	FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::Y);
	AddMovementInput(Direction, axisValue);
	// allows movement left and right
}

void APlayerChar::StartJump()
{
	bPressedJump = true;
	// press to jump
}

void APlayerChar::StopJump()
{
	bPressedJump = false;
	// not pressing space means you won't jump
}

void APlayerChar::FindObject()
{
	FHitResult HitResult;
	FVector StartLocation = PlayerCamComp->GetComponentLocation();
	// gets the component in front of you
	FVector Direction = PlayerCamComp->GetForwardVector() * 800.0f;
	// sets the forward distance to be able to interact with the object
	FVector EndLocation = StartLocation + Direction;

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);
	// ignores player character 
	QueryParams.bTraceComplex = true;
	QueryParams.bReturnFaceIndex = true;
	if (!isBuilding)
		// if we aren't building, then we can hit resources
	{
		if (GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_Visibility, QueryParams))
		{
			AResource_M* HitResource = Cast<AResource_M>(HitResult.GetActor());

			if (Stamina > 5.0f)
				// if stamina is below 5 you can not hit the resource
			{
				if (HitResource)
				{
					FString hitName = HitResource->resourceName;
					//
					int resourceValue = HitResource->resourceAmount;
					// initializes the resource value 

					HitResource->totalResource = HitResource->totalResource - resourceValue;
					// depleats the resource per hit by 5
					if (HitResource->totalResource > resourceValue)
					{
						GiveResource(resourceValue, hitName);
						// gives you the resource that is in the viewport being hit
						matsCollected = matsCollected + resourceValue;

						objWidget->UpdatematOBJ(matsCollected);
						check(GEngine != nullptr);
						GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Resource Collected"));
						// notifies you each time you hit the resource that it has been collected
						UGameplayStatics::SpawnDecalAtLocation(GetWorld(), hitDecal, FVector(10.0f, 10.0f, 10.0f), HitResult.Location, FRotator(-90, 0, 0), 2.0f);
						// sets the hit decal to spawn where your are hitting the the resource
						SetStamina(-5.0f);
						// sets the decrease in 5 stamina per hit
					}
					else
					{
						HitResource->Destroy();
						// destroys the resource when it is depleated
						check(GEngine != nullptr);
						GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("ResourceDepleted"));
						// prints out and confirms with a message when the resource is depleated
					}
				}
			}
		}
	}
	else
	{
		isBuilding = false;
		objectsBuilt = objectsBuilt + 1.0f;

		objWidget->UpdatebuildObj(objectsBuilt);

		objectsBuilt = objectsBuilt + 1.0f;

		objWidget->UpdatebuildObj(objectsBuilt);
}
void APlayerChar::SetHealth(float amount)
{
	if (Health + amount < 100)
	{
		Health = Health + amount;
	}
}
// sets you total health amount

void APlayerChar::SetHunger(float amount)
{
	if (Hunger + amount < 100)
	{
		Hunger = Hunger + amount;
	}
}
// sets your total hunger level

void APlayerChar::SetStamina(float amount)
{
	if (Stamina + amount <= 100)
	{
		Stamina = Stamina + amount;
	}
}
// sets your total stamina level

void APlayerChar::DecreaseStats()
{
	if (Hunger > 0)
	{
		SetHunger(-1.0f);
	}
	SetHunger(-1.0f);
	// hunger decreases by 1 over time
	SetStamina(10.0f);
	// stamina regenerates by 10 if not being used

	if (Hunger <= 0)
	{
		SetHealth(-3.0f);
		// if your hunger has reached 0 then your health will start to decrease by 3
	}
}
void APlayerChar::GiveResource(float amount, FString resourceType)
{ // this sets up our resource array and allows us to collect a specific amount with each hit
	if (resourceType == "Wood")
	{
		ResourcesArray[0] = ResourcesArray[0] + amount;
	}
	if (resourceType == "Stone")
	{
		ResourcesArray[1] = ResourcesArray[1] + amount;
	}
	if (resourceType == "Berry")
	{
		ResourcesArray[2] = ResourcesArray[2] + amount;
	}
	// assigns resources to the resource array and enables you to collect them based on the component the resource is assigned to
}

void APlayerChar::UpdateResources(float woodAmount, float stoneAmount, FString buildingObject)
{ // this updates our resources as we collect them and use them
	if (woodAmount <= ResourcesArray[0])
	{
		if (stoneAmount <= ResourcesArray[1])
		{
			ResourcesArray[0] = ResourcesArray[0] - woodAmount;
			ResourcesArray[1] = ResourcesArray[1] - stoneAmount;

			if (buildingObject == "Wall")
			{
				BuildingArray[0] = BuildingArray[0] + 1;
			}
			if (buildingObject == "Floor")
			{
				BuildingArray[1] = BuildingArray[1] + 1;
			}
			if (buildingObject == "Ceiling")
			{
				BuildingArray[2] = BuildingArray[2] + 1;
			}
		}
	}
}
void APlayerChar::SpawnBuilding(int buildingID, bool& isSuccess)
{ // this sets where and what we spawn when clicking the buttons set into the widget
	if (!isBuilding)
	{
		if (BuildingArray[buildingID] >= 1)
		{
			isBuilding = true;
			FActorSpawnParameters SpawnParams;
			FVector StartLocation = PlayerCamComp->GetComponentLocation();
			// spawns selected building component at the players viewport
			FVector Direction = PlayerCamComp->GetForwardVector() * 400.0f;
			// how far away from the player the piece spawns into the viewport
			FVector EndLocation = StartLocation + Direction;
			FRotator myRot(0, 0, 0);
			// initial rotation of the piece

			BuildingArray[buildingID] = BuildingArray[buildingID] - 1;
			// grabs piece in our building array

			spawnedPart = GetWorld()->SpawnActor<ABuildingPart>(BuildPartClass, EndLocation, myRot, SpawnParams);

			isSuccess = true;
		}
		isSuccess = false;
	}
}
void APlayerChar::RotateBuilding()
{ // allows for the building part to be rotated when the input is pressed by 90 degrees
	if (isBuilding)
	{
		spawnedPart->AddActorWorldRotation(FRotator(0, 90, 0));
	}
}