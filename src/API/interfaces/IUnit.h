#ifndef IUNIT_H
#define IUNIT_H

#include "ResourceUsage.h"
#include "IMapFeature.h"
#include "IUnitType.h"
#include "../Position.h"

class IUnit {
public:
	virtual int ID()=0;
	virtual std::string Name()=0;

	virtual bool IsAlive()=0;

	virtual bool IsCloaked()=0;

	virtual void Forget()=0; // makes the interface forget about this unit and cleanup
	virtual bool Forgotten()=0; // for interface/debugging use
	
	virtual IUnitType* Type()=0;

	virtual bool CanMove()=0;
	virtual bool CanDeploy()=0;
	virtual bool CanBuild()=0;

	virtual bool CanMorph()=0;
	
	virtual bool CanAssistBuilding(IUnit* unit)=0;

	virtual bool CanMoveWhenDeployed()=0;
	virtual bool CanFireWhenDeployed()=0;
	virtual bool CanBuildWhenDeployed()=0;
	virtual bool CanBuildWhenNotDeployed()=0;

	virtual void Stop()=0;
	virtual void Move(Position p)=0;
	virtual void MoveAndFire(Position p)=0;

	virtual bool Build(IUnitType* t)=0;
	virtual bool Build(std::string typeName)=0;
	virtual bool Build(std::string typeName, Position p)=0;
	virtual bool Build(IUnitType* t, Position p)=0;

	virtual bool AreaReclaim(Position p, double radius)=0;
	virtual bool Reclaim(IMapFeature* mapFeature)=0;
	virtual bool Reclaim(IUnit* unit)=0;
	virtual bool Attack(IUnit* unit)=0;
	virtual bool Repair(IUnit* unit)=0;
	
	virtual Position GetPosition()=0;
	virtual float GetHealth()=0;
	virtual float GetMaxHealth()=0;

	virtual int WeaponCount()=0;
	virtual float MaxWeaponsRange()=0;

	virtual bool CanBuild(IUnitType* t)=0;

	virtual int ResourceUsageCount()=0;
	virtual SResourceUsage GetResourceUsage(int idx)=0;

};

#endif