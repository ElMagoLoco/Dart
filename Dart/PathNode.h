#pragma once

#include <vector>
#include "D3DUtils.h"
#include "Mesh.h"

using std::vector;

//this will draw the path nodes and color them based on how many links they have
//turning this on can help debug paths, but will massively increase startup time
//#define DEBUG_PATHS

//paths stop at this time length for unimportant paths like wandering
#define MAX_PATH_TIME_LOW 0.75f
//paths stop at this time length for important paths like pursuing
#define MAX_PATH_TIME_HIGH 0.175f
//size of grid squares, remember to make this divisible by level sizes
#define GRID_SIZE 100
//how close can a path node spawn to an obstacle
#define CLOSE_RADIUS 64.0f

//class for individual path nodes for use with path finding
class PathNode
{
public:
	//path nodes will not draw a mesh unless #define DEBUG_PATHS is set at the top of PathNode.h
	//constructor/destructor
#ifdef DEBUG_PATHS
	PathNode(LPCWSTR _mesh, D3DXVECTOR3 _position, D3DXVECTOR3 _scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f));
#else
	PathNode(D3DXVECTOR3 _position);
#endif
	//get a particular node link
	PathNode* getLinkedNode(UINT _num)		{ if (_num < mLinked.size()) return mLinked[_num]; else return NULL; }
	//get the whole linked vector
	vector<PathNode*> getLinks()			{ return mLinked; }
	//link to another node
	void addLinkedNode(PathNode* _node)		{ mLinked.push_back(_node); }
	void setLinks(vector<PathNode*> _links) { vector<PathNode*>().swap(mLinked); mLinked = _links; }
	//get all links from this node
	UINT getNumLinks()						{ return mLinked.size(); }
	//set functions
	void setGScore(int _score)				{ mGScore = _score; }
	void setFScore(int _score)				{ mFScore = _score; }
	void setCameFrom(PathNode* cameFrom)	{ mCameFrom = cameFrom; }
	void setPosition(D3DXVECTOR3 _position) { mPosition = _position; }
	//get functions
	int getGScore()							{ return mGScore; }
	int getFScore()							{ return mFScore; }
	PathNode* getCameFrom()					{ return mCameFrom; }
#ifdef DEBUG_PATHS
	Mesh* getMesh()							{ return mMesh; }
#endif
	D3DXVECTOR3 getPosition()				{ return mPosition; }
private:
#ifdef DEBUG_PATHS
	//mesh, only used if debugging paths
	Mesh* mMesh;
	D3DXVECTOR3 mScale;
#endif
	//position
	D3DXVECTOR3 mPosition;
	//close nodes that it is linked to
	vector<PathNode*> mLinked;
	//node it came from during path finding
	PathNode* mCameFrom;
	//cost from start of a path along best known path so far
	int mGScore;
	//heuristic cost to end of path from present point
	int mFScore;
};
//AStar path finding for enemy AI
class AStar
{
public:
	AStar();
	~AStar();
	//init path finding = set up nodes and connect them
	void initPathfinding();
	//only draw if #define DEBUG_PATHS is at the top of PathNode.h
	void draw();
	//get functions
	void getPathNode(PathNode*& _node, UINT _num);
	vector<PathNode*> getPathNodes()	{ return mPathNodes; }
	UINT getNumNodes()					{ return mPathNodes.size(); }
	//find path using either vector positions or nodes
	vector<D3DXVECTOR3> findPath(D3DXVECTOR3 _start, D3DXVECTOR3 _end);
	vector<D3DXVECTOR3> findPath(D3DXVECTOR3 _start, PathNode* _end);
	vector<D3DXVECTOR3> findPath(PathNode* _start, D3DXVECTOR3 _end);
	vector<D3DXVECTOR3> findPath(PathNode* _start, PathNode* _end);
	//find a node given a 3d position or return NULL if not one there
	void getNodeAtPosition(PathNode*& _node, D3DXVECTOR3 _position);
	//find closest node to a 3d position
	void getNodeNearPosition(PathNode*& _node, D3DXVECTOR3 _position);
private:
	void connectPathfinding();//connect nodes together, called from initPathfinding
	//find the heuristic cost, used by findPath
	int findHeuristic(D3DXVECTOR3 _start, D3DXVECTOR3 end);
	int findHeuristic(PathNode* _start, PathNode* _end);
	//find node in open set having lowest fScore
	void findLowestFScore(PathNode*& _node);
	//find the final path after reaching the goal in path finding, called from findPath
	void reconstructPath(vector<D3DXVECTOR3>& _path, PathNode* _start, PathNode* _end);
	//return true if a node is in the closed set, called from findPath
	bool inClosedSet(PathNode* _node);
	//return true if node is in open set, called from findPath
	bool inOpenSet(PathNode* _node);
	//actual path nodes
	vector<PathNode*>	mPathNodes;
	//keeps the set of nodes already evaluated during path finding
	vector<PathNode*> mClosedSet;
	//nodes to be evaluated, initially contains the start node
	vector<PathNode*> mOpenSet;
	//the set of nodes traveled so far
	vector<PathNode*> mCameFrom;
};