#include "spring_api.h"

#include <iterator>
#include <stdlib.h>

#include "SpringUnitType.h"
#include "SpringMap.h"

#include "ExternalAI/Interface/AISCommands.h" // for UNIT_COMMAND_BUILD_NO_FACING

CSpringMap::CSpringMap(springai::OOAICallback* callback, CSpringGame* game)
:	callback(callback),
	game(game),
	metal(NULL)	{

	std::vector<springai::Resource*> resources = callback->GetResources();
	if ( !resources.empty() ) {

		std::vector<springai::Resource*>::iterator i = resources.begin();
		for(;i != resources.end();++i){
			springai::Resource* r = *i;
			std::string name = r->GetName();
			if(name == "Metal"){
				this->metal = r;
				break;
			}
		}
	}

	
	if(metal){
		this->GetMetalSpots();
	}
}

CSpringMap::~CSpringMap(){
	this->metalspots.clear();
	game = NULL;
	callback = NULL;
}

Position CSpringMap::FindClosestBuildSite(IUnitType* t, Position builderPos, double searchRadius, double minimumDistance){
	return this->FindClosestBuildSiteFacing(t,builderPos,searchRadius,minimumDistance,UNIT_COMMAND_BUILD_NO_FACING);
}

Position CSpringMap::FindClosestBuildSiteFacing(IUnitType* t, Position builderPos, double searchRadius, double minimumDistance,int facing){
	if ((t == NULL) || (callback==NULL)){
		return Position(0.0f, 1.0f, 0.0f);
	}
	CSpringUnitType* ut = static_cast<CSpringUnitType*>(t);
	const springai::AIFloat3 bPos(builderPos.x, builderPos.y, builderPos.z);
	const springai::AIFloat3 pos = callback->GetMap()->FindClosestBuildSite(ut->GetUnitDef(), bPos, searchRadius, minimumDistance, facing);
	Position p;
	p.x = pos.x;
	p.y = pos.y;
	p.z = pos.z;
	return p;
}

bool CSpringMap::CanBuildHere(IUnitType* t, Position p){
	return this->CanBuildHereFacing(t,p,UNIT_COMMAND_BUILD_NO_FACING);
}

bool CSpringMap::CanBuildHereFacing(IUnitType* t, Position p, int facing){
	CSpringUnitType* ut = static_cast<CSpringUnitType*>(t);
	const springai::AIFloat3 pos(p.x, p.y, p.z);
	return callback->GetMap()->IsPossibleToBuildAt( ut->GetUnitDef(), pos, facing );
}

int CSpringMap::SpotCount(){
	return metalspots.size();
}

Position CSpringMap::GetSpot(int idx){
	return metalspots[idx];
}

std::vector<Position>& CSpringMap::GetMetalSpots(){
	metal = this->GetMetalResource();
	metalspots.clear();
	std::vector<springai::AIFloat3> positions = callback->GetMap()->GetResourceMapSpotsPositions( metal );
	if ( !positions.empty() ) {
		std::vector<springai::AIFloat3>::iterator j = positions.begin();
		for(;j != positions.end();++j){
			Position p;
			p.x = j->x;
			p.y = j->y;
			p.z = j->z;
			metalspots.push_back(p);
		}
	}
	return metalspots;
}

Position CSpringMap::MapDimensions(){
	
	Position p;
	p.x = callback->GetMap()->GetWidth();
	p.z = callback->GetMap()->GetHeight();
	
	return p;
}

std::string CSpringMap::MapName(){
	return callback->GetMap()->GetName();
}

float CSpringMap::MaximumHeight(){
	return callback->GetMap()->GetMaxHeight();
}

float CSpringMap::MinimumHeight(){
	return callback->GetMap()->GetMinHeight();
}

double CSpringMap::AverageWind(){
	float minwind = callback->GetMap()->GetMinWind();
	float maxwind = callback->GetMap()->GetMaxWind();
	return (minwind+maxwind)/2;
}

double CSpringMap::MinimumWindSpeed(){
	return callback->GetMap()->GetMinWind();
}

double CSpringMap::MaximumWindSpeed(){
	return callback->GetMap()->GetMaxWind();
}

double CSpringMap::TidalStrength(){
	return callback->GetMap()->GetTidalStrength();
}


std::vector<IMapFeature*> CSpringMap::GetMapFeatures(){
	std::vector< IMapFeature*> mapFeatures;
	
	std::vector<springai::Feature*> features = callback->GetFeatures();
	std::vector<springai::Feature*>::iterator i = features.begin();
	for(;i != features.end(); ++i){
		CSpringMapFeature* f = new CSpringMapFeature(callback,*i,game);
		mapFeatures.push_back(f);
	}
	return mapFeatures;
}

std::vector<IMapFeature*> CSpringMap::GetMapFeaturesAt(Position p, double radius){
	const springai::AIFloat3 pos(p.x, p.y, p.z);
	std::vector< IMapFeature*> mapFeatures;
	
	std::vector<springai::Feature*> features = callback->GetFeaturesIn(pos,radius);
	std::vector<springai::Feature*>::iterator i = features.begin();
	for(;i != features.end(); ++i){
		CSpringMapFeature* f = new CSpringMapFeature(callback,*i,game);
		mapFeatures.push_back(f);
	}
	return mapFeatures;
}

springai::Resource* CSpringMap::GetMetalResource(){
	return metal;
}
