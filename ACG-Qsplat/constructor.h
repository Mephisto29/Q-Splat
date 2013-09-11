#ifndef CONSTRUCTOR_H
#define CONSTRUCTOR_H
#include <iostream>
#include <fstream>
#include <sstream>
#include <limits>
#include <math.h>
#include <string>
#include <vector>
#include <math.h>
#include <cmath>
#include <algorithm>
#include <windows.h>

using namespace std;

class Vector
{
public:
	float x,y,z;

	//Default constructors and other constructors for Vector3's
	Vector()
	{
		x = 0, y = 0, z = 0;
	}
	//Vector(Vector& vec){x=vec.x,y=vec.y,z=vec.z;}
	Vector(float a, float b, float c)
	{
		x = a, y = b, z = c;
	}

	// Overide the += -= *= and /= operators
	void operator+=(Vector& vec)
	{
		x += vec.x;
		y += vec.y;
		z += vec.z;		
	}
	void operator-=(Vector& vec)
	{
		x -= vec.x;
		y -= vec.y;
		z -= vec.z;		
	}
	void operator*=(float num)
	{
		x *= num;
		y *= num;
		z *= num;		
	}
	void operator/=(float num)
	{
		x /= num;
		y /= num;
		z /= num;		
	}

	// overide the normal maths operators
	Vector operator+(Vector& vec)
	{
		return Vector(x+vec.x, y+vec.y, z+vec.z);		
	}
	Vector operator-(Vector& vec)
	{
		return Vector(x-vec.x, y-vec.y, z-vec.z);		
	}
	Vector operator*(float num)
	{
		return Vector(x*num, y*num, z*num);		
	}
	Vector operator/(float num)
	{
		return Vector(x/num, y/num, z/num);		
	}

	// return the minimum vector
	Vector minVec(Vector& vec)
	{	
		using namespace std;
		return Vector(min<float>(x,vec.x),min<float>(y,vec.y),min<float>(z,vec.z));
	}
	// return  the maximum vector
	Vector maxVec(Vector& vec)
	{
		using namespace std;
		return Vector(max<float>(x,vec.x),max<float>(y,vec.y),max<float>(z,vec.z));
	}
	// return the length of a vector
	float length()
	{
		return sqrtf(x * x + y * y + z * z);
	}

	void normalize()
	{
		float magnitude = length();
		x /= magnitude;
		y /= magnitude;
		z /= magnitude;
	}
	// dot product of vectors
	float dot(const Vector& vec) const
	{
		return x * vec.x + y * vec.y + z * vec.z;
	}
	// cross product of vectors
	Vector cross(const Vector& vec)
	{
		return Vector(	(y*vec.z)-(vec.y*z),-(x*vec.z)+(vec.x*z),(x*vec.y)-(y*vec.x));
	}

};

class Point: public Vector
{
public:
	Vector normal;
	float radius;

	// default constructor and other constructors
	Point()
	{
		radius=0;
	}
	Point(float x1, float y1, float z1)
	{
		x = x1;
		y = y1;
		z = z1;
		radius=0;
	}

	Point(Vector& vec)
	{
		x = vec.x;
		y = vec.y;
		z = vec.z;
		radius = 0;
	}

	void setNormal(const Vector& nor)
	{
		normal = nor;
	}

	void calculateNormal(Point& p1, Point& p2, Point& p3)
	{
		Vector Normal;
		using namespace std;
		Vector temp1 = p1 - p2;
		Vector temp2 = p1 - p3;
		Normal = temp2.cross(temp1);
		//p1.normal = Normal;

		//Normal.normalize();
		//v1.normal.normalize();
		//v2.normal.normalize();
		//v3.normal.normalize();

		//cout<<"New Normal = "<<Normal.x<<" , "<<Normal.y <<" , "<<Normal.z<<endl;

		//cout<<"Old Normal = "<<p1.normal.x<<" , "<<p1.normal.y <<" , "<<p1.z<<endl;
		p1.normal += Normal;
		//cout<<"Resultant Normal = "<<p1.normal.x<<" , "<<p1.normal.y <<" , "<<p1.z<<endl;

		//cout<<"Old Normal = "<<p2.normal.x<<" , "<<p2.normal.y <<" , "<<p2.normal.z<<endl;
		p2.normal += Normal;
		//cout<<"Resultant Normal = "<<p2.normal.x<<" , "<<p2.normal.y <<" , "<<p2.normal.z<<endl;

		//cout<<"Old Normal = "<<p3.normal.x<<" , "<<p3.normal.y <<" , "<<p3.normal.z<<endl;
		p3.normal += Normal;
		//cout<<"Resultant Normal = "<<p3.normal.x<<" , "<<p3.normal.y <<" , "<<p3.normal.z<<endl;
		
		//p2.normal += Normal;
		//p3.normal += Normal;

	}
	void calculateRadius(Point& p1, Point& p2, Point& p3)
	{
		float temp = (p1 - p2).length();		
		if(temp > p1.radius)
			p1.radius = (float)temp;
		if(temp > p2.radius)
			p2.radius = (float)temp;

		temp = (p3 - p2).length();
		if(temp > p2.radius)
			p2.radius = (float)temp;
		if(temp > p3.radius)
			p3.radius = (float)temp;

		temp = (p1 - p3).length();
		if(temp > p1.radius)
			p1.radius = (float)temp;
		if(temp > p3.radius)
			p3.radius = (float)temp;		
	}

};
#endif