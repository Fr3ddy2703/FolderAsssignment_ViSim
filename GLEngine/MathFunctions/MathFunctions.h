#pragma once
#include "../Meshes/Cubes/Cubes.h"
class Cube;
class Mesh;
class Spheres;

class MathFunctions
{
public:
	int n = 1;

	int findKnotInterval(float _x);
	void deBoorsAlgorithm(float _x);
	//static std::vector<glm::vec3> LASFileToPoints(const char* _fileDirectory);
	//static void LASFileToCustomFileOfPoints(const char* _fileDirectoryInn, const char* _fileDirectoryOut);
	float calculateNormal(glm::vec3&& vector1, glm::vec3&& vector2);
};

class BSpline
{
public:

	/* Public variables */


	static glm::vec3 evaluateBSplineSurface(float _u, float _v, int _du, int _dv, const std::vector<float>& _uKnot, 
		const  std::vector<float>& _vKnot, const std::vector<std::vector<glm::vec3>>& _controlPoints);

	static glm::vec3 evaluateBSplineNormal(float _u, float _v, int _du, int _dv, int _UResolution, int _VResolution, 
		const std::vector<float>& _uKnot, const  std::vector<float>& _vKnot, const std::vector<std::vector<glm::vec3>>& _controlPoints);

	static float CoxDeBoorRecursive(int _i, int _d, float _uv, const std::vector<float>& _knotVector);



};

class Barycentric : public MathFunctions, public Cube

{
public:
	float BarycentricCord(std::shared_ptr<Spheres>_object, std::shared_ptr<Cube>_surface);
};
