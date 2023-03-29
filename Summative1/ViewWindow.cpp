#include "ViewWindow.h"

//Default constructor, starts with 800x800 viewport.
cViewWindow::cViewWindow()
{
	CreateWindow("OpenGL Window", 800, 800);
}

//Constructor - accepts window title and dimensions.
cViewWindow::cViewWindow(std::string _WindowName, int _Width, int _Height)
{
	CreateWindow(_WindowName, _Width, _Height);
}

//Destructor - no code because no memory is allocate with pointers.
cViewWindow::~cViewWindow()
{
}

//Initialises GLFW and GLEW, creates window for display
void cViewWindow::CreateWindow(std::string _WindowName, int _Width, int _Height)
{
	mWindowHeight = _Height;
	mWindowWidth = _Width;

	//Initializing GLFW and setting the version to 4.6 with only Core functionality available
	glfwInit();
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);

	//Create a GLFW controlled context window
	mpWindow = glfwCreateWindow(_Width, _Height, _WindowName.c_str(), NULL, NULL);
	if (mpWindow == NULL)
	{
		std::cout << "GLFW failed to initialize properly. Terminating program." << std::endl;
		system("pause");

		glfwTerminate();
		exit(-1);
	}

	glfwMakeContextCurrent(mpWindow);

	//Initialising GLEW to populate OpenGL function pointers
	if (glewInit() != GLEW_OK)
	{
		std::cout << "GLEW failed to initialise properly. Terminating program." << std::endl;
		system("pause");

		glfwTerminate();
		exit(-1);
	}

	//Set the color of the window for when the buffer is cleared
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	//Maps the range of the window size to NDC (-1 -> 1)
	glViewport(0, 0, mWindowWidth, mWindowHeight);
}
