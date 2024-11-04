#include "pch.h"
#include "MathFunctions.h"




//int MathFunctions::findKnotInterval(float x)
//{
//	int my = n - 1;
//
//
//	return my;
//}
//
//void MathFunctions::deBoorsAlgorithm(float x)
//{
//}

//std::vector<glm::vec3> MathFunctions::LASFileToPoints(const char* _fileDirectory)
//{
//  //  // create the reader
//  //  laszip_POINTER laszip_reader;
//  //  if (laszip_create(&laszip_reader))
//		//std::cout << "DLL ERROR: creating laszip reader" << std::endl;
//
//  //  // open the reader
//  //  laszip_BOOL is_compressed = true;
//  //  if (laszip_open_reader(laszip_reader, _fileDirectory, &is_compressed))
//		//std::cout << "DLL ERROR: opening laszip reader for '" << _fileDirectory << "'" << std::endl;
//
//  //  // get a pointer to the header of the reader that was just populated
//  //  laszip_header* header;
//  //  if (laszip_get_header_pointer(laszip_reader, &header))
//		//std::cout << "DLL ERROR: getting header pointer from laszip reader" << std::endl;
//  //  
//  //  // get a pointer to the points that will be read
//  //  laszip_point* point;
//  //  if (laszip_get_point_pointer(laszip_reader, &point))
//		//std::cout << "DLL ERROR: getting point pointer from laszip reader" << std::endl;
//
//  //  laszip_I64 numTotalPoints = (header->number_of_point_records ? header->number_of_point_records : header->extended_number_of_point_records);
//
//  //  // read the points
//  //  std::vector<glm::vec3> rPoints;
//  //  for (int i = 0; i < numTotalPoints; i++)
//  //  {
//  //      // Read the next point
//  //      if (laszip_read_point(laszip_reader))
//		//	std::cout << "DLL ERROR: reading point " << i << std::endl;
//
//  //      // Apply the scaling factors from the LAS header to get actual coordinates
//  //      float x = (float)(point->X * header->x_scale_factor + header->x_offset);
//  //      float y = (float)(point->Y * header->y_scale_factor + header->y_offset);
//  //      float z = (float)(point->Z * header->z_scale_factor + header->z_offset);
//
//  //      glm::vec3 glmPoint = glm::vec3(x, y, z);
//  //      rPoints.push_back(glmPoint);
//  //  }
//
//  //  // Clean up the LASzip reader
//  //  if (laszip_close_reader(laszip_reader))
//		//std::cout << "DLL ERROR: closing laszip reader" << std::endl;
//  //  if (laszip_destroy(laszip_reader))
//		//std::cout << "DLL ERROR: destroying laszip reader" << std::endl;
//
//  //  return rPoints;
//}

//void MathFunctions::LASFileToCustomFileOfPoints(const char* _fileDirectoryInn, const char* _fileDirectoryOut)
//{
//    std::vector<glm::vec3> filePointCloudPoints = std::move(LASFileToPoints(_fileDirectoryInn));
//
//    std::ofstream fileOut(_fileDirectoryOut);
//
//    if(fileOut.is_open())
//    {
//        // Reset file
//        fileOut.clear();
//
//        // First line is number of points
//        fileOut << filePointCloudPoints.size() << "\n";
//
//        // The rest is the points
//        for (int i = 0; i < filePointCloudPoints.size(); i++)
//        {
//            fileOut << filePointCloudPoints[i].x << ", " << filePointCloudPoints[i].z << ", " << filePointCloudPoints[i].y << "\n";
//        }
//
//        fileOut.close();
//    } else {
//		std::cout << "Error opening file for custom LAS conversion" << std::endl;
//    }
//}

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

