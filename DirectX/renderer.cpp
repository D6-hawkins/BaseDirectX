//Inclusion of basic windows header files and direct3D header files
#include "Renderer.h"
Renderer::Renderer()
{
	m_D3D = 0;
	m_Camera = 0;
	m_Model = 0;
	m_ColorShader = 0;
	m_TextureShader = 0;
	m_LightShader = 0;
	m_Light = 0;
	isRotating = false;
}
Renderer::~Renderer()
{

}
Renderer::Renderer(const Renderer &)
{
}
bool Renderer::Initialize(int screenWidth, int screenHeight, HWND hwnd)
{
	bool result;


	// Create the Direct3D object.
	m_D3D = new D3DClass;
	if (!m_D3D)
	{
		return false;
	}

	// Initialize the Direct3D object.
	result = m_D3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize Direct3D", L"Error", MB_OK);
		return false;
	}
	// Create the camera object.
	m_Camera = new Camera;
	if (!m_Camera)
	{
		return false;
	}

	// Set the initial position of the camera.
	m_Camera->SetPosition(0.0f, 0.0f, -10.0f);

	// Create the model object.
	m_Model = new Model;
	if (!m_Model)
	{
		return false;
	}

	// Initialize the model object.
	result = m_Model->Initialize(m_D3D->GetDevice(), "cube.txt", L"brick.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}
	// Create the light shader object.
	m_LightShader = new LightShaderClass;
	if (!m_LightShader)
	{
		return false;
	}

	// Initialize the light shader object.
	result = m_LightShader->Initialize(m_D3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the light shader object.", L"Error", MB_OK);
		return false;
	}
		// Create the light object.
		m_Light = new LightClass;
	if (!m_Light)
	{
		return false;
	}
		// Initialize the light object.
	m_Light->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light->SetDirection(0.0f, 0.0f, 1.0f);
	// Create the texture shader object.
	m_TextureShader = new TextureShaderClass;
	if (!m_TextureShader)
	{
		return false;
	}

	// Initialize the texture shader object.
	result = m_TextureShader->Initialize(m_D3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the texture shader object.", L"Error", MB_OK);
		return false;
	}
	//Initialise Ant tweak bar
	TwInit(TW_DIRECT3D11, m_D3D->GetDevice());

	TwWindowSize(200, 200); //Choose the window size for the bar

	m_Bar = TwNewBar("Cube Values");
	modelLength = m_Model->getLength(); //Find length
	modelHeight = m_Model->getHeight(); //Find height
	modelWidth = m_Model->getWidth(); //find width
	TwAddVarRW(m_Bar, "X Axis", TW_TYPE_FLOAT, &modelLength, ""); //X axis value added to tweak bar
	TwAddVarRW(m_Bar, "Y Axis", TW_TYPE_FLOAT, &modelHeight, ""); // Y axis value added to tweak bar
	TwAddVarRW(m_Bar, "Z Axis", TW_TYPE_FLOAT, &modelWidth, ""); //z axis value added to tweak bar
	TwAddVarRW(m_Bar, "Rotation", TW_TYPE_BOOL16, &isRotating, ""); //rotation bool added to tweak bar
	return true;
}
void Renderer::Shutdown()
{
	// Release the light object.
	if (m_Light)
	{
		delete m_Light;
		m_Light = 0;
	}

	// Release the light shader object.
	if (m_LightShader)
	{
		m_LightShader->Shutdown();
		delete m_LightShader;
		m_LightShader = 0;
	}
	// Release the texture shader object.
	if (m_TextureShader)
	{
		m_TextureShader->Shutdown();
		delete m_TextureShader;
		m_TextureShader = 0;
	}
	if (m_D3D)
	{
		m_D3D->Shutdown();
		delete m_D3D;
		m_D3D = 0;
	}
	// Release the model object.
	if (m_Model)
	{
		m_Model->Shutdown();
		delete m_Model;
		m_Model = 0;
	}

	// Release the camera object.
	if (m_Camera)
	{
		delete m_Camera;
		m_Camera = 0;
	}
	//Release the Tweak Bar
	if (m_Bar)
	{
		TwTerminate();
		m_Bar = 0;
	}
}
bool Renderer::Frame()
{
	bool result;
	// Render the graphics scene.
	static float rotation = 0.0f;
	m_Model->setLength(modelLength);
	m_Model->setHeight(modelHeight);
	m_Model->setWidth(modelWidth);
	// Update the rotation variable each frame.
	if (isRotating == true)
	{
		rotation += (float)D3DX_PI * 0.01f;
		if (rotation > 360.0f)
		{
			rotation -= 360.0f;
		}
	}
		result = Render(rotation);
		if (!result)
		{
			return false;
		}

	return true;
}
bool Renderer::Render(float rotation)
{
	// Clear the buffers to begin the scene.
	D3DXMATRIX viewMatrix, projectionMatrix, worldMatrix;
	bool result;

	// Clear the buffers to begin the scene.
	m_D3D->BeginScene(0.0f, 0.0f, 0.5f, 1.0f);

	// Generate the view matrix based on the camera's position.
	m_Camera->Render();

	// Get the world, view, and projection matrices from the camera and d3d objects.
	m_Camera->GetViewMatrix(viewMatrix);
	m_D3D->GetWorldMatrix(worldMatrix);
	m_D3D->GetProjectionMatrix(projectionMatrix);

	// Rotate the world matrix by the rotation value so that the triangle will spin.
	D3DXMatrixRotationY(&worldMatrix, rotation);
	// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
	m_Model->Render(m_D3D->GetDeviceContext());

	// Render the model using the light shader.
	result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_Model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
		m_Model->GetTexture(), m_Light->GetDirection(), m_Light->GetDiffuseColor());

	TwDraw();
	if (!result)
	{
		return false;
	}


	// Present the rendered scene to the screen.
	m_D3D->EndScene();
	return true;
}