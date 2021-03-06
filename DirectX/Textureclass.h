////////////////////////////////////////////////////////////////////////////////
// Filename: textureclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _TEXTURECLASS_H_
#define _TEXTURECLASS_H_


//////////////
// INCLUDES //
//////////////
#include <d3d11.h>
#include <d3dx10tex.h>

class TextureClass
{
public:
	TextureClass();
	TextureClass(const TextureClass&);
	~TextureClass();
	bool Initialize(ID3D10Device*, WCHAR*);
	void Shutdown();
		ID3D10ShaderResourceView* GetTexture();

private:
		ID3D10ShaderResourceView* m_texture;
};

#endif
