/*
 * Lab-01_students.c
 *
 *     This program draws straight lines connecting dots placed with mouse clicks.
 *
 * Usage:
 *   Left click to place a control point.
 *		Maximum number of control points allowed is currently set at 64.
 *	 Press "f" to remove the first control point
 *	 Press "l" to remove the last control point.
 *	 Press escape to exit.
 */


#include <iostream>
#include "ShaderMaker.h"
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <vector>

 // Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>


static unsigned int programId;

unsigned int VAO;
unsigned int VBO;

unsigned int VAO_2;
unsigned int VBO_2; 

using namespace glm;
using namespace std;

#define MaxNumPts 300
float PointArray[MaxNumPts][2];
float CurveArray[MaxNumPts][2];

int NumPts = 0;

// Window size in pixels
int		width = 500;
int		height = 500;

/* Prototypes */
void addNewPoint(float x, float y);
int main(int argc, char** argv);
void removeFirstPoint();
void removeLastPoint();

//epsilon value
float eps = 0.05f;

// Currently selected control point
int selectedPoint = -1;

//Tolleranza Planarietà
float tol_planarita = 0.01f;

//various modalities
enum Behavior { DefaultMode, CatmullRom, Adattivo, Continuity };

Behavior behavior = DefaultMode;

//Continuity Behavior
enum ContinuityBehavior { C0, C1, G1 };

ContinuityBehavior continuityType = C0;

//resolution (number of segments)
int resolution;

void myKeyboardFunc(unsigned char key, int x, int y)
{
	switch (key) {
	case 'f':
		removeFirstPoint();

		break;
	case 'l':
		removeLastPoint();

		break;
	case 27:			// Escape key
		exit(0);
		break;
	case 'n':
		behavior = DefaultMode;

		break;
	case 'i':
		behavior = CatmullRom;

		break;
	case 'a':
		behavior = Adattivo;

		break;
	case 'c':
		behavior = Continuity;

		break;
	case '0':
		continuityType = C0;

		break;
	case '1':
		continuityType = C1;

		break;
	case 'g':
		continuityType = G1;

		break;
	}


}
void removeFirstPoint() {
	int i;
	if (NumPts > 0) {
		// Remove the first point, slide the rest down
		NumPts--;
		for (i = 0; i < NumPts; i++) {
			PointArray[i][0] = PointArray[i + 1][0];
			PointArray[i][1] = PointArray[i + 1][1];
		}
	}
}
void resizeWindow(int w, int h)
{
	height = (h > 1) ? h : 2;
	width = (w > 1) ? w : 2;
	gluOrtho2D(-1.0f, 1.0f, -1.0f, 1.0f);
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
}

// this function check if a point is close to a drawn point.
void NearestPoint(vec2 point) {
	int nearestIndex = -1;
	float nearestDistance = 3; // 3 is just a constant greater than 2*sqrt(2) (max distace b/w points)
	for (int i = 0; i < NumPts; i++) {
		vec2 p(PointArray[i][0], PointArray[i][1]);
		float dist = distance(point, p);
		if (dist < eps && dist < nearestDistance) {
			nearestIndex = i;
			nearestDistance = dist;
		}
	}
	//select the nearest point (-1 is no point is close enough)
	selectedPoint = nearestIndex;
}

// Left button presses place a new control point.
void myMouseFunc(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		float xPos = -1.0f + ((float)(x) * 2 / (float)(width));
		float yPos = -1.0f + ((float)(height - y)) * 2 / ((float)(height));

		//if no point is close, add the new point
		NearestPoint(vec2(xPos, yPos));
		if (selectedPoint == -1) {
			addNewPoint(xPos, yPos); //(floor(xPos*10)/10, floor(yPos*10)/10);

		}
	}
	else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
		// Deselect the current point
		selectedPoint = -1;
	}
}

void motion(int x, int y) {
	float xPos = -1.0f + ((float)x) * 2 / ((float)(width));
	float yPos = -1.0f + ((float)(height - y)) * 2 / ((float)(height));
	if (selectedPoint != -1) {
		PointArray[selectedPoint][0] = xPos;
		PointArray[selectedPoint][1] = yPos;
	}
}
// Add a new point to the end of the list.  
// Remove the first point in the list if too many points.
void removeLastPoint() {
	if (NumPts > 0) {
		NumPts--;
	}
}

// Add a new point to the end of the list.  
// Remove the first point in the list if too many points.
void addNewPoint(float x, float y) {
	if (NumPts >= MaxNumPts) {
		removeFirstPoint();
	}
	PointArray[NumPts][0] = x;
	PointArray[NumPts][1] = y;
	NumPts++;
}

void initShader(void)
{
	GLenum ErrorCheckValue = glGetError();

	char* vertexShader = (char*)"vertexShader.glsl";
	char* fragmentShader = (char*)"fragmentShader.glsl";

	programId = ShaderMaker::createProgram(vertexShader, fragmentShader);
	glUseProgram(programId);

}

// DeCasteljau algoritm.

vec2 deCasteljau(float Array[][2], float t, int NumPts, float fArray[][2] = nullptr, float sArray[][2] = nullptr) {
	if (fArray != nullptr) {
		fArray[0][0] = Array[0][0]; fArray[0][1] = Array[0][1];
		sArray[NumPts - 1][0] = Array[NumPts - 1][0]; sArray[NumPts - 1][1] = Array[NumPts - 1][1];
	}
	if (NumPts > 1) {
        for (int i = 0; i < NumPts - 1; i++) {
            for (int j = 0; j < NumPts - i - 1; j++) {
                Array[j][0] = (float)(1 - t) * Array[j][0] + (float)t * Array[j + 1][0];
                Array[j][1] = (float)(1 - t) * Array[j][1] + (float)t * Array[j + 1][1];
            }
            if (fArray != nullptr) {
                fArray[i+1][0] = Array[0][0];
                fArray[i+1][1] = Array[0][1];
                sArray[NumPts - i - 2][0] = Array[NumPts - i - 2][0];
                sArray[NumPts - i - 2][1] = Array[NumPts - i - 2][1];
            }
        }
    }
    return vec2(Array[0][0], Array[0][1]);
}

float distancePointLine(vec2 P, vec2 A, vec2 B) {
	vec2 AB = B - A;
	vec2 AP = P - A;
	vec2 proj_AP_AB = dot(AP, AB) / dot(AB, AB) * AB;
	vec2 dist_vec = AP - proj_AP_AB;
	return length(dist_vec);
}

void defaultModality(float tempArray[][2], int NumPts) {
	for (int t = 0; t < resolution; t++) {
		vec2 splinePoint = deCasteljau(tempArray, (float)t / resolution, NumPts);
		CurveArray[t][0] = splinePoint.x;
		CurveArray[t][1] = splinePoint.y;
	}
}

void suddivisioneAdattiva(float Array[][2], int NumPts)
{
	// Estrai Control point esterni: 
	vec2 P1(
		Array[0][0],
		Array[0][1]);

	vec2 P2(
		Array[NumPts - 1][0],
		Array[NumPts - 1][1]);

	// Test di Planarità sui Control Point Interni
	int test_planarita = 1;
	for (int i = 1; i < NumPts - 1; i++)
	{
		// Calcolo distanza tempArray[i] dalla retta
		vec2 P(
			Array[i][0],
			Array[i][1]);

		if (distancePointLine(P, P1, P2) > tol_planarita) test_planarita = 0;
	}
	if (test_planarita == 1) {
		//Disegna segmento tra control point estremi tempArray[0] , tempArray[NumPts-1]
		CurveArray[resolution][0] = P1.x;		CurveArray[resolution][1] = P1.y;
		CurveArray[resolution + 1][0] = P2.x;	CurveArray[resolution + 1][1] = P2.y;
		resolution = resolution + 1; //sovrascrive punti uguali
		return;
	}
	else {
		// Applica deCasteljau nel parametro t=0.5 salvando i CP delle due nuove curve
		float firstArray[MaxNumPts][2];
		float secondArray[MaxNumPts][2];

		deCasteljau(Array, 0.5f, NumPts, firstArray, secondArray);

		suddivisioneAdattiva(firstArray, NumPts);
		suddivisioneAdattiva(secondArray, NumPts);
	}
	return;
}

void catmullRom(float Array[MaxNumPts][2], int NumPts) {
	//to get an efficient drawing I'll use the suddivisioneAdattiva Function to draw the splines

	if (NumPts == 2) {
		suddivisioneAdattiva(Array, NumPts);
		return;
	}
	float Points[MaxNumPts][2];

	//Case P[0] (spline with that interpolates points 0 & 1)
	vec2 P0(Array[0][0], Array[0][1]);
	vec2 P1(Array[1][0], Array[1][1]);
	vec2 P0p = P0 + (P1 - P0) / 3.0f;
	vec2 P2(Array[2][0], Array[2][1]);
	vec2 P1m = P1 - (P2 - P0) / 6.0f;

	Points[0][0] = P0.x;	Points[0][1] = P0.y;
	Points[1][0] = P0p.x;	Points[1][1] = P0p.y;
	Points[2][0] = P1m.x;	Points[2][1] = P1m.y;
	Points[3][0] = P1.x;	Points[3][1] = P1.y;
	suddivisioneAdattiva(Points, 4);

	//other cases
	for (int i = 1; i < NumPts - 2; i++) {
		vec2 Pim1(Array[i - 1][0], Array[i - 1][1]);
		vec2 Pip1(Array[i + 1][0], Array[i + 1][1]);
		vec2 Pip2(Array[i + 2][0], Array[i + 2][1]);
		vec2 Pi(Array[i][0], Array[i][1]);
		vec2 Pip = Pi + (Pip1 - Pim1) / 6.0f;
		vec2 Pip1m = Pip1 - (Pip2 - Pi) / 6.0f;

		Points[0][0] = Pi.x;	Points[0][1] = Pi.y;
		Points[1][0] = Pip.x;	Points[1][1] = Pip.y;
		Points[2][0] = Pip1m.x;	Points[2][1] = Pip1m.y;
		Points[3][0] = Pip1.x;	Points[3][1] = Pip1.y;
		suddivisioneAdattiva(Points, 4);
	}

	//case P[NumPts] (spline with that interpolates point NumPts - 2 & NumPts - 1)
	vec2 PNm2(Array[NumPts - 3][0], Array[NumPts - 3][1]);
	vec2 PNm1(Array[NumPts - 2][0], Array[NumPts - 2][1]);
	vec2 PN(Array[NumPts - 1][0], Array[NumPts - 1][1]);
	vec2 PNm1p = PNm1 + (PN - PNm2) / 6.0f;
	vec2 PNm = PN - (PN - PNm1) / 3.0f;

	Points[0][0] = PNm1.x;	Points[0][1] = PNm1.y;
	Points[1][0] = PNm1p.x;	Points[1][1] = PNm1p.y;
	Points[2][0] = PNm.x;	Points[2][1] = PNm.y;
	Points[3][0] = PN.x;	Points[3][1] = PN.y;
	suddivisioneAdattiva(Points, 4);
}

void continuity(float Array[MaxNumPts][2], int NumPts) {
	float Points[MaxNumPts][2];

	if (continuityType == C1) {
		//have to modify points in index 4, 7, 10
		for (int i = 1; i < NumPts / 2; i++) {
			PointArray[1 + 3 * i][0] = 2 * PointArray[3 * i][0] - PointArray[3 * i - 1][0];
			PointArray[1 + 3 * i][1] = 2 * PointArray[3 * i][1] - PointArray[3 * i - 1][1];
		}
	}

	if (continuityType == G1) {
		//have to modify points in index 4, 7, 10
		for (int i = 1; i < NumPts / 2; i++) {
			vec2 centralPoint = vec2(PointArray[3 * i][0], PointArray[3 * i][1]);
			vec2 backPoint = vec2(PointArray[3 * i - 1][0], PointArray[3 * i - 1][1]);
			vec2 Point = vec2(PointArray[3 * i + 1][0], PointArray[3 * i + 1][1]);
			vec2 line = normalize(centralPoint - backPoint);
			vec2 newPoint = centralPoint + line * dot(Point - centralPoint, line);
			PointArray[1 + 3 * i][0] = newPoint.x;
			PointArray[1 + 3 * i][1] = newPoint.y;
		}
	}

	if (NumPts > 3) {
		Points[0][0] = Array[0][0];	Points[0][1] = Array[0][1];
		Points[1][0] = Array[1][0];	Points[1][1] = Array[1][1];
		Points[2][0] = Array[2][0];	Points[2][1] = Array[2][1];
		Points[3][0] = Array[3][0];	Points[3][1] = Array[3][1];
		suddivisioneAdattiva(Points, 4);
	}

	for (int i = 1; i < (NumPts - 1) / 3; i++) {
		Points[0][0] = Array[3 * i][0];	Points[0][1] = Array[3 * i][1];
		Points[1][0] = Array[3 * i + 1][0];	Points[1][1] = Array[3 * i + 1][1];
		Points[2][0] = Array[3 * i + 2][0];	Points[2][1] = Array[3 * i + 2][1];
		Points[3][0] = Array[3 * i + 3][0];	Points[3][1] = Array[3 * i + 3][1];
		suddivisioneAdattiva(Points, 4);
	}

}

void init(void)
{
	// VAO for control polygon
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// VAO for curve
	glGenVertexArrays(1, &VAO_2);
	glBindVertexArray(VAO_2);
	glGenBuffers(1, &VBO_2);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_2);

	// Background color
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glViewport(0, 0, 500, 500);
}

void drawArrayData() {
	glBindVertexArray(VAO_2);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(CurveArray), &CurveArray[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// Draw the line segments between CP
	glLineWidth(2.0);
	glDrawArrays(GL_LINE_STRIP, 0, resolution);

	// Draw the control points CP
	glPointSize(4.0);
	glDrawArrays(GL_POINTS, 0, resolution);
	glBindVertexArray(0);
}

void drawScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	float tempArray[MaxNumPts][2];
	float firstArray[MaxNumPts][2];
	if (NumPts > 1) {
		copy(&PointArray[0][0], &PointArray[0][0] + MaxNumPts * 2, &tempArray[0][0]);
		switch (behavior)
		{
		case DefaultMode:
			resolution = 50;
			defaultModality(tempArray, NumPts);
			drawArrayData();
			break;

		case Adattivo:
			resolution = 0;
			suddivisioneAdattiva(tempArray, NumPts);
			resolution++; //ultima linea
			drawArrayData();
			break;

		case CatmullRom:
			resolution = 0;
			catmullRom(tempArray, NumPts);
			resolution++; //ultima linea
			drawArrayData();
			break;

		case Continuity:
			resolution = 0;
			continuity(tempArray, NumPts);
			resolution++; //ultima linea
			drawArrayData();
			break;

		default:
			break;
		}
	}

	// Draw control polygon
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(PointArray), &PointArray[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// Draw the control points CP
	glPointSize(6.0);
	glDrawArrays(GL_POINTS, 0, NumPts);
	// Draw the line segments between CP
	glLineWidth(2.0);
	glDrawArrays(GL_LINE_STRIP, 0, NumPts);
	glBindVertexArray(0);

	glutPostRedisplay();
	glutSwapBuffers();

}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);

	glutInitContextVersion(4, 0);
	glutInitContextProfile(GLUT_CORE_PROFILE);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);

	glutInitWindowSize(width, height);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Draw curves 2D");

	glutDisplayFunc(drawScene);
	glutReshapeFunc(resizeWindow);
	glutKeyboardFunc(myKeyboardFunc);
	glutMouseFunc(myMouseFunc);
	glutMotionFunc(motion);


	glewExperimental = GL_TRUE;
	glewInit();

	initShader();
	init();

	glutMainLoop();
}
