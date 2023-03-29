//Includes
#include <glew.h>
#include <glfw3.h>
#include <iostream>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#include "ViewWindow.h"
#include "ShapeRenderer.h"
#include "SpriteDimensions.h"

using std::string;

//Funtion prototypes
void InitialSetup();
void Update();
void Render(GLFWwindow* _RenderWindow);

void InitialiseShapeRenderers();
void CreateSingleShape(int _objNum, EShapeType _ShapeToMake, GLuint _VAOetcNum, GLenum _StaticOrDynamic, EProgramNames _ProgramToUse);
void ScaleAndTranslateForProjection(int ObjNum, float _x, float _y, float _PosX, float _PosY);

void GarbageCollection();

//Global Variables
cViewWindow* MyWindow = nullptr;
cCamera* MyCamera = nullptr;
float PreviousTimeStep;

int iNumVAOs = 2;
GLuint* VAO = nullptr;
GLuint* VBO = nullptr;
GLuint* EBO = nullptr;

//Hardcoded because there is no default constructor so difficult to dynamically change array size
cShapeRenderer* MyShapes[3];	//slightly wild pointer which is unfortunate but can't see a way around it at this stage
int iNumShapes = 3;				//change this if I change the value in array above

int main()
{
	//Setup the Initial elements of the program
	InitialSetup();

	GLFWwindow* Window = MyWindow->GetWindowPtr();

	//Main loop - update and render until told otherwise
	while (glfwWindowShouldClose(Window) == false)
	{
		//Update all objects and run the processes
		Update();

		//Render all the objects
		Render(Window);
	}

	//Ensuring correct shutdown of GLFW
	glfwTerminate();
	GarbageCollection();
	return 0;
}

//Free all allocated memory to keep memory leaks in check
void GarbageCollection()
{
	delete MyWindow;
	MyWindow = nullptr;
	
	delete MyCamera;
	MyCamera = nullptr;
	
	delete[] VAO;
	VAO = nullptr;

	delete[] VBO;
	VBO = nullptr;

	delete[] EBO;
	EBO = nullptr;

	for (int i = 0; i < iNumShapes; i++)
	{
		delete MyShapes[i];
		MyShapes[i] = nullptr;
	}
}

//Setup the Initial elements of the program
void InitialSetup()
{
	int iHeight = 800;
	int iWidth = 800;
	MyWindow = new cViewWindow("David Fransham Summative 1", iWidth, iHeight);

	//Sets the stle of the polygons we'll be using
	glPolygonMode(GL_FRONT, GL_FILL); //toggle between wire frame, dots, full image

	//Turn on face culling and set to counterclockwise
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);

	cShapeRenderer::SetupRenderers();
	
	VAO = new GLuint[iNumVAOs];
	VBO = new GLuint[iNumVAOs];
	EBO = new GLuint[iNumVAOs];
		
	//Generate the VAO, EBO, VBO for a shape/shapes
	glGenVertexArrays(iNumVAOs, &VAO[0]);
	glGenBuffers(iNumVAOs, &EBO[0]);
	glGenBuffers(iNumVAOs, &VBO[0]);

	MyCamera = new cCamera(45.0f, (float)iWidth, (float)iHeight, glm::vec3(0.0f, 0.0f, 3.0f), ECameraProjectionMode::ORTHOGRAPHIC_CENTRE);

	InitialiseShapeRenderers();
	
	//Initialise the first value of the "previous" time step
	PreviousTimeStep = (float)glfwGetTime();
}

//Updates objects to be rendered next
void Update()
{
	glfwPollEvents();
	
	float CurrentTimeStep = (float)glfwGetTime();
	float DeltaTime = CurrentTimeStep - PreviousTimeStep;
	PreviousTimeStep = CurrentTimeStep;

	for (int i = 0; i < 3; i++)
	{
		//"Object".Update(DeltaTime);
		MyShapes[i]->Update(DeltaTime);
	}
	MyCamera->Update(DeltaTime);
}

//Render all the objects just updated
void Render(GLFWwindow* _RenderWindow)
{
	//always goes first in render step
	glClear(GL_COLOR_BUFFER_BIT);

	for (int i = 0; i < 3; i++)
	{
		MyShapes[i]->RenderShape();
	}
	
	//Unbind assets to prevent accidental use/modification
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glUseProgram(0);

	//always goes last in render step
	glfwSwapBuffers(_RenderWindow);
}

//Creates the shapes to be rendered and defines the variables for their behaviour
void InitialiseShapeRenderers()
{
	int ObjNum = 0;
	CreateSingleShape(ObjNum, EShapeType::S_QUAD, 0, GL_DYNAMIC_DRAW, EProgramNames::P_CLIP_ANIMATE);	//create object
	MyShapes[ObjNum]->ChangeVertexArray(BT_Vertices, BT_ArrSize, GL_DYNAMIC_DRAW);	//Replace default vertices with specific ones for animated sprite
	MyShapes[ObjNum]->SetObjRotation(0);								//Don't rotate object on load
	ScaleAndTranslateForProjection(ObjNum, 2.0f, 2.0f, 0.0f, -0.05f);	//apply scalar if orthographic projection used - also translates, giving this object unique model matrix
	MyShapes[ObjNum]->SetTexture(0, EImageNames::T_BLUETORCH);			//apply textrue
	MyShapes[ObjNum]->SetFPS(15.0f);									//set animation speed at 15 frames per second

	ObjNum = 1;
	CreateSingleShape(ObjNum, EShapeType::S_HEX, 1, GL_STATIC_DRAW, EProgramNames::P_CLIP_TEXTURE_FADE); //create object
	MyShapes[ObjNum]->SetObjRotation(0.0f);								//don't rotate on load
	ScaleAndTranslateForProjection(ObjNum, 1.25f, 1.25f, 0.7f, 0.6f);	//scale for orthographic, apply scale and translate to give unique model matrix
	MyShapes[ObjNum]->SetTexture(0, EImageNames::T_DUNLUCE_REBUILT);	//apply texture one
	MyShapes[ObjNum]->SetTexture(1, EImageNames::T_DUNLUCE_RUINS);		//apply texture two
	MyShapes[ObjNum]->SetFrameRotation(0.75f);							//set rotation degrees per animation frame
	
	ObjNum = 2;
	CreateSingleShape(ObjNum, EShapeType::S_HEX, 1, GL_STATIC_DRAW, EProgramNames::P_CLIP_TEXTURE_FADE);
	MyShapes[ObjNum]->SetObjRotation(0.0f);
	ScaleAndTranslateForProjection(ObjNum, 1.35f, 1.35f, -0.75f, -0.5f);
	MyShapes[ObjNum]->SetTexture(0, EImageNames::T_ELECTRON_FLYING);
	MyShapes[ObjNum]->SetTexture(1, EImageNames::T_ELECTRON_PAD);
	MyShapes[ObjNum]->SetFrameRotation(-1.0f);
	MyShapes[ObjNum]->SetFPS(20);
}

//multiplies object scale by a percentage of window width if orthographic projection is used.  Arbitrary amount but makes things visible.
void ScaleAndTranslateForProjection(int ObjNum, float _ScaleX, float _ScaleY, float _PosX, float _PosY)
{
	ECameraProjectionMode cameraMode = MyCamera->GetProjectionMode();
	float scalar = static_cast<float>(MyWindow->GetWindowWidth()/2.5);
	
	if(cameraMode == ECameraProjectionMode::ORTHOGRAPHIC_TOP || cameraMode == ECameraProjectionMode::ORTHOGRAPHIC_CENTRE)
	{
		MyShapes[ObjNum]->SetObjScale(glm::vec3(_ScaleX * scalar, _ScaleY * scalar, 1.0f));
		MyShapes[ObjNum]->SetObjPosition(glm::vec3(_PosX * scalar, _PosY * scalar, 0.0f));
	}
	else
	{
		MyShapes[ObjNum]->SetObjScale(glm::vec3(_ScaleX , _ScaleY , 1.0f));
		MyShapes[ObjNum]->SetObjPosition(glm::vec3(_PosX, _PosY, 0.0f));
	}
}

//Calls shape renderer to create a shape to be rendered according to the variables given.
void CreateSingleShape(int _objNum, EShapeType _ShapeToMake, GLuint _VAOetcNum, GLenum _StaticOrDynamic, EProgramNames _ProgramToUse)
{
	MyShapes[_objNum]= new cShapeRenderer(_ShapeToMake, VAO[_VAOetcNum], EBO[_VAOetcNum], VBO[_VAOetcNum], _StaticOrDynamic, _ProgramToUse, MyCamera);
}

