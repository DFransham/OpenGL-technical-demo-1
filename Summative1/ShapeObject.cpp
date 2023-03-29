#include "ShapeObject.h"

//Constructor - creates default shape of the type given.
cShapeObject::cShapeObject(EShapeType _eShapeChoice)
{
	mShapeType = _eShapeChoice;
	miVertexArraySize = 8 * static_cast<int>(mShapeType);

	switch (mShapeType)
	{
	case EShapeType::S_TRIANGLE:
		miIndexArraySize = 3;

		mpIndices = new GLuint[miIndexArraySize];
		for (int i = 0; i < miIndexArraySize; i++)
		{
			mpIndices[i] = DefaultTriIndices[i];
		}

		mpVertices = new GLfloat[miVertexArraySize];
		for (int i = 0; i < miVertexArraySize; i++)
		{
			mpVertices[i] = DefaultTriVertices[i];
		}

		break;

	case EShapeType::S_QUAD:
		miIndexArraySize = 6;

		mpIndices = new GLuint[miIndexArraySize];
		for (int i = 0; i < miIndexArraySize; i++)
		{
			mpIndices[i] = DefaultQuadIndices[i];
		}

		mpVertices = new GLfloat[miVertexArraySize];
		for (int i = 0; i < miVertexArraySize; i++)
		{
			mpVertices[i] = DefaultQuadVertices[i];
		}

		break;

	case EShapeType::S_HEX:
		miIndexArraySize = 12;

		mpIndices = new GLuint[miIndexArraySize];
		for (int i = 0; i < miIndexArraySize; i++)
		{
			mpIndices[i] = DefaultHexIndices[i];
		}

		mpVertices = new GLfloat[miVertexArraySize];
		for (int i = 0; i < miVertexArraySize; i++)
		{
			mpVertices[i] = DefaultHexVertices[i];
		}

		break;
	default:
		std::cout << "We added extra shapes to the code without updating the cShapes() constructor switch block.  Silly us.";
		break;
	}

	UpdateObjectModelMat(true, true, true);
}

//Copies values from given array to vertex array to modify vertices.
void cShapeObject::ChangeVertexArray(GLfloat* _NewVertArray, int _NewArrSize)
{
	for (int i = 0; i < _NewArrSize; i++)
	{
		mpVertices[i] = _NewVertArray[i];
	}
}

//Destructor - deletes vertex and index array pointers.
cShapeObject::~cShapeObject()
{
	delete mpVertices;
	delete mpIndices;
	mpVertices = nullptr;
	mpIndices = nullptr;
}

//Calculates translation matrix, scale matrix, and rotation matrix if required then updates model matrix
//Default for Translation is false, Scale is false, Rotation is true
//This is primarily because I haven't implemented any automated translation or scaling at this stage.
void cShapeObject::UpdateObjectModelMat(bool _Translate, bool _Scale, bool _Rotate)
{
	mTranslationMat = glm::translate(glm::mat4(), mObjPosition);
	mScaleMat = glm::scale(glm::mat4(), mObjScale);
	mRotationMat = glm::rotate(glm::mat4(), glm::radians(mObjRotationAngle), glm::vec3(0.0f, 0.0f, 1.0f));
	mObjectModelMat = mTranslationMat * mRotationMat * mScaleMat;
}

//Binds given VAO, VBO, EBO and sets the vertex attributes based on member variables.
void cShapeObject::SetVertexAttributes(GLuint _VAO, GLuint _EBO, GLuint _VBO, GLenum _StaticOrDynamic)
{
	glBindVertexArray(_VAO);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * miIndexArraySize, mpIndices, _StaticOrDynamic);

	glBindBuffer(GL_ARRAY_BUFFER, _VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * miVertexArraySize, mpVertices, _StaticOrDynamic);

	//Set the Vertex Attribute information (how to interpret the vertex data)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
