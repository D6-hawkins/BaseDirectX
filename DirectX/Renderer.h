#ifndef _RENDERER_H_
#define _RENDERER_H_

//Classes
#include "d3dclass.h"
#include "Camera.h"
#include "Model.h"
#include "ColorShaderClass.h"
#include "textureshaderclass.h"

#include "LightShaderclass.h"
#include "Lightclass.h"
//Global

const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;
class Renderer
{
public:
	Renderer();
	Renderer(const Renderer&);
	~Renderer();

	bool Initialize(int, int, HWND);
	void Shutdown();
	bool Frame();



private:
	bool Render(float);
	D3DClass* m_D3D;
	Camera* m_Camera;
	Model* m_Model;
	ColorShaderClass* m_ColorShader;
	TextureShaderClass* m_TextureShader;

	LightShaderClass* m_LightShader;
	LightClass* m_Light;
};

#endif // !
