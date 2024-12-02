#include "pch.h"
#include "BSplineSurface.h"
#include "../../MathFunctions/MathFunctions.h"
#include "../../Shaders/Shader.h"

/* Creation of B-spline */
void BSplineSurface::CreateBSplineSurface(int _UResolution, int _VResolution, int _du, int _dv, const std::vector<float>& _uKnot, const std::vector<float>& _vKnot, const std::vector<std::vector<glm::vec3>>& _controlPoints)
{
        for (int i = 0; i < _UResolution; i++)
    {
		double u = (double)i / (_UResolution - 1);
        for (int j = 0; j < _VResolution; j++)
        {
	        double v = (double)j / (_VResolution - 1);

			glm::vec3 surfacePoint = BSpline::evaluateBSplineSurface(u, v, _du, _dv, _uKnot,
                _vKnot, _controlPoints);

			glm::vec3 surfacePointNormal = BSpline::evaluateBSplineNormal(u, v, _du, _dv, _UResolution, _VResolution, _uKnot,
				_vKnot, _controlPoints);

			mVertices.push_back(Vertex(surfacePoint, surfacePointNormal));
        }
    }
    for (int i = 0; i < _UResolution - 1; i++)
    {
        for (int j = 0; j < _VResolution - 1; ++j)
        {
			int topleft = i * _VResolution + j;
			int topright = topleft + 1;
			int bottomleft = (i + 1) * _VResolution + j;
			int bottomright = bottomleft + 1;

			mIndices.push_back(Triangle(topleft, bottomleft, topright));
			mIndices.push_back(Triangle(topright, bottomleft, bottomright));
        }
    }
	BindBuffer();
}

void BSplineSurface::Draw()
{
    glm::mat4 model = glm::mat4(1.f);
    model = glm::translate(model, mPosition);
    model = glm::scale(model, mSize);
    glUniformMatrix4fv(glGetUniformLocation(Shader::ShaderProgram, "modelMatrix"), 1, GL_FALSE, glm::value_ptr(model));
    glBindVertexArray(mVAO);
    glDrawElements(GL_TRIANGLES, mIndices.size()*3, GL_UNSIGNED_INT, (void*)(0 * sizeof(unsigned int)));
    glBindVertexArray(0);
}

void BSplineSurface::BindBuffer()
{
    // VAO
    glGenBuffers(1, &mVBO);

    // VAO
    glGenVertexArrays(1, &mVAO);
    glBindVertexArray(mVAO);

    // VBO
    glGenBuffers(1, &mEBO);

    glBindBuffer(GL_ARRAY_BUFFER, mVBO);
    glBufferData(GL_ARRAY_BUFFER, mVertices.size() * sizeof(Vertex), mVertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mIndices.size()*sizeof(Triangle), mIndices.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, mPosition)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, mColor)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, mNormal)));

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}


