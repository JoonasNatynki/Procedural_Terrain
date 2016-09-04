// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <typeinfo>
#include <string>

/**
*
*/

#include <iostream>
#include <sstream>
#include <string>
#include <stdexcept>

#include "GlobalVariables.h"
//#include "Spiral.h"








class BadConversion : public std::runtime_error {
public:
	BadConversion(std::string const& s)
		: std::runtime_error(s)
	{ }
};

inline std::string ToString(double x)
{
	std::ostringstream o;
	if (!(o << x))
		throw BadConversion("ToString(double)");
	return o.str();
}

inline std::string ToString(float x)
{
	std::ostringstream o;
	if (!(o << x))
		throw BadConversion("ToString(float)");
	return o.str();
}

inline std::string ToString(int x)
{
	std::ostringstream o;
	if (!(o << x))
		throw BadConversion("ToString(int)");
	return o.str();
}

inline std::string ToString(short x)
{
	std::ostringstream o;
	if (!(o << x))
		throw BadConversion("ToString(short)");
	return o.str();
}

inline std::string ToString(long x)
{
	std::ostringstream o;
	if (!(o << x))
		throw BadConversion("ToString(long)");
	return o.str();
}

inline std::string ToString(bool x)
{
	if (x)
	{
		return "true";
	}
	else
	{
		return "false";
	}
}

inline FString ToFString(std::string x)
{
	FString f(x.c_str());
	return f;
}

inline void PrintFStr(FString text)
{
	GEngine->AddOnScreenDebugMessage(-1, TEXT_DURATION, FColor::White, text, true);
}

inline void PrintFStrRed(FString text)
{
	GEngine->AddOnScreenDebugMessage(-1, TEXT_DURATION, FColor::Red, text, false);
}

inline void PrintFStrGreen(FString text)
{
	GEngine->AddOnScreenDebugMessage(-1, TEXT_DURATION, FColor::Green, text, true);
}

inline void PrintFStrBlue(FString text)
{
	GEngine->AddOnScreenDebugMessage(-1, TEXT_DURATION, FColor::Blue, text, true);
}

inline void PrintStr(std::string text)
{
	GEngine->AddOnScreenDebugMessage(-1, TEXT_DURATION, FColor::White, ToFString(text), true);
}

inline void PrintStrRed(std::string text)
{
	GEngine->AddOnScreenDebugMessage(-1, TEXT_DURATION, FColor::Red, ToFString(text), true);
}

inline void PrintStrGreen(std::string text)
{
	GEngine->AddOnScreenDebugMessage(-1, TEXT_DURATION, FColor::Green, ToFString(text), true);
}

inline void PrintStrBlue(std::string text)
{
	GEngine->AddOnScreenDebugMessage(-1, TEXT_DURATION, FColor::Blue, ToFString(text), true);
}

inline std::string ToString(FVector vector)
{
	std::string text = ToString(vector.X) + ", " + ToString(vector.Y) + ", " + ToString(vector.Z);
	std::ostringstream o;
	if (!(o << text))
	{
		throw BadConversion("ToString(FVector)");
	}
	return o.str();
}



inline std::string ToString(FIntVector intVector)
{
	std::string text = ToString(intVector.X) + ", " + ToString(intVector.Y) + ", " + ToString(intVector.Z);
	std::ostringstream o;
	if (!(o << text))
	{
		throw BadConversion("ToString(FIntVector)");
	}
	return o.str();
}

class PROCEDURAL_TERRAIN_API HelperFunctions
{
public:
	HelperFunctions();
	~HelperFunctions();
};

// How to create a component at runtime
/*
//DEBUG******
USphereComponent * sphere = CreateDefaultSubobject<USphereComponent>(TEXT("RootComponent"));
//sphere->SetSprite(debugBlockRootSpriteTexture);
sphere->RegisterComponent();
sphere->SetSphereRadius(CELL_SIZE);
sphere->SetWorldLocation(blockWorldRootCoordinates);
sphere->SetWorldRotation(this->GetActorRotation());
sphere->AttachTo(GetRootComponent());
sphere->SetHiddenInGame(false);
*/