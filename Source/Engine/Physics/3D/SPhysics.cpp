// Copyright 2015-2019 Piperift - All rights reserved

#include "SPhysics.h"
#include <common/PxTolerancesScale.h>
#include <extensions/PxRigidActorExt.h>
#include <foundation/PxFlags.h>
#include <geometry/PxBoxGeometry.h>
#include <geometry/PxSphereGeometry.h>
#include <PxSceneDesc.h>
#include <PxFiltering.h>
#include <PxMaterial.h>

#include "Core/Engine.h"
#include "World.h"
#include "Tools/Profiler.h"

#include "Gameplay/Components/CTransform.h"
#include "Components/CBody.h"

#include "../PhysicsTypes.h"
#include "Gameplay/Singletons/CPhysicsWorld.h"
#include "Components/CVehicle.h"


using namespace physx;


void UserErrorCallback::reportError(physx::PxErrorCode::Enum code, const char* message, const char* file, int line)
{
	Log::Error(TX("PhysX error(%i): %s at %s:%i"), (i32)code, message, file, line);
}

void SimulationCallback::onTrigger(PxTriggerPair* pairs, PxU32 count)
{
	if (!physics->physicsWorld)
		return;

	for (PxU32 i = 0; i < count; i++)
	{
		PxTriggerPair& pair = pairs[i];
		// Ignore pairs when shapes have been deleted
		if (pair.flags & (PxTriggerPairFlag::eREMOVED_SHAPE_TRIGGER | PxTriggerPairFlag::eREMOVED_SHAPE_OTHER))
			continue;

		physics->physicsWorld->triggerEvents.Add(TriggerEvent{
			SPhysics::GetActorEntity(pair.triggerActor),
			SPhysics::GetActorEntity(pair.otherActor)
		});
	}
}


PxFilterFlags CollisionFilterShader(
	PxFilterObjectAttributes attributes0, PxFilterData filterData0,
	PxFilterObjectAttributes attributes1, PxFilterData filterData1,
	PxPairFlags& pairFlags, const void* constantBlock, PxU32 constantBlockSize)
{
	// let triggers through
	if (PxFilterObjectIsTrigger(attributes0) || PxFilterObjectIsTrigger(attributes1))
	{
		pairFlags = PxPairFlag::eTRIGGER_DEFAULT;
		return PxFilterFlag::eDEFAULT;
	}
	// generate contacts for all that were not filtered above
	pairFlags = PxPairFlag::eCONTACT_DEFAULT;

	// trigger the contact callback for pairs (A,B) where
	// the filtermask of A contains the ID of B and vice versa.
	if ((filterData0.word0 & filterData1.word1) && (filterData1.word0 & filterData0.word1))
		pairFlags |= PxPairFlag::eNOTIFY_TOUCH_FOUND;

	return PxFilterFlag::eDEFAULT;
}


SPhysics::SPhysics()
	: Super()
	, simulationCallback{this}
	, physicsMTFlow{ GEngine->Tasks().CreateFlow() }
{
	foundation = PxCreateFoundation(PX_PHYSICS_VERSION, *Memory::GetPhysicsAllocator(), pxErrorCallback);
	if (!foundation)
	{
		Log::Error("PhysX error: PxCreateFoundation failed");
		return;
	}

	physx::PxTolerancesScale scale;
	world = PxCreatePhysics(PX_PHYSICS_VERSION, *foundation, {});
	if (!world)
	{
		Log::Error("PhysX error: PxCreatePhysics failed");
		return;
	}

	InitVehicles();
}

void SPhysics::BeginPlay()
{
	Super::BeginPlay();

	physicsWorld = ECS()->FindSingleton<CPhysicsWorld>();
	CreateScene();
}

void SPhysics::Tick(float deltaTime)
{
	ScopedStackGameZone();
	Super::Tick(deltaTime);

	if (physicsWorld)
	{
		physicsWorld->ResetEvents();
	}

	CreateBodies();

	// Simulate at a fixed rate
	deltaTimeIncrement += deltaTime;
	if (deltaTimeIncrement >= stepSize)
	{
		deltaTimeIncrement -= stepSize;

		UploadBodies();
		Step(stepSize);
		DownloadBodies();
	}
}

void SPhysics::EndPlay()
{
	scene->release();
	scene = nullptr;
	Super::EndPlay();
}

void SPhysics::BeforeDestroy()
{
	world->release();
	world = nullptr;
	foundation->release();
	foundation = nullptr;
	Super::BeforeDestroy();
}

void SPhysics::CreateBodies()
{
	auto view = ECS()->View<CTransform, CBody>();

	// Initialize invalid bodies
	for (EntityId entity : view)
	{
		CBody& body = view.get<CBody>(entity);
		if (!body.IsInitialized())
		{
			CreateBody(entity, view.get<CTransform>(entity), body);
		}
	}
}

void SPhysics::UploadBodies()
{
	ScopedGameZone("Upload Bodies");

	auto view = ECS()->View<CTransform, CBody>();

	// Upload valid bodies to physics (Splitted between all threads)
	physicsMTFlow.parallel_for(view.begin(), view.end(), [&view](EntityId entity)
	{
		ScopedGameZone("Upload Body");

		const CBody& body = view.get<CBody>(entity);
		if (body.IsInitialized() && !body.IsStatic())
		{
			const CTransform& transform = view.get<CTransform>(entity);

			const v3& location = transform.GetWLocation();
			const Quat& rotation = transform.GetWRotation();
			const auto currTransform = body.rigidBody->getGlobalPose();

			// If position or rotation changed, upload it
			if (location.DistanceSqrt(FromPx(currTransform.p)) > Math::SMALL_NUMBER ||
				!rotation.Equals(FromPx(currTransform.q)))
			{
				const physx::PxTransform newTransform{ ToPx(location), ToPx(rotation) };
				body.rigidBody->setGlobalPose(newTransform);
			}
		}
	});
	physicsMTFlow.wait_for_all();
}

void SPhysics::Step(float deltaTime)
{
	ScopedGameZone("Step");

	scene->simulate(deltaTime);
	// #TODO: Support multi-threading while doing Render tick
	scene->fetchResults(true);
}

void SPhysics::DownloadBodies()
{
	ScopedGameZone("Download Bodies");

	auto view = ECS()->View<CTransform, CBody>();

	// Download valid bodies from physics (Splitted between all threads)
	physicsMTFlow.parallel_for(view.begin(), view.end(), [&view](EntityId entity)
	{
		ScopedGameZone("Download Body");

		const CBody& body = view.get<CBody>(entity);
		if (body.IsInitialized() && !body.IsStatic())
		{
			CTransform& transform = view.get<CTransform>(entity);

			transform.SetWLocation(FromPx(body.rigidBody->getGlobalPose().p));
			transform.SetWRotation(FromPx(body.rigidBody->getGlobalPose().q));
		}
	});
	physicsMTFlow.wait_for_all();
}

void SPhysics::CreateScene()
{
	physx::PxSceneDesc sceneDesc(world->getTolerancesScale());
	sceneDesc.gravity = ToPx(physicsWorld->gravity);

	if (!sceneDesc.cpuDispatcher)
	{
		// Number of desired threads
		sceneDesc.cpuDispatcher = physx::PxDefaultCpuDispatcherCreate(1);
		if (!sceneDesc.cpuDispatcher)
		{
			Log::Error("PhysX error: PxDefaultCpuDispatcherCreate failed!");
		}
	}

	if (!sceneDesc.filterShader)
		sceneDesc.filterShader = physx::PxDefaultSimulationFilterShader;

	//sceneDesc.frictionType = PxFrictionType::eTWO_DIRECTIONAL;
	//sceneDesc.frictionType = PxFrictionType::eONE_DIRECTIONAL;
	//sceneDesc.flags |= PxSceneFlag::eENABLE_GPU_DYNAMICS;
	sceneDesc.flags |= physx::PxSceneFlag::eENABLE_PCM;
	//sceneDesc.flags |= PxSceneFlag::eENABLE_AVERAGE_POINT;
	sceneDesc.flags |= physx::PxSceneFlag::eENABLE_STABILIZATION;
	//sceneDesc.flags |= PxSceneFlag::eADAPTIVE_FORCE;
	sceneDesc.flags |= physx::PxSceneFlag::eENABLE_ACTIVE_ACTORS;
	sceneDesc.sceneQueryUpdateMode = physx::PxSceneQueryUpdateMode::eBUILD_ENABLED_COMMIT_DISABLED;

	//sceneDesc.flags |= PxSceneFlag::eDISABLE_CONTACT_CACHE;
	//sceneDesc.broadPhaseType =  PxBroadPhaseType::eGPU;
	//sceneDesc.broadPhaseType = PxBroadPhaseType::eSAP;
	sceneDesc.gpuMaxNumPartitions = 8;

	//sceneDesc.solverType = PxSolverType::eTGS;

#ifdef USE_MBP
	sceneDesc.broadPhaseType = PxBroadPhaseType::eMBP;
#endif

	sceneDesc.simulationEventCallback = &simulationCallback;
	sceneDesc.filterShader = CollisionFilterShader;

	scene = world->createScene(sceneDesc);
	if (!scene)
	{
		Log::Error("PhysX error: CreateScene failed!");
	}
}

void SPhysics::CreateBody(EntityId entity, const CTransform& transform, CBody& body)
{
	physx::PxTransform t{ ToPx(transform.GetWLocation()), ToPx(transform.GetWRotation()) };

	switch (EMobilityType(body.mobility))
	{
	case EMobilityType::Movable:
		body.rigidBody = world->createRigidDynamic(t);
		break;
	case EMobilityType::Kinematic: {
		physx::PxRigidBody* rigidBody = world->createRigidDynamic(t);
		rigidBody->setRigidBodyFlag(physx::PxRigidBodyFlag::eKINEMATIC, true);
		body.rigidBody = rigidBody;
		break;
	}
	case EMobilityType::Static:
		body.rigidBody = world->createRigidStatic(t);
		break;
	}

	SetupBodyShapes(body);
	SetActorEntity(body.rigidBody, entity);
	scene->addActor(*body.rigidBody);
}

void SPhysics::SetupBodyShapes(CBody& body)
{
	physx::PxMaterial* material = world->createMaterial(0.5f, 0.5f, 0.1f);

	if (body.bHasBoxShape)
	{
		physx::PxShape* shape = physx::PxRigidActorExt::createExclusiveShape(
			*body.rigidBody,
			physx::PxBoxGeometry(ToPx(body.boxExtent)),
			*material
		);

		if (body.bIsTrigger)
		{
			shape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, false);
			shape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, true);
		}
	}

	if (body.bHasSphereShape)
	{
		physx::PxShape* shape = physx::PxRigidActorExt::createExclusiveShape(*body.rigidBody, physx::PxSphereGeometry(body.radius), *material);

		if (body.bIsTrigger)
		{
			shape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, false);
			shape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, true);
		}
	}
}

void SPhysics::InitVehicles()
{
	PxInitVehicleSDK(*world);
	PxVehicleSetBasisVectors(ToPx(v3::Up), ToPx(v3::Forward));
	PxVehicleSetUpdateMode(PxVehicleUpdateMode::eACCELERATION);
}

void SPhysics::UpdateVehicles()
{

}

void SPhysics::CreateVehicle(CVehicle& vehicle)
{
	/*const PxU32 numWheels = 4;

	PxVehicleWheelsSimData* wheelsSimData = PxVehicleWheelsSimData::allocate(numWheels);
	PxVehicleDriveSimData4W driveData4W;
	PxVehicleChassisData chassisData4W;
	createVehicle4WSimulationData(
		chassis.mass, chassisConvexMesh,
		20.0f, wheelConvexMeshes4, wheelCentreOffsets4,
		*wheelsSimData4W, driveData4W, chassisData4W);


	PxVehicleDriveSimData4W driveSimData;
	setupDriveSimData(driveSimData);

	PxRigidDynamic* vehActor = world->createRigidDynamic(startPose);
	setupVehicleActor(vehActor);
	scene->addActor(*vehActor);

	PxVehicleDrive4W* vehDrive4W = PxVehicleDrive4W::allocate(numWheels);
	vehDrive4W->setup(physics, veh4WActor, *wheelsSimData, driveSimData, numWheels - 4);
	wheelsSimData->free();*/
}

void SPhysics::ShutdownVehicles()
{
	PxCloseVehicleSDK();
}

/*void createVehicle4WSimulationData(VehicleChassisSettings chassis, VehicleWheelSettings wheel,
	PxVehicleWheelsSimData& wheelsData, PxVehicleDriveSimData4W& driveData, PxVehicleChassisData& chassisData)
{
	//The origin is at the center of the chassis mesh.
	//Set the center of mass to be below this point and a little towards the front.
	const PxVec3 chassisCMOffset = PxVec3(0.0f, -chassis.size.y * 0.5f + 0.65f, 0.25f);

	//Now compute the chassis mass and moment of inertia.
	//Use the moment of inertia of a cuboid as an approximate value for the chassis moi.
	PxVec3 chassisMOI {
		(chassis.size.y * chassis.size.y + chassis.size.z * chassis.size.z) * chassis.mass / 12.0f,
		(chassis.size.x * chassis.size.x + chassis.size.z * chassis.size.z) * chassis.mass / 12.0f,
		(chassis.size.x * chassis.size.x + chassis.size.y * chassis.size.y) * chassis.mass / 12.0f
	};
	//A bit of tweaking here.  The car will have more responsive turning if we reduce the
	//y-component of the chassis moment of inertia.
	chassisMOI.y *= 0.8f;

	//Let's set up the chassis data structure now.
	chassisData.mMass = chassis.mass;
	chassisData.mMOI = chassisMOI;
	chassisData.mCMOffset = chassisCMOffset;

	//Compute the sprung masses of each suspension spring using a helper function.
	PxF32 suspSprungMasses[4];
	PxVehicleComputeSprungMasses(4, wheelCentreOffsets, chassisCMOffset, chassis.mass, 1, suspSprungMasses);

	//Extract the wheel radius and width from the wheel convex meshes.
	PxF32 wheelWidths[4];
	PxF32 wheelRadii[4];
	computeWheelWidthsAndRadii(wheelConvexMeshes, wheelWidths, wheelRadii);

	//Now compute the wheel masses and inertias components around the axle's axis.
	//http://en.wikipedia.org/wiki/List_of_moments_of_inertia
	PxF32 wheelMOIs[4];
	for (PxU32 i = 0; i < 4; i++)
	{
		wheelMOIs[i] = 0.5f * wheel.mass * wheelRadii[i] * wheelRadii[i];
	}
	//Let's set up the wheel data structures now with radius, mass, and moi.
	PxVehicleWheelData wheels[4];
	for (PxU32 i = 0; i < 4; i++)
	{
		wheels[i].mRadius = wheelRadii[i];
		wheels[i].mMass = wheel.mass;
		wheels[i].mMOI = wheelMOIs[i];
		wheels[i].mWidth = wheelWidths[i];
	}
	//Disable the handbrake from the front wheels and enable for the rear wheels
	wheels[PxVehicleDrive4WWheelOrder::eFRONT_LEFT].mMaxHandBrakeTorque = 0.0f;
	wheels[PxVehicleDrive4WWheelOrder::eFRONT_RIGHT].mMaxHandBrakeTorque = 0.0f;
	wheels[PxVehicleDrive4WWheelOrder::eREAR_LEFT].mMaxHandBrakeTorque = 4000.0f;
	wheels[PxVehicleDrive4WWheelOrder::eREAR_RIGHT].mMaxHandBrakeTorque = 4000.0f;
	//Enable steering for the front wheels and disable for the front wheels.
	wheels[PxVehicleDrive4WWheelOrder::eFRONT_LEFT].mMaxSteer = PxPi * 0.3333f;
	wheels[PxVehicleDrive4WWheelOrder::eFRONT_RIGHT].mMaxSteer = PxPi * 0.3333f;
	wheels[PxVehicleDrive4WWheelOrder::eREAR_LEFT].mMaxSteer = 0.0f;
	wheels[PxVehicleDrive4WWheelOrder::eREAR_RIGHT].mMaxSteer = 0.0f;

	//Let's set up the tire data structures now.
	//Put slicks on the front tires and wets on the rear tires.
	PxVehicleTireData tires[4];
	tires[PxVehicleDrive4WWheelOrder::eFRONT_LEFT].mType = TIRE_TYPE_SLICKS;
	tires[PxVehicleDrive4WWheelOrder::eFRONT_RIGHT].mType = TIRE_TYPE_SLICKS;
	tires[PxVehicleDrive4WWheelOrder::eREAR_LEFT].mType = TIRE_TYPE_WETS;
	tires[PxVehicleDrive4WWheelOrder::eREAR_RIGHT].mType = TIRE_TYPE_WETS;

	//Let's set up the suspension data structures now.
	PxVehicleSuspensionData susps[4];
	for (PxU32 i = 0; i < 4; i++)
	{
		susps[i].mMaxCompression = 0.3f;
		susps[i].mMaxDroop = 0.1f;
		susps[i].mSpringStrength = 35000.0f;
		susps[i].mSpringDamperRate = 4500.0f;
	}
	susps[PxVehicleDrive4WWheelOrder::eFRONT_LEFT].mSprungMass = suspSprungMasses[PxVehicleDrive4WWheelOrder::eFRONT_LEFT];
	susps[PxVehicleDrive4WWheelOrder::eFRONT_RIGHT].mSprungMass = suspSprungMasses[PxVehicleDrive4WWheelOrder::eFRONT_RIGHT];
	susps[PxVehicleDrive4WWheelOrder::eREAR_LEFT].mSprungMass = suspSprungMasses[PxVehicleDrive4WWheelOrder::eREAR_LEFT];
	susps[PxVehicleDrive4WWheelOrder::eREAR_RIGHT].mSprungMass = suspSprungMasses[PxVehicleDrive4WWheelOrder::eREAR_RIGHT];

	//Set up the camber.
	//Remember that the left and right wheels need opposite camber so that the car preserves symmetry about the forward direction.
	//Set the camber to 0.0f when the spring is neither compressed or elongated.
	const PxF32 camberAngleAtRest = 0.0;
	susps[PxVehicleDrive4WWheelOrder::eFRONT_LEFT].mCamberAtRest = camberAngleAtRest;
	susps[PxVehicleDrive4WWheelOrder::eFRONT_RIGHT].mCamberAtRest = -camberAngleAtRest;
	susps[PxVehicleDrive4WWheelOrder::eREAR_LEFT].mCamberAtRest = camberAngleAtRest;
	susps[PxVehicleDrive4WWheelOrder::eREAR_RIGHT].mCamberAtRest = -camberAngleAtRest;
	//Set the wheels to camber inwards at maximum droop (the left and right wheels almost form a V shape)
	const PxF32 camberAngleAtMaxDroop = 0.001f;
	susps[PxVehicleDrive4WWheelOrder::eFRONT_LEFT].mCamberAtMaxDroop = camberAngleAtMaxDroop;
	susps[PxVehicleDrive4WWheelOrder::eFRONT_RIGHT].mCamberAtMaxDroop = -camberAngleAtMaxDroop;
	susps[PxVehicleDrive4WWheelOrder::eREAR_LEFT].mCamberAtMaxDroop = camberAngleAtMaxDroop;
	susps[PxVehicleDrive4WWheelOrder::eREAR_RIGHT].mCamberAtMaxDroop = -camberAngleAtMaxDroop;
	//Set the wheels to camber outwards at maximum compression (the left and right wheels almost form a A shape).
	const PxF32 camberAngleAtMaxCompression = -0.001f;
	susps[PxVehicleDrive4WWheelOrder::eFRONT_LEFT].mCamberAtMaxCompression = camberAngleAtMaxCompression;
	susps[PxVehicleDrive4WWheelOrder::eFRONT_RIGHT].mCamberAtMaxCompression = -camberAngleAtMaxCompression;
	susps[PxVehicleDrive4WWheelOrder::eREAR_LEFT].mCamberAtMaxCompression = camberAngleAtMaxCompression;
	susps[PxVehicleDrive4WWheelOrder::eREAR_RIGHT].mCamberAtMaxCompression = -camberAngleAtMaxCompression;

	//We need to set up geometry data for the suspension, wheels, and tires.
	//We already know the wheel centers described as offsets from the actor center and the center of mass offset from actor center.
	//From here we can approximate application points for the tire and suspension forces.
	//Lets assume that the suspension travel directions are absolutely vertical.
	//Also assume that we apply the tire and suspension forces 30cm below the center of mass.
	PxVec3 suspTravelDirections[4] = { PxVec3(0,-1,0),PxVec3(0,-1,0),PxVec3(0,-1,0),PxVec3(0,-1,0) };
	PxVec3 wheelCentreCMOffsets[4];
	PxVec3 suspForceAppCMOffsets[4];
	PxVec3 tireForceAppCMOffsets[4];
	for (PxU32 i = 0; i < 4; i++)
	{
		wheelCentreCMOffsets[i] = wheelCentreOffsets[i] - chassisCMOffset;
		suspForceAppCMOffsets[i] = PxVec3(wheelCentreCMOffsets[i].x, -0.3f, wheelCentreCMOffsets[i].z);
		tireForceAppCMOffsets[i] = PxVec3(wheelCentreCMOffsets[i].x, -0.3f, wheelCentreCMOffsets[i].z);
	}

	//Now add the wheel, tire and suspension data.
	for (PxU32 i = 0; i < 4; i++)
	{
		wheelsData.setWheelData(i, wheels[i]);
		wheelsData.setTireData(i, tires[i]);
		wheelsData.setSuspensionData(i, susps[i]);
		wheelsData.setSuspTravelDirection(i, suspTravelDirections[i]);
		wheelsData.setWheelCentreOffset(i, wheelCentreCMOffsets[i]);
		wheelsData.setSuspForceAppPointOffset(i, suspForceAppCMOffsets[i]);
		wheelsData.setTireForceAppPointOffset(i, tireForceAppCMOffsets[i]);
	}

	//Set the car to perform 3 sub-steps when it moves with a forwards speed of less than 5.0
	//and with a single step when it moves at speed greater than or equal to 5.0.
	wheelsData.setSubStepCount(5.0f, 3, 1);


	//Now set up the differential, engine, gears, clutch, and ackermann steering.

	//Diff
	PxVehicleDifferential4WData diff;
	diff.mType = PxVehicleDifferential4WData::eDIFF_TYPE_LS_4WD;
	driveData.setDiffData(diff);

	//Engine
	PxVehicleEngineData engine;
	engine.mPeakTorque = 500.0f;
	engine.mMaxOmega = 600.0f;//approx 6000 rpm
	driveData.setEngineData(engine);

	//Gears
	PxVehicleGearsData gears;
	gears.mSwitchTime = 0.5f;
	driveData.setGearsData(gears);

	//Clutch
	PxVehicleClutchData clutch;
	clutch.mStrength = 10.0f;
	driveData.setClutchData(clutch);

	//Ackermann steer accuracy
	PxVehicleAckermannGeometryData ackermann;
	ackermann.mAccuracy = 1.0f;
	ackermann.mAxleSeparation = wheelCentreOffsets[PxVehicleDrive4WWheelOrder::eFRONT_LEFT].z - wheelCentreOffsets[PxVehicleDrive4WWheelOrder::eREAR_LEFT].z;
	ackermann.mFrontWidth = wheelCentreOffsets[PxVehicleDrive4WWheelOrder::eFRONT_RIGHT].x - wheelCentreOffsets[PxVehicleDrive4WWheelOrder::eFRONT_LEFT].x;
	ackermann.mRearWidth = wheelCentreOffsets[PxVehicleDrive4WWheelOrder::eREAR_RIGHT].x - wheelCentreOffsets[PxVehicleDrive4WWheelOrder::eREAR_LEFT].x;
	driveData.setAckermannGeometryData(ackermann);
}*/
