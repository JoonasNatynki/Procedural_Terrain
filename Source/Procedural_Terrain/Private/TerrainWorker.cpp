// Fill out your copyright notice in the Description page of Project Settings.

#include "Procedural_Terrain.h"
#include "TerrainWorker.h"

#include "HelperFunctions.h"

//############################################################################################################################
TerrainWorker::TerrainWorker(ATerrain * terrain, UWorld * world)
{
	_terrain = terrain;
	_world = world;
}
//############################################################################################################################

//############################################################################################################################
TerrainWorker::~TerrainWorker()
{
	delete _thread;
	_thread = NULL;
}
//############################################################################################################################

//############################################################################################################################
void TerrainWorker::EnsureCompletion()
{
	//Stop();
	//incremeents the thread safe counter so that the Run() will not iterate again in the while loops
	_stopTaskCounter.Increment();
	// Halts the caller until _thread destruction is finished and returns back to caller's scope
	_thread->WaitForCompletion();
}
//############################################################################################################################

//############################################################################################################################
//stop
void TerrainWorker::Stop()
{
	_stopTaskCounter.Increment();
}
//############################################################################################################################

//############################################################################################################################
void TerrainWorker::Shutdown()
{
	if (!_thread)
	{
		return;
	}

	_thread->Kill(true);
}
//############################################################################################################################

//############################################################################################################################
bool TerrainWorker::CreateThread()
{
	if (FPlatformProcess::SupportsMultithreading())
	{
		// First goes to Init() then Run()
		_thread = FRunnableThread::Create(this, TEXT("TerrainWorkerAntThread"), 0, TPri_BelowNormal); //windows default = 8mb for thread, could specify more
		return true;
	}
	else
	{
		return false;
	}
}
//############################################################################################################################

//############################################################################################################################
void TerrainWorker::Exit()
{

}
//############################################################################################################################

//############################################################################################################################
bool TerrainWorker::IsThreadFinished()
{
	return true;
}
//############################################################################################################################










//############################################################################################################################
//Run
uint32 TerrainWorker::Run()
{
	//Initial wait before starting
	FPlatformProcess::Sleep(0.03);

	PrintStrRed("THREAD STARTED!!");

	while (_stopTaskCounter.GetValue() == 0 && !IsFinished())
	{
		//DO THE CALCULATION SHIT HERE
		if (!_taskQueue.IsEmpty())
		{
			WorkerTask task;
			_taskQueue.Dequeue(task);

			switch (task._taskToBeDone)
				case 0:	InitializeBlock(task);
		}
	}

	//Run FPrimeNumberWorker::Shutdown() from the timer in Game Thread that is watching
	//to see when FPrimeNumberWorker::IsThreadFinished()

	return 0;
}
//############################################################################################################################

//############################################################################################################################
//Init
bool TerrainWorker::Init()
{
	//PUT RANDOM INIT SHIT HERE IF YOU NEED
	PrintStrGreen("THREAD INITIATED!!");

	return true;
}
//############################################################################################################################

// TASK FUNCTIONS! ###########################################################################################################
// Createa block in the overloaded array at the given coordinates!
bool TerrainWorker::InitializeBlock(WorkerTask task)
{
	return true;
}
//############################################################################################################################

//############################################################################################################################
FVector TerrainWorker::getBlockRootCoordinatesFromCoordinates(FVector coordinate)
{
	return FVector(_terrain->TERRAIN_ROOT.X + (floor(coordinate.X / BLOCK_SIZE) * BLOCK_SIZE), _terrain->TERRAIN_ROOT.Y + (floor(coordinate.Y / BLOCK_SIZE) * BLOCK_SIZE), _terrain->TERRAIN_ROOT.Z + (floor(coordinate.Z / BLOCK_SIZE) * BLOCK_SIZE));
}
//############################################################################################################################
