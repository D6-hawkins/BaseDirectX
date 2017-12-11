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

	float getLength() {
		return length;
	};
	void setLength(float len)
	{
		length = len;
	};
	float getWidth() {
		return width;
	};
	void setWidth(float wid)
	{
		width = wid;
	};
	float getHeight() {
		return height;
	};
	void setHeight(float Hei)
	{
		height = Hei;
	};
private:
	bool InitializeBuffers(ID3D11Device*);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);

private:
	ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;
	int m_vertexCount, m_indexCount;

	float length = 0.0f;
	float height = 0.0f;
	float width = 0.0f;

	float Xpos = 0.0f;
	float Ypos = 0.0f;
	float Zpos = 0.0f;
	bool LoadTexture(ID3D11Device*, WCHAR*);
	void ReleaseTexture();

	int lenArray[14] = {0, 2, 3, 33, 17, 16, 13, 18, 19, 20, 21, 22, 23, 24};
	int heightArray[18] = {0, 1, 4,24, 25,26, 27, 28, 29, 6, 10, 7, 12, 13, 16, 18, 19, 22};
	int depthArray[12] = {7, 10, 11, 12, 13, 14, 15, 16, 17, 18, 20, 21};
	//int depthArray[1] = { 12 };
	bool LoadModel(char*);
	void ReleaseModel();
	TextureClass* m_Texture;
	ModelType* m_model;

	ID3D11Device* devStore;
	char* modelFileName;
	WCHAR* texName;
};

#endif