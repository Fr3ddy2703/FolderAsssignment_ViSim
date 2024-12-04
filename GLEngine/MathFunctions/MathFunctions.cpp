#include "pch.h"
#include "MathFunctions.h"

#include "../Meshes/Mesh.h"
#include "../Meshes/Cubes/Cubes.h"
#include "../Meshes/Sphere/Spheres.h"
#include "glm/glm.hpp"

float MathFunctions::calculateNormal(glm::vec3&& vector1, glm::vec3&& vector2)
{
    return vector1[0]* vector2[2]- vector2[0]*vector1[2];
}

// Calculation for B-Spline surface
glm::vec3 BSpline::evaluateBSplineSurface(float _u, float _v, int _du, int _dv, 
	const std::vector<float>& _uKnot, 
	const std::vector<float>& _vKnot, 
	const std::vector<std::vector<glm::vec3>>& _controlPoints)
{
	glm::vec3 surfacePoint(0.0f);
	int numControlPointsU = _controlPoints.size() - 1;
	int numControlPointsV = _controlPoints[0].size() - 1;

	/* Loop through all control points */
	for (int i = 0; i <= numControlPointsU; i++)
	{
		for (int j = 0; j <= numControlPointsV; j++)
		{
			/* Evaluate the influence of control point i in u direction */
			float CDBi = CoxDeBoorRecursive(i, _du, _u, _uKnot);
			/* Evaluate the influence of control point j in v direction */
			float CDBj = CoxDeBoorRecursive(j, _dv, _v, _vKnot);
			/* Calculate the weight of the control point */
			glm::vec3 controlPointWeight = _controlPoints[i][j] * CDBi * CDBj;
			/* Sum of weight * control point */
			surfacePoint += controlPointWeight;
		}
	}
	return surfacePoint;
}

glm::vec3 BSpline::evaluateBSplineCurve(float _u, int _du, const std::vector<float>& _uKnot, const std::vector<glm::vec3>& _controlPoints)
{
    int numControlPoints = _controlPoints.size() - 1;

    int span = 0;
    while (_u >= _uKnot[span + _du + 1] && span < numControlPoints - _du) {
        span++;
    }

    /* Evaluate the B-spline basis functions using Cox-de Boor recursion */
    std::vector<float> basisFunctions(_du + 1);
    for (int j = 0; j <= _du; ++j) {
        basisFunctions[j] = CoxDeBoorRecursive(span - _du + j, _du, _u, _uKnot);
    }

    /* Evaluate the B-spline curve point */
    glm::vec3 curvePoint(0.0f);
    for (int j = 0; j <= _du; ++j) {
           curvePoint += basisFunctions[j] * _controlPoints[span - _du + j];
    }

    return curvePoint;
}


// Calculation for B-Spline surface normal
glm::vec3 BSpline::evaluateBSplineNormal(float _u, float _v, int _du, int _dv, int _UResolution, int _VResolution, 
	const std::vector<float>& _uKnot, 
	const std::vector<float>& _vKnot, 
	const std::vector<std::vector<glm::vec3>>& _controlPoints)
{
	glm::vec3 P = BSpline::evaluateBSplineSurface(_u, _v, _du, _dv, _uKnot, _vKnot,
		_controlPoints);
	glm::vec3 Pu = BSpline::evaluateBSplineSurface(_u + 1.0 / _UResolution, _v, _du, _dv, _uKnot, _vKnot,
		_controlPoints);
	glm::vec3 Pv = BSpline::evaluateBSplineSurface(_u, _v + 1.0 / _VResolution, _du, _dv, _uKnot, _vKnot,
		_controlPoints);

	/* Tangents */
	glm::vec3 Tu = Pu - P;
	glm::vec3 Tv = Pv - P;

	/* Normal */
	glm::vec3 N = glm::normalize(glm::cross(Tu, Tv));

	return N;
}

// Recursive Cox De Boor algorithm
float BSpline::CoxDeBoorRecursive(int _i, int _d, float _uv, const std::vector<float>& _knotVector)
{
	if (_d == 0)
	{
		return (_knotVector[_i] <= _uv && _uv < _knotVector[_i + 1]) ? 1.0f : 0.0f;
	}
	else
	{
		float left = 0.0f;
		float right = 0.0f;

		float leftDenominator = _knotVector[_i + _d] - _knotVector[_i];
		if (leftDenominator != 0.0f)
		{
			left = (_uv - _knotVector[_i]) / leftDenominator * CoxDeBoorRecursive(_i, _d - 1, _uv, _knotVector);
		}

		float rightDenominator = _knotVector[_i + _d + 1] - _knotVector[_i + 1];
		if (rightDenominator != 0.0f)
		{
			right = (_knotVector[_i + _d + 1] - _uv) / rightDenominator * CoxDeBoorRecursive(_i + 1, _d - 1, _uv, _knotVector);
		}

		return left + right;
	}
}

float Barycentric::getfriction(Vertex& _v1, Vertex& _v2, Vertex& _v3)
{
	return (_v1.mFriction + _v2.mFriction + _v3.mFriction) / 3;
}

/* Getting height from the objects */
float Barycentric::GetHeight(glm::vec3& _p1, glm::vec3& _p2, glm::vec3& _p3,  glm::vec3 _barycode)
{
	return(_p1.y * _barycode.x + _p2.y * _barycode.y + _p3.y * _barycode.z);
}

glm::vec3 Barycentric::GetNormal(glm::vec3& _p1, glm::vec3& _p2, glm::vec3& _p3)
{
	glm::vec3 p12 = _p2 - _p1;
	glm::vec3 p13 = _p3 - _p1;

	glm::vec3 n123 = glm::cross(p12, p13);

	glm::vec3 normalizedN = glm::normalize(n123);
	normalizedN *= glm::vec3(-1.f);
	return normalizedN;

}

bool Barycentric::BarycentricCord(Spheres _object, Cube _surface, float& _height, glm::vec3& _normal, float& _friction)
{
	for (auto Triangle : _surface.mIndices)
	{
		glm::vec3 barycord = Getbarycord(
			_surface.mVertices[Triangle.mIndex1].mPosition * _surface.mSize,
			_surface.mVertices[Triangle.mIndex2].mPosition * _surface.mSize, 
		 	_surface.mVertices[Triangle.mIndex3].mPosition * _surface.mSize, 
			_object.mPosition);

		if (barycord.x == 0 &&
			barycord.y == 0 &&
			barycord.z == 0)
		{
			_object.mPosition = _object.GetPosition() + glm::vec3(0.1, 0, 0.1);
			BarycentricCord(_object, _surface, _height, _normal, _friction);
		}
			if (barycord.x > 0 && barycord.x < 1 && 
			barycord.y > 0 && barycord.y < 1 &&
			barycord.z > 0 && barycord.z < 1)
				{
					/* Calculating each triangles height position scaled with the size */
					glm::vec3 scaledPosition1 = _surface.mVertices[Triangle.mIndex1].mPosition * _surface.mSize;
					glm::vec3 scaledPosition2 = _surface.mVertices[Triangle.mIndex2].mPosition * _surface.mSize;
					glm::vec3 scaledPosition3 = _surface.mVertices[Triangle.mIndex3].mPosition * _surface.mSize;
					_height = GetHeight(scaledPosition1, scaledPosition2, scaledPosition3, barycord);
					_height += 1.f;

					_friction = getfriction(
						_surface.mVertices[Triangle.mIndex1],
						_surface.mVertices[Triangle.mIndex2],
						_surface.mVertices[Triangle.mIndex3]);

					/* Calculation of normals */
					_normal = GetNormal(
						_surface.mVertices[Triangle.mIndex1].mPosition, 
						_surface.mVertices[Triangle.mIndex2].mPosition, 
						_surface.mVertices[Triangle.mIndex3].mPosition);
					return true;
				}
	}
	return false;
}


/* Calculation for the barysentric coordinates */
glm::vec3 Barycentric::Getbarycord(glm::vec3 _p1, glm::vec3 _p2, glm::vec3 _p3, glm::vec3 _ballpoint)
{
	_p1.y = 0;
	_p2.y = 0;
	_p3.y = 0;
	_ballpoint.y = 0;
	glm::vec3 barcoords;

	glm::vec3 p12 = _p2 - _p1;
	glm::vec3 p13 = _p3 - _p1;

	glm::vec3 n123 = glm::cross(p12, p13);

	float area123 = n123.y;

	glm::vec3 obj1 = _p1 - _ballpoint;
	glm::vec3 obj2 = _p2 - _ballpoint;
	glm::vec3 obj3 = _p3 - _ballpoint;

	glm::vec3 subn1 = glm::cross(obj1, obj2);
	glm::vec3 subn2 = glm::cross(obj2, obj3);
	glm::vec3 subn3 = glm::cross(obj3, obj1);


	float subtriangle1 = subn2.y;

	float subtriangle2 = subn3.y;

	float subtriangle3 = subn1.y; 

	barcoords.x = subtriangle1 / area123; 
	barcoords.y = subtriangle2 / area123; 
	barcoords.z = subtriangle3 / area123; 

	return barcoords;
}


