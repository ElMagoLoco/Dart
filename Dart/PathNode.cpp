#include "PathNode.h"
#include <time.h>
#include "Level.h"


//constructor
#ifdef DEBUG_PATHS
PathNode::PathNode(LPCWSTR _mesh, D3DXVECTOR3 _position, D3DXVECTOR3 _scale) :
mPosition(_position), mScale(_scale)
{
	mMesh = new Mesh(_mesh, _position, _scale);
	//textures for the mesh are set when the grid is initialized
}
#else
PathNode::PathNode(D3DXVECTOR3 _position) : mPosition(_position)
{
}
#endif

AStar::AStar()
{
}

AStar::~AStar()
{
	for (PathNode* PN : mPathNodes)
		delete PN;
	vector<PathNode*>().swap(mPathNodes);
}
//takes the corner of the -x/-z corner and int size
//since the map for this is always flat and square
//works best when map size is divisible by GRID_SIZE
//This only works effectively if done after all the addWorldGeometry
void AStar::initPathfinding()
{
	D3DXVECTOR2 corner = gCurrentLevel->getNegCorner();
	//set up path nodes for enemy path finding
	//place path nodes every GRID_SIZE units except
	//in places where it would collide
	UINT iterations = gCurrentLevel->getSquareSize() / GRID_SIZE;
	for (UINT x = 0; x < iterations; ++x)
	{
		for (UINT z = 0; z < iterations; ++z)
		{
			bool canPlace = true;
			//for each mesh in the current map
			for (Mesh* M : gCurrentLevel->getWorldGeometry())
			{
				//for each AABB in that mesh
				for (AxisAlignedBoundingBox2D* AABB : M->getAABBs())
				{
					//find the center of the node to compare with
					D3DXVECTOR2 nodeCenter = D3DXVECTOR2(corner.x + (float)(x * GRID_SIZE),
						corner.y + (float)(z * GRID_SIZE));
					//make a bounding sphere with it
					BoundingSphere2D* PS = new BoundingSphere2D(nodeCenter, CLOSE_RADIUS);
					//if the AABB collides with that sphere, we can't place one here
					if (collides(*AABB, *PS))
					{
						canPlace = false;
						break;
					}
					if (!canPlace)
						break;
				}
				if (!canPlace)
					break;
			}
			//if no obstruction was found, place the node
			if (true)
			{
				D3DXVECTOR3 position = D3DXVECTOR3(corner.x, 50.0f, corner.y)
					+ D3DXVECTOR3((float)(x * GRID_SIZE), 0.0f, (float)(z * GRID_SIZE));

#ifdef DEBUG_PATHS
				//set up the mesh if in debug
				PathNode* pn = new PathNode(L"Content/Models/ball.x", position, D3DXVECTOR3(2.0f, 2.0f, 2.0f));
#else
				//use no mesh if not in debug, this saves a lot of time loading
				PathNode* pn = new PathNode(position);
#endif
				//add new node
				mPathNodes.push_back(pn);
			}
		}
	}
	//connect the nodes to their closest neighbors
	connectPathfinding();
}

void AStar::draw()
{
	//only draw nodes if in debug
#ifdef DEBUG_PATHS
	for (PathNode* PN : mPathNodes)
	{
		PN->getMesh()->draw();
	}
#endif
}
//get a path node
void AStar::getPathNode(PathNode*& _node, UINT _num)
{
	if (_num < mPathNodes.size())
		_node = mPathNodes[_num];
	else
		_node = NULL;
}

//find the node at that position, used by connectPathfinding
void AStar::getNodeAtPosition(PathNode*& _node, D3DXVECTOR3 _position)
{
	for (PathNode* PN : mPathNodes)
	{
		if (PN->getPosition() == _position)
		{
			_node = PN;
			return;
		}
	}
	_node = NULL;
}
//find node nearest the position, used by findPath
//will return the first node within 150 units
void AStar::getNodeNearPosition(PathNode*& _node, D3DXVECTOR3 _position)
{
	PathNode* whichNode = NULL;
	float closestDistanceSq = 1000000000000000000000000.0f;
	for (UINT i = 0; i < mPathNodes.size(); ++i)
	{
		float distanceSq = D3DXVec3LengthSq(&(_position - mPathNodes[i]->getPosition()));
		if (distanceSq < closestDistanceSq)
		{
			whichNode = mPathNodes[i];
			closestDistanceSq = distanceSq;
		}
	}
	_node = whichNode;
}
//find the close nodes and connect to them
void AStar::connectPathfinding()
{
	//this assumes path nodes are all GRID_SIZE apart in a grid, as set up by initPathfinding
	for (PathNode* PN : mPathNodes)
	{
		PathNode* link;
		//check each direction to see if there is a node GRID_SIZE units in that direction
		//if there is, add it; if not then skip
		//west
		getNodeAtPosition(link, D3DXVECTOR3(PN->getPosition().x - GRID_SIZE, PN->getPosition().y, PN->getPosition().z));
		if (link != NULL)
		{
			PN->addLinkedNode(link);
		}
		//east
		getNodeAtPosition(link, D3DXVECTOR3(PN->getPosition().x + GRID_SIZE, PN->getPosition().y, PN->getPosition().z));
		if (link != NULL)
		{
			PN->addLinkedNode(link);
		}
		//south
		getNodeAtPosition(link, D3DXVECTOR3(PN->getPosition().x, PN->getPosition().y, PN->getPosition().z + GRID_SIZE));
		if (link != NULL)
		{
			PN->addLinkedNode(link);
		}
		//north
		getNodeAtPosition(link, D3DXVECTOR3(PN->getPosition().x, PN->getPosition().y, PN->getPosition().z - GRID_SIZE));
		if (link != NULL)
		{
			PN->addLinkedNode(link);
		}
		//northeast
		getNodeAtPosition(link, D3DXVECTOR3(PN->getPosition().x + GRID_SIZE, PN->getPosition().y, PN->getPosition().z - GRID_SIZE));
		if (link != NULL)
		{
			PN->addLinkedNode(link);
		}
		//northwest
		getNodeAtPosition(link, D3DXVECTOR3(PN->getPosition().x - GRID_SIZE, PN->getPosition().y, PN->getPosition().z - GRID_SIZE));
		if (link != NULL)
		{
			PN->addLinkedNode(link);
		}
		//southwest
		getNodeAtPosition(link, D3DXVECTOR3(PN->getPosition().x - GRID_SIZE, PN->getPosition().y, PN->getPosition().z + GRID_SIZE));
		if (link != NULL)
		{
			PN->addLinkedNode(link);
		}
		//southeast
		getNodeAtPosition(link, D3DXVECTOR3(PN->getPosition().x + GRID_SIZE, PN->getPosition().y, PN->getPosition().z + GRID_SIZE));
		if (link != NULL)
		{
			PN->addLinkedNode(link);
		}
		//if we are debugging paths, set their texture based on how many links they have
#ifdef DEBUG_PATHS
		if (PN->getNumLinks() < 4)
			PN->getMesh()->addTexture(L"Content/Textures/energy_h.dds", L"Content/Textures/energy_n.dds");
		else if (PN->getNumLinks() < 6)
			PN->getMesh()->addTexture(L"Content/Textures/grass_h.dds", L"Content/Textures/grass_n.dds");
		else if (PN->getNumLinks() < 8)
			PN->getMesh()->addTexture(L"Content/Textures/ice_h.dds", L"Content/Textures/ice_n.dds");
		else
			PN->getMesh()->addTexture(L"Content/Textures/fire_h.dds", L"Content/Textures/fire_n.dds");
#endif
	}
}
//find heuristic between two nodes by looking for it between their positions
int AStar::findHeuristic(PathNode* _start, PathNode* _end)
{
	return findHeuristic(_start->getPosition(), _end->getPosition());
}
//find heuristic between two positions
//to save processing time we're comparing the length squared instead of length and
//using int instead of float
//ignores y distance because it is a 2d map surface
int AStar::findHeuristic(D3DXVECTOR3 _start, D3DXVECTOR3 _end)
{
	int xDist = int(_start.x - _end.x);
	int zDist = int(_start.z - _end.z);
	return xDist * xDist + zDist * zDist;
}

void AStar::findLowestFScore(PathNode*& _node)
{
	int compare;//FScore we compare in loop to find lowest
	PathNode* pnLowest = NULL;//path node with lowest fScore
	int iLowest = INT_MAX;//lowest fScore
	//for each path node
	for (PathNode* PN : mOpenSet)
	{
		//find the fScore
		compare = PN->getFScore();
		// compare it to present lowest
		if (compare < iLowest)
		{
			//if lower, save new result
			pnLowest = PN;
			iLowest = compare;
		}
	}
	//return final result
	_node = pnLowest;
}


bool AStar::inClosedSet(PathNode* _node)
{
	for (vector<PathNode*>::iterator iter = mClosedSet.begin(); iter != mClosedSet.end(); ++iter)
	{
		if (*iter == _node)
			return true;
	}
	return false;
}

void AStar::reconstructPath(vector<D3DXVECTOR3>& _path, PathNode* _start, PathNode* _end)
{
	vector<D3DXVECTOR3>().swap(_path);
	_path.push_back(_end->getPosition());
	while (_start != _end)
	{
		_end = _end->getCameFrom();
		_path.push_back(_end->getPosition());
	}
}

bool AStar::inOpenSet(PathNode* _node)
{
	for (vector<PathNode*>::iterator iter = mOpenSet.begin(); iter != mOpenSet.end(); ++iter)
	{
		if (*iter == _node)
			return true;
	}
	return false;
}

vector<D3DXVECTOR3> AStar::findPath(D3DXVECTOR3 _start, D3DXVECTOR3 _end)
{
	//create a temporary node at the start position to start from
	//this prevents it from jerking about slightly each time it finds
	//a new path
	PathNode* startNode;
	getNodeNearPosition(startNode, _start);
	PathNode* endNode;
	getNodeNearPosition(endNode, _end);
	if (_start == NULL || _end == NULL)
		return vector<D3DXVECTOR3>();
	else
		return findPath(startNode, endNode);
}


vector<D3DXVECTOR3> AStar::findPath(D3DXVECTOR3 _start, PathNode* _end)
{
	PathNode* startNode;
	getNodeNearPosition(startNode, _start);
	if (_start == NULL)
		return vector<D3DXVECTOR3>();
	else
		return findPath(startNode, _end);
}

vector<D3DXVECTOR3> AStar::findPath(PathNode* _start, D3DXVECTOR3 _end)
{
	PathNode* endNode;
	getNodeNearPosition(endNode, _end);
	if (_end == NULL)
		return vector<D3DXVECTOR3>();
	else
		return findPath(_start, endNode);
}

vector<D3DXVECTOR3> AStar::findPath(PathNode* _start, PathNode* _end)
{
	//time it for debugging
	clock_t clockStart = clock();
	//empty the closed set vector
	vector<PathNode*>().swap(mClosedSet);
	//empty the open set and put in only the start node
	vector<PathNode*>().swap(mOpenSet);
	mOpenSet.push_back(_start);
	//set all node gScores to max
	for (PathNode* PN : mPathNodes)
		PN->setGScore(INT_MAX);
	//set start node properties
	_start->setGScore(0);
	_start->setFScore(findHeuristic(_start, _end));
	//while the open set is not empty
	while (mOpenSet.size() > 0)
	{
		//the node in open set having the lowest f score
		PathNode* current;
		findLowestFScore(current);
		//if we found the goal, return the path
		if (current == _end || ((clock() - clockStart) / (float)CLOCKS_PER_SEC) > MAX_PATH_TIME_LOW)
		{
			vector<D3DXVECTOR3> rePath;
			reconstructPath(rePath, _start, current);
			return rePath;
		}
		//save current to closed set
		mClosedSet.push_back(current);
		//remove current from open set
		for (vector<PathNode*>::iterator iter = mOpenSet.begin(); iter != mOpenSet.end(); ++iter)
		{
			if (*iter == current)
			{
				mOpenSet.erase(iter);
				break;
			}
		}
		//for each linked node in the current node
		for (PathNode* PN : current->getLinks())
		{
			//if it is already in the closed set, continue
			if (inClosedSet(PN))
				continue;
			//find tentative gScore
			int tempGScore = current->getGScore() + findHeuristic(current, _end);
			//if link node is not in open set or tempGScore < present GScore
			if (!inOpenSet(PN) || tempGScore < PN->getGScore())
			{
				//set the came from of the link to the current node
				PN->setCameFrom(current);
				//set g score
				PN->setGScore(tempGScore);
				PN->setFScore(PN->getGScore() + findHeuristic(PN, _end));
				//if link is not in open set, add to it
				if (!inOpenSet(PN))
					mOpenSet.push_back(PN);
			}
		}
	}
	//finished loop without finding goal
	OutputDebugString(L"ERROR: No path found.");
	return vector<D3DXVECTOR3>();

}

