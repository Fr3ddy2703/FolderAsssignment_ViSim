#pragma once
#include "../Mesh.h"

class PointCloud : public Mesh
{
public:
	PointCloud(){};
	~PointCloud(){};
	//void CreatePointCloudFromFile(const char* _fileDirectory, float _scaleFactor);
	void CreatePointCloudFromFile(const char* _fileDirectory, float _scaleFactor);

	void Draw();
	void BindBuffer();

private:
	unsigned int VBO;
	unsigned int EBO;
	unsigned int VAO;

};

