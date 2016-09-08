// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "TerrainSection.h"
#include "TerrainWorker.h"
#include "ProceduralMeshComponent.h"
class TerrainWorker;

#include "GameFramework/Actor.h"
#include "Terrain.generated.h"
UCLASS()
class PROCEDURAL_TERRAIN_API ATerrain : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATerrain();
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;
	virtual void BeginDestroy() override;
	void DestroyTerrainWorker();

	// EXPERIMENTAL (instead of creating actors that are block who have procedural mesh component components, we create a single terrain mesh that has different mesh sections.
	UProceduralMeshComponent * _terrainMesh;

	// Running integer that we use to give mesh sections their index
	int _runningMeshSectionIndex = 0;
	// All the mesh section indeces that were used but are now free.
	TArray<int> _freedMeshSectionIndeces;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float _debugColorHandle = 2.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool _bShowVoxelDebugPoints;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool _bShowBlockDebugLines;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int worldSideX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int worldSideY;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int worldSideZ;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USphereComponent * _rootSphere;

	// The worker that will work to make, update and change the terrain when changes are made (THREADED)
	TerrainWorker * _terrainWorker;

	// Mutex for blockArray
	FCriticalSection _blockArrayCriticalSection;

	// Testing new complete hashmap array for cubes, hashkey is a 64-bit number where the last 32 are for cube numbers and last 32 for voxel order numbers
	TMap<FVector, UTerrainSection*> _terrainSectionMap;

	// FOR INITIALIZATION ONLY 
	UFUNCTION(BlueprintCallable, category = "TransVoxel Procedural Terrain")
		bool InitializeTerrainSections();

	UFUNCTION(BlueprintCallable, category = "TransVoxel Procedural Terrain")
		UTerrainSection * getTerrainBlockFromCoordinates(FVector blockRootCoordinates);

	UFUNCTION(BlueprintCallable, category = "TransVoxel Procedural Terrain")
		FVector getBlockRootCoordinatesFromCoordinates(FVector coordinate);

	UFUNCTION(BlueprintCallable, category = "TransVoxel Procedural Terrain")
		bool CreateBlock(FVector blockCoordinates);

	UFUNCTION(BlueprintCallable, category = "TransVoxel Procedural Terrain")
		int getVoxelHashMapIndexFromRootCoordinates(FVector voxelRootCoordinates);

	UFUNCTION(BlueprintCallable, category = "TransVoxel Procedural Terrain")
		FIntVector getIntVectorFromHashMapIndex(int hashMap);

	UFUNCTION(BlueprintCallable, category = "TransVoxel Procedural Terrain")
		FIntVector getIntVectorFromBlockRootCoordinates(FVector blockRootCoordinates);
};
