#pragma once
#include "ResourceManager.h"
#include "Mesh.h"

class MeshManager : public ResourceManager
{
public:
	MeshManager();
	~MeshManager();
	MeshPtr createMeshFromFile(const wchar_t* file_path);

protected:
	virtual Resource* createResourceFromFileConcrete(const wchar_t* file_path);
};

