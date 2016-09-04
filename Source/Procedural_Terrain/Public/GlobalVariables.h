// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//#define WORLD_SIZE_IN_BLOCKS_X 1	//in chunks (50 = ~4GB)
//#define WORLD_SIZE_IN_BLOCKS_Y 2	//in chunks (50 = ~4GB)
//#define WORLD_SIZE_IN_BLOCKS_Z 2	//in chunks (10 = ~4GB)
#define VOXELS_PER_CELL 8	//never really changes
#define BLOCK_CELL_SIDE_COUNT 8	//How many^3 cell does a block have?
#define CELL_SIZE 20.0f	//cell's world size in Unreal units 100 = 1m = 100cm
#define BLOCK_SIZE (CELL_SIZE*BLOCK_CELL_SIDE_COUNT)
#define TERRAIN_ROOT GetActorLocation()

#define PI 3.14159265358979323846
#define TEXT_DURATION 15

class PROCEDURAL_TERRAIN_API GlobalVariables
{
public:
	GlobalVariables();
	~GlobalVariables();
};
