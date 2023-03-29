#pragma once

#ifndef __VIEWWINDOW_H__
#define __VIEWWINDOW_H__

#include <glew.h>
#include <glfw3.h>
#include <iostream>

//Small class used to potentially create multiple view windows if an application ever needed that ability.
//Also to tidy up the main file code a little.
class cViewWindow
{
protected:
	int mWindowWidth;
	int mWindowHeight;
	GLFWwindow* mpWindow = nullptr;  //Window pointer

	void CreateWindow(std::string _WindowName, int _Width, int _Height);
		
public:
	cViewWindow();
	cViewWindow(std::string _WindowName, int _Width, int _Height);
	~cViewWindow();
	
	int SetWindowHeight(int _NewHeight) { mWindowHeight = _NewHeight; };
	int SetWindowWidth(int _NewWidth) { mWindowWidth = _NewWidth; };

	const int GetWindowWidth() { return mWindowWidth; };
	const int GetWindowHeight() { return mWindowHeight; };
		
	GLFWwindow* GetWindowPtr() { return mpWindow; };
};


#endif // !__VIEWWINDOW_H__
