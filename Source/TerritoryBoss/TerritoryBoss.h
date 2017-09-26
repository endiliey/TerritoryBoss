// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#ifndef __TERRITORYBOSS_H__
#define __TERRITORYBOSS_H__

#include "Engine.h"

// This is NOT included by default in an empty project! It's required for replication and setting of the GetLifetimeReplicatedProps
#include "Net/UnrealNetwork.h"

/*DefaultEngine.ini [/Script/Engine.CollisionProfile]*/
#define COLLISION_WEAPON			ECC_GameTraceChannel1

/* DefaultEngine.ini[/Script/Engine.PhysicsSettings]*/
#define SURFACE_DEFAULT				SurfaceType_Default
#define SURFACE_FLESH				SurfaceType1
#define SURFACE_CONCRETE			SurfaceType2
#endif
