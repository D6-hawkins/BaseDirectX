////////////////////////////////////////////////////////////////////////////////
// Filename: modelclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "Model.h"

Model::Model()
{
	m_vertexBuffer = 0;
	m_indexBuffer = 0;
	m_Texture = 0;
	m_model = 0;
	m_instanceBuffer = 0;
}


Model::Model(const Model& other)
{
}


Model::~Model()
{
}

bool Model::Initialize(ID3D10Device* device,char* modelFilename, WCHAR* textureFileName)
{
	bool result;

	// Load in the model data,
	result = LoadModel(modelFilename);
	if (!result)
	{
		return false;
	}
	// Initialize the vertex and index buffer that hold the geometry for the triangle.
	result = InitializeBuffers(device);
	if (!result)
	{
		return false;
	}
	//Load the texture
	result = LoadTexture(device, textureFileName);
	if (!result)
	{
		return false;
	}
	return true;
}

void Model::Shutdown()
{
	//release the model
	ReleaseTexture();
	// Release the vertex and index buffers.
	ShutdownBuffers();

	// Release the model data.
	ReleaseModel();
	return;
}
void Model::Render(ID3D10Device* device)
{
	// Put the vertex and index buffers on the graphics pipeline to prepare them for drawing.
	RenderBuffers(device);

	return;
}
int Model::GetIndexCount()
{
	return m_indexCount;
}
ID3D10ShaderResourceView* Model::GetTexture()
{
	return m_Texture->GetTexture();
}
bool Model::InitializeBuffers(ID3D10Device* device)
{
	VERTEX* vertices;
	InstanceType* instances;
	unsigned long* indices;
	D3D10_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D10_BUFFER_DESC instanceBufferDesc;
	D3D10_SUBRESOURCE_DATA vertexData, indexData, instanceData;
	HRESULT result;
	int i;
	//	// Set the number of vertices in the vertex array.
	//	m_vertexCount = 3;

	//// Set the number of indices in the index array.
	//m_indexCount = 3;

	// Create the vertex array.
	vertices = new VERTEX[m_vertexCount];
	if (!vertices)
	{
		return false;
	}

	// Create the index array.
	//indices = new unsigned long[m_indexCount];
	//if (!indices)
	//{
	//	return false;
	//}
	// Load the vertex array and index array with data.
	for (i = 0; i<m_vertexCount; i++)
	{
		vertices[i].Pos = D3DXVECTOR3(m_model[i].x, m_model[i].y, m_model[i].z);
		vertices[i].Tex = D3DXVECTOR2(m_model[i].tu, m_model[i].tv);
		//vertices[i].Norm = D3DXVECTOR3(m_model[i].nx, m_model[i].ny, m_model[i].nz);

		//indices[i] = i;
	}

	// Set up the description of the static vertex buffer.
	vertexBufferDesc.Usage = D3D10_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VERTEX) * m_vertexCount;
	vertexBufferDesc.BindFlags = D3D10_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	//vertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Now create the vertex buffer.
	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// Set up the description of the static index buffer.
	//indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	//indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_indexCount;
	//indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	//indexBufferDesc.CPUAccessFlags = 0;
	//indexBufferDesc.MiscFlags = 0;
	//indexBufferDesc.StructureByteStride = 0;

	//// Give the subresource structure a pointer to the index data.
	//indexData.pSysMem = indices;
	//indexData.SysMemPitch = 0;
	//indexData.SysMemSlicePitch = 0;

	//// Create the index buffer.
	//result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
	//if (FAILED(result))
	//{
	//	return false;
	//}
		// Release the arrays now that the vertex and index buffers have been created and loaded.
		delete[] vertices;
	vertices = 0;

	//delete[] indices;
	//indices = 0;

	//number of instances
	m_instanceCount = 4;

	//Instance array
	instances = new InstanceType[m_instanceCount];
	if (!instances)
	{
		return false;
	}
	// Load the instance array with data.
	instances[0].position = D3DXVECTOR3(-1.5f, -1.5f, 5.0f);
	instances[1].position = D3DXVECTOR3(-1.5f, 1.5f, 5.0f);
	instances[2].position = D3DXVECTOR3(1.5f, -1.5f, 5.0f);
	instances[3].position = D3DXVECTOR3(1.5f, 1.5f, 5.0f);

	// Set up the description of the instance buffer.
	instanceBufferDesc.Usage = D3D10_USAGE_DEFAULT;
	instanceBufferDesc.ByteWidth = sizeof(InstanceType) * m_instanceCount;
	instanceBufferDesc.BindFlags = D3D10_BIND_VERTEX_BUFFER;
	instanceBufferDesc.CPUAccessFlags = 0;
	instanceBufferDesc.MiscFlags = 0;

	// Give the subresource structure a pointer to the instance data.
	instanceData.pSysMem = instances;

	// Create the instance buffer.
	result = device->CreateBuffer(&instanceBufferDesc, &instanceData, &m_instanceBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// Release the instance array now that the instance buffer has been created and loaded.
	delete[] instances;
	instances = 0;
	return true;
}
void Model::ShutdownBuffers()
{
	// Release the index buffer.
	//if (m_indexBuffer)
	//{
	//	m_indexBuffer->Release();
	//	m_indexBuffer = 0;
	//}
	// Release the instance buffer.
	if (m_instanceBuffer)
	{
		m_instanceBuffer->Release();
		m_instanceBuffer = 0;
	}
	// Release the vertex buffer.
	if (m_vertexBuffer)
	{
		m_vertexBuffer->Release();
		m_vertexBuffer = 0;
	}

	return;
}
void Model::RenderBuffers(ID3D10Device* device)
{
	//unsigned int stride;
	//unsigned int offset;

	unsigned int strides[2];
	unsigned int offsets[2];
	ID3D10Buffer* bufferPointers[2];
	// Set vertex buffer stride and offset.
	strides[0] = sizeof(VERTEX);
	strides[1] = sizeof(InstanceType);
	offsets[0] = 0;
	offsets[1] = 0;

	bufferPointers[0] = m_vertexBuffer;
	bufferPointers[1] = m_instanceBuffer;
	
	// Set the vertex buffer to active in the input assembler so it can be rendered.
	device->IASetVertexBuffers(0, 2, bufferPointers, strides, offsets);

	// Set the index buffer to active in the input assembler so it can be rendered.
	device->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	device->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return;
}
bool Model::LoadTexture(ID3D10Device* device, WCHAR* filename)
{
	bool result;
	// Create the texture object.
	m_Texture = new TextureClass;
	if (!m_Texture)
	{
		return false;
	}

	// Initialize the texture object.
	result = m_Texture->Initialize(device, filename);
	if (!result)
	{
		return false;
	}

	return true;
}
void Model::ReleaseTexture()
{
	// Release the texture object.
	if (m_Texture)
	{
		m_Texture->Shutdown();
		delete m_Texture;
		m_Texture = 0;
	}

	return;
}
bool Model::LoadModel(char* filename)
{
	ifstream fin;
	char input;
	int i;


	// Open the model file.
	fin.open(filename);

	// If it could not open the file then exit.
	if (fin.fail())
	{
		return false;
	}

	// Read up to the value of vertex count.
	fin.get(input);
	while (input != ':')
	{
		fin.get(input);
	}

	// Read in the vertex count.
	fin >> m_vertexCount;

	// Set the number of indices to be the same as the vertex count.
	m_indexCount = m_vertexCount;

	// Create the model using the vertex count that was read in.
	m_model = new ModelType[m_vertexCount];
	if (!m_model)
	{
		return false;
	}

	// Read up to the beginning of the data.
	fin.get(input);
	while (input != ':')
	{
		fin.get(input);
	}
	fin.get(input);
	fin.get(input);

	// Read in the vertex data.
	for (i = 0; i<m_vertexCount; i++)
	{
		fin >> m_model[i].x >> m_model[i].y >> m_model[i].z;
		fin >> m_model[i].tu >> m_model[i].tv;
		fin >> m_model[i].nx >> m_model[i].ny >> m_model[i].nz;
	}

	// Close the model file.
	fin.close();

	return true;
}
void Model::ReleaseModel()
{
	if (m_model)
	{
		delete[] m_model;
		m_model = 0;
	}

	return;
}
int Model::GetVertexCount()
{
	return m_vertexCount;
}
int Model::GetInstanceCount()
{
	return m_instanceCount;
}