#include "pch.h"
#include "MathFunctions.h"




//int MathFunctions::findKnotInterval(float _x)
//{
//	int my = n - 1;
//
//
//	return my;
//}
//
//void MathFunctions::deBoorsAlgorithm(float _x)
//{
//}


// Calculation for B-Spline surface
glm::vec3 BSpline::evaluateBSplineSurface(float _u, float _v, int _du, int _dv, const std::vector<float>& _uKnot, const std::vector<float>& _vKnot, const std::vector<std::vector<glm::vec3>>& _controlPoints)
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

// Calculation for B-Spline surface normal
glm::vec3 BSpline::evaluateBSplineNormal(float _u, float _v, int _du, int _dv, int _UResolution, int _VResolution, const std::vector<float>& _uKnot, const std::vector<float>& _vKnot, const std::vector<std::vector<glm::vec3>>& _controlPoints)
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

