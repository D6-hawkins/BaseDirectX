////////////////////////////////////////////////////////////////////////////////
// Filename: modelclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _MODELCLASS_H_
#define _MODELCLASS_H_


//////////////
// INCLUDES //
//////////////
#include <d3d11.h>
#include <d3dx10math.h>

#include "Textureclass.h"
#include <fstream>
using namespace std;
////////////////////////////////////////////////////////////////////////////////
// Class name: ModelClass
////////////////////////////////////////////////////////////////////////////////
class Model
{
private:
	struct VERTEX
	{
		D3DXVECTOR3 Pos; //position
		D3DXVECTOR2 Tex; // Colour
		D3DXVECTOR3 Norm;//normal position
	};

	struct ModelType
	{
		float x, y, z;
		float tu, tv;
		float nx, ny, nz;
	};
public:
	Model();
	Model(const Model&);
	~Model();

	bool Initialize(ID3D11Device*,char*, WCHAR*);
	void Shutdown();
	void Render(ID3D11DeviceContext*);

	int GetIndexCount();

	ID3D11ShaderResourceView* GetTexture();
private:
	bool InitializeBuffers(ID3D11Device*);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);

private:
	ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;
	int m_vertexCount, m_indexCount;

	float length = 1.0f;
	float height = 1.0f;
	float width = 1.0f;

	bool LoadTexture(ID3D11Device*, WCHAR*);
	void ReleaseTexture();

	bool LoadModel(char*);
	void ReleaseModel();
	TextureClass* m_Texture;
	ModelType* m_model;
};

#endif