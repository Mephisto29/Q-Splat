//#include <iostream>
//#include <fstream>
//#include <sstream>
//#include <limits>
//#include <math.h>
//#include <string>
//#include <vector>
//#include <math.h>
//#include <cmath>
//#include <algorithm>
#include <windows.h>

class Vector3{	
public:
	float x, y, z, radius, maxRadius, currRadius;
	float nx,ny,nz;

	Vector3()
	{
		x = 0, y = 0, z = 0, nx = 0, ny = 0, nz = 0;
	}
	Vector3(float x1, float y1, float z1)
	{
		x = x1;
		y = y1;
		z = z1;
		nx = 0;
		ny = 0;
		nz = 0;
	}

	void getLength(Vector3 v1, Vector3 v2, Vector3 v3)
	{

		//================================================================
		// =======================  V1
		maxRadius = 0;
		currRadius = v1.radius;

		maxRadius = max(Length(v1,v2),Length(v1,v3));

		if (maxRadius < currRadius)
			maxRadius = currRadius;

		v1.radius = maxRadius;

		//========================  V2
		maxRadius = 0;
		currRadius = v2.radius;

		maxRadius = max(Length(v1,v2),Length(v2,v3));

		if (maxRadius < currRadius)
			maxRadius = currRadius;

		v2.radius = maxRadius;

		//=======================  V3
		maxRadius = 0;
		currRadius = v3.radius;

		maxRadius = max(Length(v1,v3),Length(v2,v3));

		if (maxRadius < currRadius)
			maxRadius = currRadius;

		v3.radius = maxRadius;

		//=============================================================
		// Get the normal of the current vertex

		Vector3 temp1((v1.x - v2.x), (v1.y - v2.y), (v1.z - v2.z));
		Vector3 temp2((v1.x - v3.x), (v1.y - v3.y), (v1.z - v3.z));

		Vector3 temp3 ((temp1.y * temp2.z)-(temp2.y*temp1.z),-(temp1.x*temp2.z)+(temp2.x*temp1.z),(temp1.x*temp2.y)-(temp1.y*temp2.x));

		normalise(temp3);

		Vector3 normal1((v1.nx + temp3.x),(v1.ny + temp3.y), (v1.nz + temp3.z));
		Vector3 normal2((v2.nx + temp3.x),(v2.ny + temp3.y), (v2.nz + temp3.z));
		Vector3 normal3((v3.nx + temp3.x),(v3.ny + temp3.y), (v3.nz + temp3.z));

// ============================  V1
		normalise(normal1);

		v1.nx = normal1.x;
		v1.ny = normal1.y;
		v1.nz = normal1.z;

// ============================  V2
		normalise(normal2);

		v2.nx = normal2.x;
		v2.ny = normal2.y;
		v2.nz = normal2.z;

// ============================  V3
		normalise(normal3);

		v3.nx = normal3.x;
		v3.ny = normal3.y;
		v3.nz = normal3.z;
		
	}
	
	float Length(Vector3 v1, Vector3 v2)
	{
		float length = 0;

		length = sqrt(pow((v1.x - v2.x),2) + pow((v1.y - v2.y),2) + pow((v1.z -v2.z),2));

		return length;
	}

	float Length(Vector3 v1)
	{
		float length = 0;

		length = sqrt(pow((v1.x),2) + pow((v1.y),2) + pow((v1.z),2));

		return length;
	}

	void normalise(Vector3 v1)
	{
		v1.x = v1.x/v1.Length(v1);
		v1.y = v1.y/v1.Length(v1);
		v1.z = v1.z/v1.Length(v1);
	}

};