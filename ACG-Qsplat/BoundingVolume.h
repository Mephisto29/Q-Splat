#ifndef BOUNDINGVOLUME_H
#define BOUNDINGVOLUME_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <limits>
#include <string>
#include <vector>
#include <math.h>
#include <cmath>
#include <algorithm>
//#include "Vector3.h"
#include "constructor.h"
#include "constructor.h"

#include <windows.h>
#include <GL/gl.h>
#include <GL/glut.h>


bool minVertX(Point& vert, Point& vertTwo){return vert.x<vertTwo.x;}
bool minVertY(Point& vert, Point& vertTwo){return vert.y<vertTwo.y;}
bool minVertZ(Point& vert, Point& vertTwo){return vert.z<vertTwo.z;}

vector<Point> vertices;


class BoundingVolumeNode
{
public:
	bool leaf;
	bool empty;
	Vector centre;
	Vector normal;
	Vector min;
	Vector max;

	BoundingVolumeNode* left;
	BoundingVolumeNode* right;

	int containedVerts;
	int processedVerts;
	int depth;
	int number;
	float radius;


	// default constructor
	BoundingVolumeNode()
	{
		leaf = false;
		empty = false;
		containedVerts = 0;
		processedVerts = 0;
	}

	// coppy constructor
	BoundingVolumeNode operator=(const BoundingVolumeNode & node)
	{
		BoundingVolumeNode out;

		out.min = node.min;
		out.max = node.max;
		out.centre = node.centre;
		out.radius = node.radius;
		out.normal = node.normal;
		out.processedVerts = node.processedVerts;
		return out;
	}

	BoundingVolumeNode* test()
	{
		BoundingVolumeNode* output = new BoundingVolumeNode();

		output->radius = 100;

		return output;
	}



	// BUILD THE TREE,  get start vert, and end vert, min and max, and a depth (just incase)
	BoundingVolumeNode* buildTree(int start, int end, Vector newMin, Vector newMax, int depth, vector<Point> newVertices)
	{
		BoundingVolumeNode* out = new BoundingVolumeNode();
		//cout<<start<<endl;
		vertices = newVertices;
		//cout<<vertices.at(0).radius<<endl;

		containedVerts = end - start;

		if(containedVerts <=1)
		{
			if(containedVerts !=0)
			{
				processedVerts++;

				Point vert = vertices.at(start);

				out->leaf = true;
				out->depth = depth;
				//cout<< depth<<endl;
				out->centre = Vector(vert.x,vert.y,vert.z);
				out->radius = vert.radius;
				//cout<< vert.radius<<endl;
				out->normal = vert.normal;
				out->max = newMax;
				out->min = newMin;
				out->processedVerts = 1;

				return out;
			}

			out->leaf=true;
			out->empty=true;
			return out;
		}
		
		else
		{
			Vector leftMax = Vector(newMax);
			Vector rightMin = Vector(newMin);
			Vector mid;

			vector<Point> sorter;
			for(int i = start; i < end ; i++)
			{
				sorter.push_back(vertices.at(i));
			}

			int midpoint = axisSplit(sorter.begin(), sorter.end(), newMin, newMax, mid);

			if(mid.x != 0)
			{
				leftMax.x = mid.x;
				rightMin.x = mid.x;
			}
			else if (mid.y != 0)
			{
				leftMax.y = mid.y;
				rightMin.y = mid.y;
			}
			else if(mid.z != 0)
			{
				leftMax.z = mid.z;
				rightMin.z = mid.z;
			}

			out->left =  buildTree(start, start + midpoint, newMin, leftMax, depth + 1, newVertices);		
			out->right = buildTree(start + midpoint, end, rightMin, newMax, depth + 1, newVertices);

			out->min = newMin;
			out->max = newMax;
			//out->calculateBoundingSphere();
			out->centre = (out->left->centre + out->right->centre)/2;
			//(out->leftSubTree->sphereRadius+out->rightSubTree->sphereRadius)
			float newRadius = (out->left->centre - out->right->centre).length() + ((out->right->radius + out->left->radius)/2);
				//+(out->leftSubTree->sphereRadius+out->rightSubTree->sphereRadius);
			out->radius = newRadius;
			
			out->normal = out->left->normal + out->right->normal;
			out->depth = depth;
			out->processedVerts = out->left->processedVerts + out->right->processedVerts;
		}
		//cout<<out->radius<<endl;
		return out;
	}

	int axisSplit(vector<Point>::iterator& start, vector<Point>::iterator& end, Vector newMin, Vector newMax, Vector mid)
	{

		// Get max Axis
		float xAxis = newMax.x-newMin.x;
		float yAxis = newMax.y-newMin.y;
		float zAxis = newMax.z-newMin.z;

		float max = max(xAxis, max(yAxis, zAxis));

		if(max == xAxis)
		{
			std::sort(start, end, &minVertX);		
			mid.x = (*(start + (containedVerts/2))).x;
		}
		else if (max == yAxis)
		{
			std::sort(start, end, &minVertY);		
			mid.y = (*(start + (containedVerts/2))).y;
		}
		else if (max == zAxis)
		{
			std::sort(start, end, &minVertZ);		
			mid.z = (*(start + (containedVerts/2))).z;
		}

		return containedVerts/2;
	}

	
	/*else{

		Vector3 leftBBMax = Vector3(bbMax), rightBBmin=Vector3(bbMin);	
		Vector3 deltaBB;
		
		int midPoint = partitionAlongLongestAxis(verticesStart, verticesEnd, bbMin, bbMax, deltaBB);
		
		for(int i = 0; i<3;i++){
			if(deltaBB[i]!=0){
				leftBBMax[i]=deltaBB[i];
				rightBBmin[i]=deltaBB[i];
			}

		}*/
	// sort the verts from start to end.
	// find the largest difference between min and max (x / y / z) within the sorted list, and split the array in half along that axis
	// find the Axis value for the middle element in the array, and set that element for your min and max for the next recursion.
	// call left and right subtrees using min, leftmax, and rightmin, max.
	// collect different element values into node
	// return node

};

#endif