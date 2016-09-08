// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Object.h"
#include "TerrainSection.generated.h"

/**
 * 
 */
UCLASS()
class PROCEDURAL_TERRAIN_API UTerrainSection : public UObject
{
	GENERATED_BODY()

public:
	// The index of the mesh's section id in the terrain's mesh
	int _meshSectionIndex;
		
};
