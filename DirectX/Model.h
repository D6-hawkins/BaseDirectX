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
	struct InstanceType
	{
		D3DXVECTOR3 position;
	};
public:
	Model();
	Model(const Model&);
	~Model();

	bool Initialize(ID3D10Device*,char*, WCHAR*);
	void Shutdown();
	void Render(ID3D10Device*);

	int GetIndexCount();

	ID3D10ShaderResourceView* GetTexture();

	int GetVertexCount();
	int GetInstanceCount();
private:
	bool InitializeBuffers(ID3D10Device*);
	void ShutdownBuffers();
	void RenderBuffers(ID3D10Device*);

private:
	ID3D10Buffer *m_vertexBuffer, *m_indexBuffer;
	int m_vertexCount, m_indexCount;

	float length = 1.0f;
	float height = 1.0f;
	float width = 1.0f;

	bool LoadTexture(ID3D10Device*, WCHAR*);
	void ReleaseTexture();

	bool LoadModel(char*);
	void ReleaseModel();
	TextureClass* m_Texture;
	ModelType* m_model;
	ID3D10Buffer *m_instanceBuffer;
	int m_instanceCount;
};

#endif