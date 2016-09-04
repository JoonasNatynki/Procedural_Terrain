// Fill out your copyright notice in the Description page of Project Settings.

#include "Procedural_Terrain.h"
#include "Terrain.h"

#include "GlobalVariables.h"
#include "HelperFunctions.h"

//############################################################################################################################
// Sets default values
ATerrain::ATerrain()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	_rootSphere = CreateDefaultSubobject<USphereComponent>(TEXT("RootComponent"));
	//sphere->SetSprite(debugBlockRootSpriteTexture);
	_rootSphere->RegisterComponent();
	_rootSphere->SetSphereRadius(CELL_SIZE);
	_rootSphere->SetWorldLocation(this->GetActorLocation());
	_rootSphere->SetWorldRotation(this->GetActorRotation());
	RootComponent = _rootSphere;
	_rootSphere->SetHiddenInGame(false);

}
//############################################################################################################################

//############################################################################################################################
// Called when the game starts or when spawned
void ATerrain::BeginPlay()
{
	Super::BeginPlay();

	// TESTING THREADING #############
	_terrainWorker = new TerrainWorker(this, GetWorld());	// Initializes the terrainWorker object that will spawn threads that work shit
												//WorkerTask task;
												//task.taskToBeDone = 0;
												//task.blockRootCoordinates = FVector(0, 0, 0);
												//terrainWorker->taskQueue.Enqueue(task);
	_terrainWorker->CreateThread();					//Starts a new thread that does shit that you give it
													// INITIALIZE TERRAIN VOXELS
	InitializeTerrainBlocks();
}
//############################################################################################################################

//############################################################################################################################
void ATerrain::BeginDestroy()
{
	DestroyTerrainWorker();
	delete _terrainWorker;

	// Calls the actual BeginDestroy that is not overriden
	Super::BeginDestroy();
}
//############################################################################################################################

//############################################################################################################################
void ATerrain::DestroyTerrainWorker()
{
	if (!_terrainWorker)
		return;

	// Destroy the Worker Thread
	_terrainWorker->EnsureCompletion();
	_terrainWorker->Shutdown();
}
//############################################################################################################################

//############################################################################################################################
// Called every frame
void ATerrain::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	// BLOCK DEBUG LINES ##########################################################
	if (_bShowBlockDebugLines)
	{
		//we lock this shit down so the array can't change while we draw it
		_blockArrayCriticalSection.Lock();
		for (auto &item : _terrainSectionMap)
		{
			DrawDebugLine(
				GetWorld(),
				item.Key,
				FVector(item.Key.X + (BLOCK_SIZE), item.Key.Y, item.Key.Z),
				FColor(255, 0, 0),
				true, -1, 1,
				1);

			DrawDebugLine(
				GetWorld(),
				item.Key,
				FVector(item.Key.X, item.Key.Y + (BLOCK_SIZE), item.Key.Z),
				FColor(255, 0, 0),
				true, -1, 1,
				1);

			DrawDebugLine(
				GetWorld(),
				item.Key,
				FVector(item.Key.X, item.Key.Y, item.Key.Z + (BLOCK_SIZE)),
				FColor(255, 0, 0),
				true, -1, 1,
				1);


			//PrintStr(ToString(getIntVectorFromBlockRootCoordinates(item->GetActorLocation())));
			
			if (getIntVectorFromBlockRootCoordinates(item.Key).X == worldSideX - 1)
			{
				DrawDebugLine(
					GetWorld(),
					FVector(item.Key.X + (BLOCK_SIZE), item.Key.Y, item.Key.Z),
					FVector(item.Key.X + (BLOCK_SIZE), item.Key.Y + (BLOCK_SIZE), item.Key.Z),
					FColor(255, 0, 0),
					true, -1, 1,
					1);
				DrawDebugLine(
					GetWorld(),
					FVector(item.Key.X + (BLOCK_SIZE), item.Key.Y, item.Key.Z),
					FVector(item.Key.X + (BLOCK_SIZE), item.Key.Y, item.Key.Z + (BLOCK_SIZE)),
					FColor(255, 0, 0),
					true, -1, 1,
					1);
			}
			if (getIntVectorFromBlockRootCoordinates(item.Key).Y == worldSideY - 1)
			{
				DrawDebugLine(
					GetWorld(),
					FVector(item.Key.X, item.Key.Y + (BLOCK_SIZE), item.Key.Z),
					FVector(item.Key.X + (BLOCK_SIZE), item.Key.Y + (BLOCK_SIZE), item.Key.Z),
					FColor(255, 0, 0),
					true, -1, 1,
					1);
				DrawDebugLine(
					GetWorld(),
					FVector(item.Key.X, item.Key.Y + (BLOCK_SIZE), item.Key.Z),
					FVector(item.Key.X, item.Key.Y + (BLOCK_SIZE), item.Key.Z + (BLOCK_SIZE)),
					FColor(255, 0, 0),
					true, -1, 1,
					1);
			}


			//PrintStr(ToString(item->GetActorLocation()));
			/*
			if (_bShowVoxelDebugPoints)
			{
				for (auto &itemm : item->voxelArray)
				{
					DrawDebugPoint(
						GetWorld(),
						FVector(itemm.x, itemm.y, itemm.z),
						8,  					//size
						FColor(255 * ((itemm.z) / _debugColorHandle), 0, 255),  //pink
						false,  				//persistent (never goes away)
						-1,
						1
						);
				}
			}
			*/
		}
		//release lock for threads
		_blockArrayCriticalSection.Unlock();
	}
}
//############################################################################################################################



//############################################################################################################################
bool ATerrain::InitializeTerrainBlocks()
{
	for (int z = 0; z < worldSideZ; z++)
	{
		for (int y = 0; y < worldSideY; y++)
		{
			for (int x = 0; x < worldSideX; x++)
			{
				// If block does not exist in coordinates, create it....well obviously
				if (getTerrainBlockFromCoordinates(FVector(TERRAIN_ROOT.X + x * BLOCK_SIZE, TERRAIN_ROOT.Y + y * BLOCK_SIZE, TERRAIN_ROOT.Z + z * BLOCK_SIZE)) == NULL)
				{
					//WorkerTask task;
					//task._taskToBeDone = 0;
					//task._blockRootCoordinates = FVector(TERRAIN_ROOT.X + x * BLOCK_SIZE, TERRAIN_ROOT.Y + y * BLOCK_SIZE, TERRAIN_ROOT.Z + z * BLOCK_SIZE);
					//_terrainWorker->_taskQueue.Enqueue(task);
					FVector blockRootCoordinates = FVector(TERRAIN_ROOT.X + x * BLOCK_SIZE, TERRAIN_ROOT.Y + y * BLOCK_SIZE, TERRAIN_ROOT.Z + z * BLOCK_SIZE);
					CreateBlock(blockRootCoordinates);
				}
			}
		}
	}

	return true;
}
//############################################################################################################################

// Createa block in the overloaded array at the given coordinates! ###########################################################
bool ATerrain::CreateBlock(FVector blockRootCoordinates)
{
	UTerrainSection * createdSection = NewObject<UTerrainSection>(this);

	// Adds blocks into the array[x][y][z]
	_blockArrayCriticalSection.Lock();
		_terrainSectionMap.Add(blockRootCoordinates, createdSection);	// add into the tmap
	_blockArrayCriticalSection.Unlock();

	//InitializeBlockVoxels(spawnedBlock);
	return true;
}
//############################################################################################################################

// Check if a block exists in these coordinates, return the pointer to the block if it does
//############################################################################################################################
UTerrainSection * ATerrain::getTerrainBlockFromCoordinates(FVector blockRootCoordinates)
{
	// Alters the coordinate to match the nearest possible block root coordinate
	blockRootCoordinates = getBlockRootCoordinatesFromCoordinates(blockRootCoordinates);

	// Check if a block with the same coordinates exists in the array already ###################################################
	for (auto &item : _terrainSectionMap)
	{
		if (item.Key.X == blockRootCoordinates.X && item.Key.Y == blockRootCoordinates.Y && item.Key.Z == blockRootCoordinates.Z)
		{
			return item.Value;
		}
	}
	return NULL;
}
//############################################################################################################################

//############################################################################################################################
FVector ATerrain::getBlockRootCoordinatesFromCoordinates(FVector coordinate)
{
	return FVector(TERRAIN_ROOT.X + (floor(coordinate.X / BLOCK_SIZE) * BLOCK_SIZE), TERRAIN_ROOT.Y + (floor(coordinate.Y / BLOCK_SIZE) * BLOCK_SIZE), TERRAIN_ROOT.Z + (floor(coordinate.Z / BLOCK_SIZE) * BLOCK_SIZE));
}
//############################################################################################################################

//############################################################################################################################
int ATerrain::getVoxelHashMapIndexFromRootCoordinates(FVector voxelRootCoordinates)
{
	int hashMapIndex;
	FVector blockRootCoordinates = getBlockRootCoordinatesFromCoordinates(voxelRootCoordinates);

	int x = (voxelRootCoordinates.X - blockRootCoordinates.X) / (CELL_SIZE);
	int y = (voxelRootCoordinates.Y - blockRootCoordinates.Y) / (CELL_SIZE);
	int z = (voxelRootCoordinates.Z - blockRootCoordinates.Z) / (CELL_SIZE);

	hashMapIndex = (z << 16) + (y << 8) + x;

	//PrintStrGreen("Voxel coordinates in block: (" + ToString(x) + ", " + ToString((hashMapIndex >> 8 & 255)) + ", " + ToString((hashMapIndex >> 16 & 255)) + ")");

	return hashMapIndex;
}
//############################################################################################################################

FIntVector ATerrain::getIntVectorFromBlockRootCoordinates(FVector blockRootCoordinates)
{

	int x = (blockRootCoordinates.X - this->GetActorLocation().X) / (BLOCK_SIZE);
	int y = (blockRootCoordinates.Y - this->GetActorLocation().Y) / (BLOCK_SIZE);
	int z = (blockRootCoordinates.Z - this->GetActorLocation().Z) / (BLOCK_SIZE);

	return FIntVector(x,y,z);
}

//############################################################################################################################
FIntVector ATerrain::getIntVectorFromHashMapIndex(int hashMap)
{
	return FIntVector((hashMap & 255), (hashMap >> 8 & 255), (hashMap >> 16 & 255));
}
//############################################################################################################################