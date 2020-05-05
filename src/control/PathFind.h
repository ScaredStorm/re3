#pragma once

#include "Treadable.h"

class CVehicle;
class CPtrList;

enum
{
	NodeTypeExtern = 1,
	NodeTypeIntern = 2,

	UseInRoadBlock = 1,
	ObjectEastWest = 2,
};

enum
{
	PATH_CAR = 0,
	PATH_PED = 1,
};

enum
{
	SWITCH_OFF = 0,
	SWITCH_ON = 1,
};

enum 
{
	ROUTE_ADD_BLOCKADE = 0,
	ROUTE_NO_BLOCKADE = 1
};

struct CPedPathNode
{
	bool bBlockade;
	uint8 nodeIdX;
	uint8 nodeIdY;
	int16 id;
	CPedPathNode* prev;
	CPedPathNode* next;
};
static_assert(sizeof(CPedPathNode) == 0x10, "CPedPathNode: error");

class CPedPath {
public:
	static bool CalcPedRoute(int8 pathType, CVector position, CVector destination, CVector *pointPoses, int16 *pointsFound, int16 maxPoints);
	static void AddNodeToPathList(CPedPathNode *pNodeToAdd, int16 id, CPedPathNode *pNodeList);
	static void RemoveNodeFromList(CPedPathNode *pNode);
	static void AddNodeToList(CPedPathNode *pNode, int16 index, CPedPathNode *pList);
	static void AddBlockade(CEntity *pEntity, CPedPathNode(*pathNodes)[40], CVector *pPosition);
	static void AddBlockadeSectorList(CPtrList& list, CPedPathNode(*pathNodes)[40], CVector *pPosition);
};

struct CPathNode
{
#ifndef MIAMI
	CVector pos;
	CPathNode *prev;
	CPathNode *next;
	int16 distance;		// in path search
	int16 objectIndex;
	int16 firstLink;
	uint8 numLinks;

	uint8 unkBits : 2;
	uint8 bDeadEnd : 1;
	uint8 bDisabled : 1;
	uint8 bBetweenLevels : 1;

	int8 group;

	CVector &GetPosition(void) { return pos; }
	void SetPosition(const CVector &p) { pos = p; }
	float GetX(void) { return pos.x; }
	float GetY(void) { return pos.y; }
	float GetZ(void) { return pos.z; }

	CPathNode *GetPrev(void) { return prev; }
	CPathNode *GetNext(void) { return next; }
	void SetPrev(CPathNode *node) { prev = node; }
	void SetNext(CPathNode *node) { next = node; }
#else
	int16 prevIndex;
	int16 nextIndex;
	int16 x;
	int16 y;
	int16 z;
	int16 distance;		// in path search
	int16 firstLink;
	int8 width;
	int8 group;

	uint8 numLinks : 4;
	uint8 bDeadEnd : 1;
	uint8 bDisabled : 1;
	uint8 bBetweenLevels : 1;
	uint8 bUseInRoadBlock : 1;

	uint8 bWaterPath : 1;
	uint8 flagB2 : 1;	// flag 2 in node info, always zero
	uint8 flagB4 : 1;	// where is this set?
	uint8 speedLimit : 2;
	//uint8 flagB20 : 1;
	//uint8 flagB40 : 1;
	//uint8 flagB80 : 1;

	uint8 spawnRate : 4;
	uint8 flagsC : 4;

	CVector GetPosition(void) { return CVector(x/8.0f, y/8.0f, z/8.0f); }
	void SetPosition(const CVector &p) { x = p.x*8.0f; y = p.y*8.0f; z = p.z*8.0f; }
	float GetX(void) { return x/8.0f; }
	float GetY(void) { return y/8.0f; }
	float GetZ(void) { return z/8.0f; }
	CPathNode *GetPrev(void);
	CPathNode *GetNext(void);
	void SetPrev(CPathNode *node);
	void SetNext(CPathNode *node);
#endif
};

union CConnectionFlags
{
	uint8 flags;
	struct {
		uint8 bCrossesRoad : 1;
		uint8 bTrafficLight : 1;
	};
};

struct CCarPathLink
{
#ifndef MIAMI
	CVector2D pos;
	CVector2D dir;
	int16 pathNodeIndex;
	int8 numLeftLanes;
	int8 numRightLanes;
	uint8 trafficLightType;

	uint8 bBridgeLights : 1;
	// more?

	CVector2D &GetPosition(void) { return pos; }
	CVector2D &GetDirection(void) { return dir; }
	float GetX(void) { return pos.x; }
	float GetY(void) { return pos.y; }
	float GetDirX(void) { return dir.x; }
	float GetDirY(void) { return dir.y; }
#else
	int16 x;
	int16 y;
	int16 pathNodeIndex;
	int8 dirX;
	int8 dirY;
	int8 numLeftLanes : 3;
	int8 numRightLanes : 3;
	uint8 flag1 : 1;
	uint8 trafficLightType : 2;
	uint8 bBridgeLights : 1;	// at least in LCS...
	int8 width;

	CVector2D GetPosition(void) { return CVector2D(x/8.0f, y/8.0f); }
	CVector2D GetDirection(void) { return CVector2D(dirX/100.0f, dirY/100.0f); }
	float GetX(void) { return x/8.0f; }
	float GetY(void) { return y/8.0f; }
	float GetDirX(void) { return dirX/100.0f; }
	float GetDirY(void) { return dirY/100.0f; }
#endif

	float OneWayLaneOffset()
	{
		if (numLeftLanes == 0)
			return 0.5f - 0.5f * numRightLanes;
		if (numRightLanes == 0)
			return 0.5f - 0.5f * numLeftLanes;
		return 0.5f;
	}
};

// This is what we're reading from the files, only temporary
struct CPathInfoForObject
{
#ifndef MIAMI
	int16 x;
	int16 y;
	int16 z;
	int8 type;
	int8 next;
	int8 numLeftLanes;
	int8 numRightLanes;
	uint8 crossing : 1;
#else
	float x;
	float y;
	float z;
	int8 type;
	int8 next;
	int8 numLeftLanes;
	int8 numRightLanes;
	int8 speedLimit;
	int8 width;

	uint8 crossing : 1;
	uint8 flag02 : 1;	// always zero
	uint8 roadBlock : 1;
	uint8 disabled : 1;
	uint8 waterPath : 1;
	uint8 betweenLevels : 1;

	uint8 spawnRate : 4;

	void SwapConnectionsToBeRightWayRound(void);
#endif
};
extern CPathInfoForObject *InfoForTileCars;
extern CPathInfoForObject *InfoForTilePeds;

struct CTempNode
{
	CVector pos;
#ifndef MIAMI
	float dirX;
	float dirY;
	int16 link1;
	int16 link2;
	int8 numLeftLanes;
	int8 numRightLanes;
	int8 linkState;
#else
	int8 dirX;	// *100
	int8 dirY;
	int16 link1;
	int16 link2;
	int8 numLeftLanes;
	int8 numRightLanes;
	int8 width;
	bool isCross;
	int8 linkState;
#endif
};

#ifdef MIAMI
struct CTempNodeExternal	// made up name
{
	CVector pos;
	int16 next;
	int8 numLeftLanes;
	int8 numRightLanes;
	int8 width;
	bool isCross;
};
#endif

#ifndef MIAMI
struct CTempDetachedNode	// unused
{
	uint8 foo[20];
};
#endif

class CPathFind
{
public:
	CPathNode m_pathNodes[NUM_PATHNODES];
	CCarPathLink m_carPathLinks[NUM_CARPATHLINKS];
	CTreadable *m_mapObjects[NUM_MAPOBJECTS];
#ifndef MIAMI
	uint8 m_objectFlags[NUM_MAPOBJECTS];
	int16 m_connections[NUM_PATHCONNECTIONS];
	int16 m_distances[NUM_PATHCONNECTIONS];
	CConnectionFlags m_connectionFlags[NUM_PATHCONNECTIONS];
#else
	uint16 m_connections[NUM_PATHCONNECTIONS];	// and flags
	uint8 m_distances[NUM_PATHCONNECTIONS];
#endif
	int16 m_carPathConnections[NUM_PATHCONNECTIONS];

	int32 m_numPathNodes;
	int32 m_numCarPathNodes;
	int32 m_numPedPathNodes;
	int16 m_numMapObjects;
	int16 m_numConnections;
	int32 m_numCarPathLinks;
	int32 unk;
	uint8 m_numGroups[2];
	CPathNode m_searchNodes[512];

	void Init(void);
	void AllocatePathFindInfoMem(int16 numPathGroups);
	void RegisterMapObject(CTreadable *mapObject);
	void StoreNodeInfoPed(int16 id, int16 node, int8 type, int8 next, int16 x, int16 y, int16 z, int16 width, bool crossing);
	void StoreNodeInfoCar(int16 id, int16 node, int8 type, int8 next, int16 x, int16 y, int16 z, int16 width, int8 numLeft, int8 numRight);
#ifndef MIAMI
	void CalcNodeCoors(int16 x, int16 y, int16 z, int32 id, CVector *out);
#else
	void CalcNodeCoors(float x, float y, float z, int32 id, CVector *out);
#endif
	bool LoadPathFindData(void);
	void PreparePathData(void);
	void CountFloodFillGroups(uint8 type);
	void PreparePathDataForType(uint8 type, CTempNode *tempnodes, CPathInfoForObject *objectpathinfo,
#ifndef MIAMI
		float maxdist, CTempDetachedNode *detachednodes, int32 numDetached);
#else
		float maxdist, CPathInfoForObject *detachednodes, int32 numDetached);
#endif

	bool IsPathObject(int id) { return id < PATHNODESIZE && (InfoForTileCars[id*12].type != 0 || InfoForTilePeds[id*12].type != 0); }

	float CalcRoadDensity(float x, float y);
	bool TestForPedTrafficLight(CPathNode *n1, CPathNode *n2);
	bool TestCrossesRoad(CPathNode *n1, CPathNode *n2);
	void AddNodeToList(CPathNode *node, int32 listId);
	void RemoveNodeFromList(CPathNode *node);
	void RemoveBadStartNode(CVector pos, CPathNode **nodes, int16 *n);
	void SetLinksBridgeLights(float, float, float, float, bool);
	void SwitchOffNodeAndNeighbours(int32 nodeId, bool disable);
	void SwitchRoadsOffInArea(float x1, float x2, float y1, float y2, float z1, float z2, bool disable);
	void SwitchPedRoadsOffInArea(float x1, float x2, float y1, float y2, float z1, float z2, bool disable);
	void SwitchRoadsInAngledArea(float x1, float y1, float z1, float x2, float y2, float z2, float length, uint8 type, uint8 enable);
	void MarkRoadsBetweenLevelsNodeAndNeighbours(int32 nodeId);
	void MarkRoadsBetweenLevelsInArea(float x1, float x2, float y1, float y2, float z1, float z2);
	void PedMarkRoadsBetweenLevelsInArea(float x1, float x2, float y1, float y2, float z1, float z2);
#ifndef MIAMI
	int32 FindNodeClosestToCoors(CVector coors, uint8 type, float distLimit, bool ignoreDisabled = false, bool ignoreBetweenLevels = false);
#else
//--MIAMI: TODO: check callers for new arguments
	int32 FindNodeClosestToCoors(CVector coors, uint8 type, float distLimit, bool ignoreDisabled = false, bool ignoreBetweenLevels = false, bool ignoreFlagB4 = false, bool bWaterPath = false);
#endif
	int32 FindNodeClosestToCoorsFavourDirection(CVector coors, uint8 type, float dirX, float dirY);
	float FindNodeOrientationForCarPlacement(int32 nodeId);
	float FindNodeOrientationForCarPlacementFacingDestination(int32 nodeId, float x, float y, bool towards);
	bool NewGenerateCarCreationCoors(float x, float y, float dirX, float dirY, float spawnDist, float angleLimit, bool forward, CVector *pPosition, int32 *pNode1, int32 *pNode2, float *pPositionBetweenNodes, bool ignoreDisabled = false);
	bool GeneratePedCreationCoors(float x, float y, float minDist, float maxDist, float minDistOffScreen, float maxDistOffScreen, CVector *pPosition, int32 *pNode1, int32 *pNode2, float *pPositionBetweenNodes, CMatrix *camMatrix);
#ifndef MIAMI
	CTreadable *FindRoadObjectClosestToCoors(CVector coors, uint8 type);
#endif
	void FindNextNodeWandering(uint8, CVector, CPathNode**, CPathNode**, uint8, uint8*);
	void DoPathSearch(uint8 type, CVector start, int32 startNodeId, CVector target, CPathNode **nodes, int16 *numNodes, int16 maxNumNodes, CVehicle *vehicle, float *dist, float distLimit, int32 forcedTargetNode);
	bool TestCoorsCloseness(CVector target, uint8 type, CVector start);
	void Save(uint8 *buf, uint32 *size);
	void Load(uint8 *buf, uint32 size);

#ifdef MIAMI
	CPathNode *GetNode(int16 index);
	int16 GetIndex(CPathNode *node);

	uint16 ConnectedNode(int id) { return m_connections[id] & 0x3FFF; }
	bool ConnectionCrossesRoad(int id) { return !!(m_connections[id] & 0x8000); }
	bool ConnectionHasTrafficLight(int id) { return !!(m_connections[id] & 0x4000); }
	void ConnectionSetTrafficLight(int id) { m_connections[id] |= 0x4000; }
#else
	uint16 ConnectedNode(int id) { return m_connections[id]; }
	bool ConnectionCrossesRoad(int id) { return m_connectionFlags[id].bCrossesRoad; }
	bool ConnectionHasTrafficLight(int id) { return m_connectionFlags[id].bTrafficLight; }
	void ConnectionSetTrafficLight(int id) { m_connectionFlags[id].bTrafficLight = true; }
#endif

	void DisplayPathData(void);
};
#ifndef MIAMI
static_assert(sizeof(CPathFind) == 0x49bf4, "CPathFind: error");
#endif

extern CPathFind ThePaths;

#ifdef MIAMI
inline CPathNode *CPathNode::GetPrev(void) { return ThePaths.GetNode(prevIndex); }
inline CPathNode *CPathNode::GetNext(void) { return ThePaths.GetNode(nextIndex); }
inline void CPathNode::SetPrev(CPathNode *node) { prevIndex = ThePaths.GetIndex(node); }
inline void CPathNode::SetNext(CPathNode *node) { nextIndex = ThePaths.GetIndex(node); }
#endif

extern bool gbShowPedPaths;
extern bool gbShowCarPaths;
extern bool gbShowCarPathsLinks;
