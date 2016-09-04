// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Terrain.h"
class ATerrain;

// Task structure ###################################################################
struct WorkerTask
{
public:
	FVector _blockRootCoordinates;	// The coordinates where a block should exist
	UTerrainSection * _section;	// The pointer of the block to be worked on
	unsigned char _taskToBeDone; // 1 = Create block and initialize, 2 = Update values
};
//###################################################################################

//###################################################################################
class PROCEDURAL_TERRAIN_API TerrainWorker:public FRunnable
{
	/** Thread to run the worker FRunnable on */
	FRunnableThread * _thread;
	/** Stop this thread? Uses Thread Safe Counter */
	FThreadSafeCounter _stopTaskCounter;

public:
	TerrainWorker(ATerrain * terrain, UWorld* const world);
	~TerrainWorker();
	// Begin FRunnable interface.
	virtual bool Init() override;
	bool CreateThread();
	virtual uint32 Run();
	virtual void Stop() override;
	virtual void Exit() override;
	// End FRunnable interface



	// TASK FUNCTIONS!!---------------------------------------------------------------
	bool InitializeBlock(WorkerTask task);
	//--------------------------------------------------------------------------------
	FVector getBlockRootCoordinatesFromCoordinates(FVector coordinates);




	//Done?
	bool IsFinished() const { return false; }
	void Shutdown();
	void EnsureCompletion();
	bool IsThreadFinished();
	// The terrain that the worker belongs to
	ATerrain * _terrain;
	// Pointer to the world
	UWorld* _world;
	// The queue for tasks to be done
	TQueue<WorkerTask> _taskQueue;
};
//###################################################################################