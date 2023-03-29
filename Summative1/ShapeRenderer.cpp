#include "ShapeRenderer.h"

//Constructor acts like a wrapper to create a shape object. Probably not the tidiest way of doing things but it works for now.
cShapeRenderer::cShapeRenderer(EShapeType _ShapeToMake, GLuint _VAO, GLuint _EBO, GLuint _VBO, GLenum _StaticOrDynamic, EProgramNames _ProgramToUse, cCamera* _Camera)
{
	meShapeType = _ShapeToMake;
	miVAO = _VAO;
	miEBO = _EBO;
	miVBO = _VBO;
	meProgramType = _ProgramToUse;
	miProgramID = Programs[static_cast<int>(meProgramType)];

	mpMyCamera = _Camera;
	mpMyShape = new cShapeObject(meShapeType);
	mpMyShape->SetVertexAttributes(miVAO, miEBO, miVBO, _StaticOrDynamic);
	miNumVertices = mpMyShape->GetIndexArraySize();
}

//I probably need to check that I don't have a memory leak, and these are the only pointers I need to remove...
cShapeRenderer::~cShapeRenderer()
{
	delete mpMyShape;
	mpMyShape = nullptr;
}

//Loads programs and textures to be ready to use for rendering.
void cShapeRenderer::SetupRenderers()
{
	LoadPrograms();
	LoadTextures();
}

//No easy loop here, each program listed with their shader types to find in the arrays etc above and load programs.
void cShapeRenderer::LoadPrograms()
{
	//Create the programs
	Programs[static_cast<int>(EProgramNames::P_FIXED_TRI)] = ShaderLoader::CreateProgram(VShaderNames[static_cast<int>(EVShaderNames::VS_FIXED_TRIANGLE)], FShaderNames[static_cast<int>(EFShaderNames::FS_FIXED_COLOR)]);
	Programs[static_cast<int>(EProgramNames::P_POSITION_ONLY)] = ShaderLoader::CreateProgram(VShaderNames[static_cast<int>(EVShaderNames::VS_POSITION_ONLY)], FShaderNames[static_cast<int>(EFShaderNames::FS_FIXED_COLOR)]);
	Programs[static_cast<int>(EProgramNames::P_VERTEX_COLOR)] = ShaderLoader::CreateProgram(VShaderNames[static_cast<int>(EVShaderNames::VS_VERTEX_COLOR)], FShaderNames[static_cast<int>(EFShaderNames::FS_VERTEX_COLOR)]);
	Programs[static_cast<int>(EProgramNames::P_COLOR_FADE)] = ShaderLoader::CreateProgram(VShaderNames[static_cast<int>(EVShaderNames::VS_VERTEX_COLOR)], FShaderNames[static_cast<int>(EFShaderNames::FS_VERTEX_COLOR_FADE)]);
	Programs[static_cast<int>(EProgramNames::P_TEXTURE)] = ShaderLoader::CreateProgram(VShaderNames[static_cast<int>(EVShaderNames::VS_NDC_TEXTURE)], FShaderNames[static_cast<int>(EFShaderNames::FS_TEXTURE)]);
	Programs[static_cast<int>(EProgramNames::P_TEXTURE_MIX)] = ShaderLoader::CreateProgram(VShaderNames[static_cast<int>(EVShaderNames::VS_NDC_TEXTURE)], FShaderNames[static_cast<int>(EFShaderNames::FS_TEXTURE_MIX)]);
	Programs[static_cast<int>(EProgramNames::P_ANIMATE_SPRITE)] = ShaderLoader::CreateProgram(VShaderNames[static_cast<int>(EVShaderNames::VS_SPRITE_ANIMATE)], FShaderNames[static_cast<int>(EFShaderNames::FS_TEXTURE)]);
	Programs[static_cast<int>(EProgramNames::P_TEXTURE_MIX_FADE)] = ShaderLoader::CreateProgram(VShaderNames[static_cast<int>(EVShaderNames::VS_NDC_TEXTURE)], FShaderNames[static_cast<int>(EFShaderNames::FS_TEXTURE_MIX_FADE)]);
	Programs[static_cast<int>(EProgramNames::P_WORLD_SPACE)] = ShaderLoader::CreateProgram(VShaderNames[static_cast<int>(EVShaderNames::VS_WORLD_SPACE)], FShaderNames[static_cast<int>(EFShaderNames::FS_TEXTURE)]);
	Programs[static_cast<int>(EProgramNames::P_CLIP_SPACE)] = ShaderLoader::CreateProgram(VShaderNames[static_cast<int>(EVShaderNames::VS_CLIP_SPACE)], FShaderNames[static_cast<int>(EFShaderNames::FS_TEXTURE)]);
	Programs[static_cast<int>(EProgramNames::P_WORLD_TEXTURE_FADE)] = ShaderLoader::CreateProgram(VShaderNames[static_cast<int>(EVShaderNames::VS_WORLD_SPACE)], FShaderNames[static_cast<int>(EFShaderNames::FS_TEXTURE_MIX_FADE)]);
	Programs[static_cast<int>(EProgramNames::P_CLIP_TEXTURE_FADE)] = ShaderLoader::CreateProgram(VShaderNames[static_cast<int>(EVShaderNames::VS_CLIP_SPACE)], FShaderNames[static_cast<int>(EFShaderNames::FS_TEXTURE_MIX_FADE)]);
	Programs[static_cast<int>(EProgramNames::P_WORLD_COLOR_FADE)] = ShaderLoader::CreateProgram(VShaderNames[static_cast<int>(EVShaderNames::VS_WORLD_SPACE)], FShaderNames[static_cast<int>(EFShaderNames::FS_VERTEX_COLOR_FADE)]);
	Programs[static_cast<int>(EProgramNames::P_CLIP_COLOR_FADE)] = ShaderLoader::CreateProgram(VShaderNames[static_cast<int>(EVShaderNames::VS_CLIP_SPACE)], FShaderNames[static_cast<int>(EFShaderNames::FS_VERTEX_COLOR_FADE)]);
	Programs[static_cast<int>(EProgramNames::P_WORLD_ANIMATE)] = ShaderLoader::CreateProgram(VShaderNames[static_cast<int>(EVShaderNames::VS_WORLD_SPACE)], FShaderNames[static_cast<int>(EFShaderNames::FS_ANIMATED_TEXTURE)]);
	Programs[static_cast<int>(EProgramNames::P_CLIP_ANIMATE)] = ShaderLoader::CreateProgram(VShaderNames[static_cast<int>(EVShaderNames::VS_CLIP_SPACE)], FShaderNames[static_cast<int>(EFShaderNames::FS_ANIMATED_TEXTURE)]);
}

//loops through all textures listed in arrays and enums to load the textures for later use.
void cShapeRenderer::LoadTextures()
{
	//Sets stbi to render textures up the right way
	TextureLoader::SetStbiFlip(true);
	
	int iSize = sizeof(Textures) / sizeof(GLuint);
	//Create a new texture template
	glGenTextures(iSize, &Textures[0]);

	for (int i = 0; i < iSize; i++)
	{
		Textures[i] = TextureLoader::CheckTexture(ImageNames[i]);
	}
}

//adds delta time to accumulated time.  If reaches target FPS threshold, subtracts 1 frametime and initiates calculations required for next frame.
void cShapeRenderer::Update(float _fDeltaTime)
{
	mfAccumulatedDelta += _fDeltaTime;
	if (mfAccumulatedDelta >= (1 / mfTargetFPS))
	{
		mfAccumulatedDelta -= (1 / mfTargetFPS);
		if (CalculateNextFrame())
		{
			mpMyShape->UpdateObjectModelMat(); //only gets called if changes have been made, to reduce overall calculation requirements
		}
	}
}

//Changes variables as required for updating animation, based on program called.
//Returns TRUE if changes are made to variables that affect model matrix, false if not.
bool cShapeRenderer::CalculateNextFrame()
{
	switch (meProgramType)
	{
	case EProgramNames::P_FIXED_TRI:		//fall through - no calc needed
	case EProgramNames::P_POSITION_ONLY:	//fall through - no calc needed
	case EProgramNames::P_VERTEX_COLOR:	//fall through - no calc needed
	case EProgramNames::P_TEXTURE:			//fall through - no calc needed
	case EProgramNames::P_TEXTURE_MIX:		//fall through - no calc needed
		return false;
		break;
	
	case EProgramNames::P_COLOR_FADE:
		UpdateFader();
		return false;		//fader alone does not go through object matrix so no need to update matrix
		break;
	
	case EProgramNames::P_ANIMATE_SPRITE:
		CalculateSpriteChanges();
		break;
	
	case EProgramNames::P_TEXTURE_MIX_FADE:
		UpdateFader();
		return false;		//fader alone does not go through object matrix so no need to update matrix
		break;
	
	case EProgramNames::P_WORLD_SPACE:
		UpdateRotMat();
		break;
	
	case EProgramNames::P_CLIP_SPACE:
		UpdateRotMat();
		break;
	
	case EProgramNames::P_WORLD_TEXTURE_FADE:
		UpdateFader();
		UpdateRotMat();
		break;
	
	case EProgramNames::P_CLIP_TEXTURE_FADE:
		UpdateFader();
		UpdateRotMat();
		break;
	
	case EProgramNames::P_WORLD_COLOR_FADE:
		UpdateFader();
		UpdateRotMat();
		break;
	
	case EProgramNames::P_CLIP_COLOR_FADE:
		UpdateFader();
		UpdateRotMat();
		break;
	
	case EProgramNames::P_WORLD_ANIMATE:
		CalculateSpriteChanges();
		UpdateRotMat();
		break;
	
	case EProgramNames::P_CLIP_ANIMATE:
		CalculateSpriteChanges();
		UpdateRotMat();
		break;
	
	default:
		std::cout << "I added some extra items to the Program array and forgot to update the Switch block.  ShapeRenderer::CalcNextFrame()\n";
		break;
	}
	return true;
}

//Adds frame rotation variable to object rotation variable, to be updated in object matrix later
void cShapeRenderer::UpdateRotMat()
{
	if (mfRotationPerFrame != 0.0f)
	{
		mpMyShape->SetObjRotation(mpMyShape->GetObjRotation() + mfRotationPerFrame);
	}
}

//Changes the transparency variable by incrementing or decrementing as necessary.
void cShapeRenderer::UpdateFader()
{
	float fTransparencyChange = 0.02f;
	if (mbTraverseRight)
	{
		mfTransparency += fTransparencyChange;
		if (mfTransparency >= 1.0f)
		{
			mbTraverseRight = !mbTraverseRight;
		}
	}
	else
	{
		mfTransparency -= fTransparencyChange;
		if (mfTransparency <= 0.0f)
		{
			mbTraverseRight = !mbTraverseRight;
		}
	}
}

//uses program and binds array, then calls other functions and sends uniforms, dependant on which shaders are present.  Finishes by drawing elements.
void cShapeRenderer::RenderShape()
{
	glUseProgram(miProgramID);
	glBindVertexArray(miVAO);

	switch (meProgramType)
	{
	case EProgramNames::P_FIXED_TRI:
		//VS_FIXED_TRIANGLE;
		//FS_FIXED_COLOR;
		break;
	
	case EProgramNames::P_POSITION_ONLY:
		//VS_POSITION_ONLY;
		//FS_FIXED_COLOR;
		break;
	
	case EProgramNames::P_VERTEX_COLOR:
		//VS_VERTEX_COLOR;
		//FS_VERTEX_COLOR;
		break;
	
	case EProgramNames::P_COLOR_FADE:
		//VS_VERTEX_COLOR;
		//FS_VERTEX_COLOR_FADE;
		
		//Get Uniform location and pass uniform to shader
		glUniform1f(glGetUniformLocation(miProgramID, "Transparency"), mfTransparency);

		break;
	
	case EProgramNames::P_TEXTURE:
		//VS_NDC_TEXTURE;
		//FS_TEXTURE;

		BindAndUseTexture(0, miTexture0);
		break;
	
	case EProgramNames::P_TEXTURE_MIX:
		//VS_NDC_TEXTURE;
		//FS_TEXTURE_MIX;

		BindAndUseTexture(0, miTexture0);
		BindAndUseTexture(1, miTexture1);
		glUniform1f(glGetUniformLocation(miProgramID, "Transparency"), 0.5f);
		break;
	
	case EProgramNames::P_ANIMATE_SPRITE:
		//VS_SPRITE_ANIMATE;
		//FS_TEXTURE;
		BindAndUseTexture(0, miTexture0);
		glUniform2fv(glGetUniformLocation(miProgramID, "TexOffset"), 1, TexOffset);
		break;
	
	case EProgramNames::P_TEXTURE_MIX_FADE:
		//VS_NDC_TEXTURE;
		//FS_TEXTURE_MIX_FADE;

		BindAndUseTexture(0, miTexture0);
		BindAndUseTexture(1, miTexture1);
		glUniform1f(glGetUniformLocation(miProgramID, "Transparency"), mfTransparency);
		break;
	
	case EProgramNames::P_WORLD_SPACE:
		//VS_WORLD_SPACE;
		//FS_TEXTURE;

		BindAndUseTexture(0, miTexture0);
		glUniformMatrix4fv(glGetUniformLocation(miProgramID, "ObjectModelMat"), 1, GL_FALSE, glm::value_ptr(mpMyShape->GetModelMat()));
		//RenderWorldSpace();
		break;
	
	case EProgramNames::P_CLIP_SPACE:
		//VS_CLIP_SPACE;
		//FS_TEXTURE;

		glUniformMatrix4fv(glGetUniformLocation(miProgramID, "PvmMat"), 1, GL_FALSE, glm::value_ptr((mpMyCamera->GetProjectionMat() * mpMyCamera->GetViewMat() * mpMyShape->GetModelMat())));

		BindAndUseTexture(0, miTexture0);
		break;
	
	case EProgramNames::P_WORLD_TEXTURE_FADE:
		//VS_WORLD_SPACE;
		//FS_TEXTURE_MIX_FADE;

		BindAndUseTexture(0, miTexture0);
		BindAndUseTexture(1, miTexture1);
		glUniform1f(glGetUniformLocation(miProgramID, "Transparency"), mfTransparency);
		glUniformMatrix4fv(glGetUniformLocation(miProgramID, "ObjectModelMat"), 1, GL_FALSE, glm::value_ptr(mpMyShape->GetModelMat()));
		break;

	case EProgramNames::P_CLIP_TEXTURE_FADE:
		//VS_CLIP_SPACE;
		//FS_TEXTURE_MIX_FADE;
		BindAndUseTexture(0, miTexture0);
		BindAndUseTexture(1, miTexture1);
		glUniform1f(glGetUniformLocation(miProgramID, "Transparency"), mfTransparency);
		glUniformMatrix4fv(glGetUniformLocation(miProgramID, "PvmMat"), 1, GL_FALSE, glm::value_ptr((mpMyCamera->GetProjectionMat() * mpMyCamera->GetViewMat() * mpMyShape->GetModelMat())));
		break;

	case EProgramNames::P_WORLD_COLOR_FADE:
		//VS_WORLD_SPACE;
		//FS_VERTEX_COLOR_FADE;

		glUniform1f(glGetUniformLocation(miProgramID, "Transparency"), mfTransparency);
		glUniformMatrix4fv(glGetUniformLocation(miProgramID, "ObjectModelMat"), 1, GL_FALSE, glm::value_ptr(mpMyShape->GetModelMat()));
		break;

	case EProgramNames::P_CLIP_COLOR_FADE:
		//VS_CLIP_SPACE;
		//FS_VERTEX_COLOR_FADE;

		glUniformMatrix4fv(glGetUniformLocation(miProgramID, "PvmMat"), 1, GL_FALSE, glm::value_ptr((mpMyCamera->GetProjectionMat() * mpMyCamera->GetViewMat() * mpMyShape->GetModelMat())));
		glUniform1f(glGetUniformLocation(miProgramID, "Transparency"), mfTransparency);
		break;

	case EProgramNames::P_WORLD_ANIMATE:
		//VS_WORLD_SPACE;
		//FS_ANIMATED_TEXTURE
		BindAndUseTexture(0, miTexture0);
		glUniform2fv(glGetUniformLocation(miProgramID, "TexOffset"), 1, TexOffset);
		glUniformMatrix4fv(glGetUniformLocation(miProgramID, "ObjectModelMat"), 1, GL_FALSE, glm::value_ptr(mpMyShape->GetModelMat()));
		break;

	case EProgramNames::P_CLIP_ANIMATE:
		//VS_CLIP_SPACE;
		//FS_ANIMATED_TEXTURE
		BindAndUseTexture(0, miTexture0);
		glUniform2fv(glGetUniformLocation(miProgramID, "TexOffset"), 1, TexOffset);
		glUniformMatrix4fv(glGetUniformLocation(miProgramID, "PvmMat"), 1, GL_FALSE, glm::value_ptr((mpMyCamera->GetProjectionMat() * mpMyCamera->GetViewMat() * mpMyShape->GetModelMat())));
		break;

	default:
		std::cout << "I added some extra items to the Program array and forgot to update the Switch block.  ShapeRenderer::RenderShape()\n";
		break;
	}

	glDrawElements(GL_TRIANGLES, miNumVertices, GL_UNSIGNED_INT, 0);

}	 

//Binds given texture to given GLuint, and gets the relevant string for the shader.
void cShapeRenderer::BindAndUseTexture(int _TextureNum, GLuint _TextureID)
{
	std::string sImageName;
	switch (_TextureNum)
	{
	case 0:
		glActiveTexture(GL_TEXTURE0);
		sImageName = "ImageTexture";
		break;
	
	case 1:
		glActiveTexture(GL_TEXTURE1);
		sImageName = "ImageTexture1";
		break;

	default:
		std::cout << "I tried to bind an extra texture but only allowed 2 slots in cShapeRenderer::BindAndUseTexture() \n";
		break;
	}

	glBindTexture(GL_TEXTURE_2D, _TextureID);
	glUniform1i(glGetUniformLocation(miProgramID, sImageName.c_str()), _TextureNum);
}

//Applies the texture to the second slot if boolean is true, first slot if false.  Only allows for two textures at this stage.
void cShapeRenderer::SetTexture(bool _SecondTexture, EImageNames _DesiredTexture)
{
	if (_SecondTexture)
	{
		miTexture1 = Textures[static_cast<int>(_DesiredTexture)];
	}
	else
	{
		miTexture0 = Textures[static_cast<int>(_DesiredTexture)];
	}
}

//Updates the sprite animation variables to be ready to show the next frame
void cShapeRenderer::CalculateSpriteChanges()
{
	mbTraverseRight ? miFrameCounter++ : miFrameCounter--;

	if (mbLoopSprite)
	{
		if (miFrameCounter >= miFramesInSprite)
		{
			miFrameCounter = 0;
		}
	}
	else if (miFrameCounter >= miFramesInSprite - 1 || miFrameCounter <= 0)
	{
		mbTraverseRight = !mbTraverseRight;
	}

	UpdateTexOffset();
}

//Updates the texture offset vector to show the next frame of the sprite sheet.
void cShapeRenderer::UpdateTexOffset()
{
	for (int i = 0; i < 8; i += 2)
	{
		TexOffset[i] = (miFrameCounter * mfSpriteWidth) / mfTextureWidth;
	}
}

//copies given vertex array onto shape vertex array to replace the default vertices
void cShapeRenderer::ChangeVertexArray(GLfloat* _NewVertArray, int _NewArrSize, GLenum _StaticOrDynamic)
{
	mpMyShape->ChangeVertexArray(_NewVertArray, _NewArrSize); 
	mpMyShape->SetVertexAttributes(miVAO, miEBO, miVBO, _StaticOrDynamic);
};
