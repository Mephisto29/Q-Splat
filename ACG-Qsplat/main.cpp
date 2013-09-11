/*
 * QSplat
 *
 * main.cpp file
 *
 * Author : Schalk Jordaan
 * 			JRDSCH001
 */

//Different includes

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
#include "BoundingVolume.h"

#include <windows.h>
#include <GL/gl.h>
#include <GL/glut.h>

//#include "vector.h"

using namespace std;
// using namespace to avoid sdt:: every where


//setup window info
const int WINDOW_HEIGHT = 900;
const int WINDOW_WIDTH = 1600;



//Transformation and rotation variables: ==========================================

//variables for the rotation and translation
float RotX = 0.0f;
float RotY = 0.0f;
float RotZ = 0.0f;

float TransX = 0.0f;
float TransY = 0.0f;
float TransZ = 0.0f;

//Camera variables: =============================================================
//camera variables to be used
float Cx = 0.0f;
float Cy = 0.0f;
float Cz = 0.0f;
float CRx = 0.0f;
float CRy = 0.0f;
float Zoom = -5;

float pi = 3.141592654;

//boolean checks for axis transformations
bool RotCheckX = false;
bool RotCheckY = false;
bool RotCheckZ = false;
bool TranslationX = false;
bool TranslationY = false;
bool TranslationZ = false;

float Windowidth;
float Windowheight;


//booleans to differentiate between translation and rotation
bool rotation = false;
bool translation = true;

int numVertex = 0;
int numFaces = 0;

int drawDepth = 0;
int drawnSplats = 0;

int mousePosX, mousePosY;
int oldMousePosX, oldMousePosY;
bool mouseClicked = false;

bool backFaceCulling = false;
bool DrawTree = false;
bool DrawPoints = true;

Vector camPos;
Vector modelPos;
Vector modelMin;
Vector modelMax;
// Stores coordinates for points
vector<Point> vertexes;
//Stores combination of points for triangles
vector<Point> points;
BoundingVolumeNode* tree = new BoundingVolumeNode();
	//vector<Vector3> vertices;//= new <Vector3>vector();
	//vector<Vector3> connections;


//reset translations and rotations to initial positions
void Reset()
{
	RotX = 0.0f;
	RotY = 0.0f;
	RotZ = 0.0f;

	TransX = 0.0f;
	TransY = 0.0f;
	TransZ = 0.0f;

	RotCheckX = false;
	RotCheckY = false;
	RotCheckZ = false;
	TranslationX = false;
	TranslationY = false;
	TranslationZ = false;

	rotation = false;
	translation = true;

}

//reset camera to initial position
void resetCamera()
{
	Cx = 0;
	Cy = 0;
	Cz = 0;
	CRx = 0;
	CRy = 0;
}

//================================================================================




//Iput handeling : =================================================================

//if key is pressed
void keyPress(unsigned char key, int x, int y)
{
	if(key=='r' || key == 'R')
	{
		resetCamera();
	}
	if(key =='q' || key == 'Q')
	{
		Reset();
	}
	if(key == '+'){
		Zoom += 0.1;
	}
	if(key == '-'){
		Zoom -= 0.1;
	}
	if(key == 't' || key == 'T')
	{
		DrawTree = true;
		DrawPoints = false;
	}
	if(key == 'p' || key == 'P')
	{
		DrawPoints = true;
		DrawTree = false;
	}
	if(key == 'b' || key == 'B')
	{
		if(backFaceCulling)
			backFaceCulling = false;
		else
			backFaceCulling = true;
	}
	if(key == '8')
	{
		drawDepth++;
	}
	if(key == '2')
	{
		drawDepth--;
	}
}
//===================================================================================




//Methods for drawing:============================================================

// edit the point of view of the camera
void camera()
{
	//move 7 units back
	glTranslatef(0,0,Zoom);
	//rotations of the camera
	glRotatef(CRx, 1.0f,0.0f,0.0f);
	glRotatef(CRy, 0.0f,1.0f,0.0f);
	//translations of the camera
	glTranslatef(-Cx, -Cy, -Cz);

	float radianCrx = CRx/180*pi;
	float radianCry = CRy/180*pi;

	camPos.x=sin(radianCry)*cos(radianCrx)*Zoom;
	camPos.z=sin(radianCry)*sin(radianCrx)*Zoom;
	camPos.y=cos(radianCry)*Zoom;




}

//draw text on screen
void Text(const char * text, int length, int x, int y)
{
	glMatrixMode(GL_PROJECTION);
	// lol is a matrix array
	double *lol = new double[16];
	glGetDoublev(GL_PROJECTION_MATRIX, lol);
	glLoadIdentity();
	glOrtho(0,800,0,600,-5,5);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glPushMatrix();
	glLoadIdentity();
	glRasterPos2i(x,y);

	for(int a = 0;a < length;++a)
		glutBitmapCharacter(GLUT_BITMAP_9_BY_15, (int)text[a]);

	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixd(lol);
	glMatrixMode(GL_MODELVIEW);
}

string getState(bool state)
{
	if(state)
		return "true";
	else
		return "false";
}

void mouseClick(int button, int state, int x, int y){
	using namespace std;
	if(button==0)
		if(state==0)
			mouseClicked=true;
		else
			mouseClicked=false;	
}

void mouseDrag(int x, int y){
	using namespace std;
	
	mousePosX=x;
	mousePosY=y;
		
	short inc = 3.0f;
	short Xinc = inc;
	short Yinc = -inc;
	
	
	if(mousePosX<oldMousePosX)
		CRy-= Xinc;
	if(mousePosX>oldMousePosX)
		CRy+= Xinc;
	if(mousePosY<oldMousePosY)
		CRx+= Yinc;
	if(mousePosY>oldMousePosY)
		CRx-= Yinc;


	oldMousePosX=mousePosX;
	oldMousePosY=mousePosY;
}

void drawBox(Vector min, Vector max)
{
	glBegin(GL_LINES);
		glColor3f(0.0, 1.0, 1.0);

	glVertex3d(min.x,min.y,min.z);
	glVertex3d(min.x,min.y,max.z);

	glVertex3d(max.x,min.y,max.z);
	glVertex3d(min.x,min.y,max.z);

	glVertex3d(min.x,min.y,min.z);
	glVertex3d(max.x,min.y,min.z);
	
	glVertex3d(max.x,min.y,min.z);
	glVertex3d(max.x,min.y,max.z);

	glVertex3d(min.x,min.y,min.z);
	glVertex3d(min.x,max.y,min.z);

	glVertex3d(max.x,min.y,min.z);
	glVertex3d(max.x,max.y,min.z);

	glVertex3d(min.x,min.y,max.z);
	glVertex3d(min.x,max.y,max.z);

	glVertex3d(max.x,min.y,max.z);
	glVertex3d(max.x,max.y,max.z);

	glVertex3d(min.x,max.y,min.z);
	glVertex3d(min.x,max.y,max.z);

	glVertex3d(min.x,max.y,min.z);
	glVertex3d(max.x,max.y,min.z);

	glVertex3d(max.x,max.y,min.z);
	glVertex3d(max.x,max.y,max.z);

	glVertex3d(max.x,max.y,max.z);
	glVertex3d(min.x,max.y,max.z);

	glEnd();
}

void drawTree(BoundingVolumeNode* tree)
{
	if(tree->depth <= drawDepth)
	{
		if(((tree->leaf == true) && tree->empty != true) || ((tree->depth == drawDepth) && (tree->empty != true)))// || ((tree->left->empty || tree->left->leaf)&& (tree->right->empty || tree->right->leaf)))
		{
			Vector eyeVec(tree->centre.x - camPos.x, tree->centre.y - camPos.y, tree->centre.z - camPos.z);
			float eyedist = eyeVec.length();

			float pointsize = tree->radius*Windowidth/eyedist;

			glPointSize(pointsize);
			
			Vector lightDir(-1,-1,-1);
			Vector diffuseColor(0.6,0.5,0.4);

			tree->normal.normalize();

			float diffuseImpact = lightDir.dot(tree->normal);

			if(diffuseImpact < 0)
				diffuseImpact = 0;

			Vector final = diffuseColor*diffuseImpact;
			
			glColor3f(sqrt(pow(final.x,2)), sqrt(pow(final.y,2)), sqrt(pow(final.z,2)));

			glBegin(GL_POINTS);
				glVertex3f(tree->centre.x, tree->centre.y, tree->centre.z);
			glEnd();
		}
		else if(tree->empty)
		{}
		else
		{
			drawTree(tree->left);
			drawTree(tree->right);
		}
	}
}

void drawPoints()
{
	drawnSplats = 0;
	for(int i = 0; i < vertexes.size() ; i++)
	{
		if(!backFaceCulling)
		{
			Vector eyeVec(vertexes.at(i).x - camPos.x, vertexes.at(i).y - camPos.y, vertexes.at(i).z - camPos.z);
			float eyedist = eyeVec.length();

			float pointsize = vertexes.at(i).radius*2*Windowidth/eyedist;

			glPointSize(pointsize);
			
			Vector lightDir(-1,-1,-1);
			Vector diffuseColor(0.6,0.5,0.4);

			vertexes.at(i).normal.normalize();

			float diffuseImpact = lightDir.dot(vertexes.at(i).normal);

			if(diffuseImpact < 0)
				diffuseImpact = 0;

			Vector final = diffuseColor*diffuseImpact;

			glColor3f(sqrt(pow(final.x,2)), sqrt(pow(final.y,2)), sqrt(pow(final.z,2)));

			glBegin(GL_POINTS);
				glVertex3f(vertexes.at(i).x, vertexes.at(i).y, vertexes.at(i).z);
			glEnd();

			drawnSplats++;
		}
		else
		{
			//cout << "AM CULLING "<<endl;
			Vector eyeVec(vertexes.at(i).x - camPos.x, vertexes.at(i).y - camPos.y, vertexes.at(i).z - camPos.z);
			float cull = eyeVec.dot(vertexes.at(i).normal);

			if(cull <= 0)
			{
				Vector eyeVec(vertexes.at(i).x - camPos.x, vertexes.at(i).y - camPos.y, vertexes.at(i).z - camPos.z);
				float eyedist = eyeVec.length();

				float pointsize = vertexes.at(i).radius*2*Windowidth/eyedist;

				glPointSize(pointsize);
				
				Vector lightDir(-1,-1,-1);
				Vector diffuseColor(0.6,0.5,0.4);

				vertexes.at(i).normal.normalize();

				float diffuseImpact = lightDir.dot(vertexes.at(i).normal);

				if(diffuseImpact < 0)
					diffuseImpact = 0;

				Vector final = diffuseColor*diffuseImpact;

				glColor3f(sqrt(pow(final.x,2)), sqrt(pow(final.y,2)), sqrt(pow(final.z,2)));

				glBegin(GL_POINTS);
					glVertex3f(vertexes.at(i).x, vertexes.at(i).y, vertexes.at(i).z);
				glEnd();

				drawnSplats++;
			}
		}
	}
}

// Method for displaying in the window
void drawDisplay()
{
	// check for a pressed key.
	
	string Result;          // string which will contain the result

	ostringstream convert;   // stream used for the conversion

	string text = "";
	string text2 = "";
	string text3 = "";

	convert << drawDepth;
	Result = convert.str();

	//write text to the window
	text = "Point: " + getState(DrawPoints) +  " Tree " + getState(DrawTree);
	text2 = "Depth: " + Result +  " Culling: " + getState(backFaceCulling);

	

	convert << drawnSplats;      // insert the textual representation of 'Number' in the characters in the stream

	Result = convert.str();

	text3 = "Drawn Splats : " + Result;

	//Clear the window Background
	glClearColor(0,0,0,1);

	//Clear the buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	drawBox(modelMin,modelMax);


	//Draw the axis
	//X axis
	glBegin(GL_LINES);
		glColor3f(0.0, 0.0, 1.0);
		glVertex3d(50,0,0);
		glVertex3d(-50,0,0);
		glColor3f(1.0, 1.0, 1.0);

		glColor3f(1.0, 0.0, 0.0);
		glVertex3d(0,0,50);
		glVertex3d(0,0,-50);
		glColor3f(1.0, 1.0, 1.0);

	glEnd();
	
	//Y axis
	glBegin(GL_LINE);
		
	glEnd();
	
	//Z axis
	glBegin(GL_LINE);
		
	glEnd();


	//Draw the window text
	Text(text.data(), text.size(),0,0);
	Text(text2.data(), text2.size(),0,25);
	if(DrawPoints)
		Text(text3.data(), text3.size(),0,50);

	////Load Identity matrix(resets drawing location)
	glLoadIdentity();

	//reconfigure camera position for display
	camera();

	//translate according to movement
	glTranslatef(TransX, TransY,TransZ);

	//rotate in the x direction
	glRotatef(RotX, 1.0f, 0.0f, 0.0f);

	//rotate in the y direction
	glRotatef(RotY, 0.0f, 1.0f, 0.0f);

	//rotate in the z direction
	glRotatef(RotZ, 0.0f, 0.0f, 1.0f);

	//Draw the primative Object to draw (Teapot)
	//glutWireTeapot(2.0f);

	drawnSplats = 0;
	//drawPoints();
	if(DrawTree)
		drawTree(tree);
	else if(DrawPoints)
		drawPoints();
	

	//flush the buffer to the window
	glFlush();
}

// method to redraw window content
void drawReshape(int width, int height)
{
	Windowidth = width;
	Windowheight = height;

	//Set vieport size to window size
	glViewport(0,0,(GLsizei) width,(GLsizei) height);

	//switch to the projection matrix to change view
	glMatrixMode(GL_PROJECTION);

	//Load the identity matrix
	glLoadIdentity();

	//set perspective (angle(in degrees),
	//				  (the aspect ratio of the window),
	//				  (new plane),
	//                (far plane)
	gluPerspective(60, (GLfloat) width/(GLfloat) height,1.0, 100);

	//Set matrix to modelview to draw shapes
	glMatrixMode(GL_MODELVIEW);
}



//=====================================================================================


// Main method



int main(int argc, char** argv)
{

	string newLine = "";
	string word = "";



	int numVertex = 0;
	int numFaces = 0;

	float x, y, z;

	bool getVertex = false;
	bool getFaces = false;
	bool end = false;


	camPos=Vector(0, 0, Zoom);

	ofstream myoutput;
	ifstream myfile;

	myfile.open("bun0001.ply");
	myoutput.open("output.txt");

	myoutput<<"here we go"<<endl;

	if(myfile.is_open())
	{
		myoutput<< "opened file" <<endl;
		while (myfile.good())
		{
			if(end == false)
			{
				getline(myfile,newLine);

				istringstream line(newLine);
				line >> word;

				if(word == "end_header")
				{
					end = true;
				}
				else
				{
					if((getVertex == false) && (word == "element"))
					{
						line >> word;
						line >> numVertex;
						getVertex = true;
					}
					else if((getFaces == false) &&(word == "element"))
					{
						line >> word;
						line >> numFaces;
						getFaces = true;
					}
				}
			}
			else
			{
				if(numVertex != 0)
				{
					
					for(int i = 0 ; i < numVertex ; i ++)
					{
						getline(myfile,newLine);
						istringstream line(newLine);

						line >> x >> y >> z;

						//vertices.push_back(Vector3(x,y,z));
						vertexes.push_back(Vector(x*10,y*10,z*10));
					}
				}
				myoutput<<"vertexes : "<<vertexes.size()<<endl;
				if(numFaces != 0)
				{
					for(int i = 0 ; i < numFaces ; i ++)
					{
						getline(myfile,newLine);
						istringstream line(newLine);

						line >> x >> x >> y >> z;
						
						points.push_back(Vector(x,y,z));

						// =================================================
						// Set max length of connections
						vertexes.at(x).calculateRadius(vertexes.at(x),vertexes.at(y),vertexes.at(z));

						//==================================================
						// Set Vertex normal.
						points.at(i).calculateNormal(vertexes.at(x),vertexes.at(y),vertexes.at(z));

						
					}
					myoutput<<"Faces : "<<points.size()<<endl;
				}
				//vertices.clear();
			}

			//myoutput<< newLine <<endl;
		}

		myfile.close();
		
	}
	else
		myoutput<< "unable to open the file" << endl;

	
	for(int i = 0 ; i < numVertex ; i ++)
	{
		//myoutput<< "print face" << endl;
		//myoutput<<vertices.at(i).x <<", " << vertices.at(i).y <<", " << vertices.at(i).z <<endl;
		myoutput<<vertexes.at(i).x <<", " << vertexes.at(i).y <<", " << vertexes.at(i).z <<endl;
	}
	
	for(int i = 0 ; i < numFaces ; i ++)
	{
		//myoutput<< "print face" << endl;
		//myoutput<<connections.at(i).x <<", " << connections.at(i).y <<", " << connections.at(i).z <<endl;
		myoutput<<points.at(i).x <<", " << points.at(i).y <<", " << points.at(i).z <<endl;
	}

	

	modelMin = vertexes.at(0);
	modelMax = vertexes.at(0);

	//return 0;
	for(int i = 0 ; i < numVertex ; i ++)
	{
		modelMin = modelMin.minVec(vertexes.at(i));
		modelMax = modelMax.maxVec(vertexes.at(i));
	}

	modelPos = (modelMin + modelMax)/2;
	camPos = Vector(modelPos.x,modelPos.y,modelPos.z -Zoom);

	//vector<Point>::iterator& start, vector<Point>::iterator& end, Vector newMin, Vector newMax, int depth, vector<Point>& newVertices
	myoutput<<"Starting tree build"<<endl;
	cout<<tree->radius<<endl;
	tree = tree->buildTree(0,numVertex, modelMin, modelMax, 0, vertexes);
	cout<< tree->radius << endl;
	myoutput<<"build done"<<endl;


	myoutput.close();

	//Initialise window
	glutInit(&argc, argv);

	//set title of window
	glutCreateWindow("Qsplat - JRDSCH001");

	//set window size
	glutInitWindowSize(1600, 900);

	//set screen position of window
	glutInitWindowPosition(0,0);

	//set window buffer to single
	glutInitDisplayMode( GLUT_DEPTH | GLUT_SINGLE);
	glEnable(GL_DEPTH_TEST);


	//set where to get input changes
	glutKeyboardFunc(keyPress);
	//glutKeyboardUpFunc(keyRelease);
	glutMouseFunc(mouseClick);
	glutMotionFunc(mouseDrag);

	//set how to update display
	glutDisplayFunc(drawDisplay);
	glutIdleFunc(drawDisplay);
	glutReshapeFunc(drawReshape);

	//start the display loop
	glutMainLoop();

}


