#pragma once
#include "../Mesh.h"

class BSplineSurface : public Mesh
{
public:

	BSplineSurface(){};

	void CreateBSplineSurface(int _UResolution, int _VResolution, int _du,
		int _dv, const std::vector<float>& _uKnot, const std::vector<float>& _vKnot,
	const std::vector<std::vector<glm::vec3>>& _controlPoints);

	void Draw();

	void BindBuffer();

private:
	unsigned int mVBO;
	unsigned int mEBO;
	unsigned int mVAO;
};

