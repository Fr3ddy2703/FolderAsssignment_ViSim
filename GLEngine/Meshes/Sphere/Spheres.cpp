#include "pch.h"
#include "Spheres.h"
#include "../../Shaders/shader.h"

glm::vec3& Spheres::GetPosition()
{
	return mPosition;
}

glm::vec3& Spheres::GetScale()
{
	return mSize;
}

void Spheres::CreateSphere(glm::vec3 _size, float _segment, glm::vec3 _pos, float _mass,glm::vec3 _speed, glm::vec3 _color)
{
    GetScale() = _size;
    GetPosition() = _pos;
	mVelocity = _speed;
    mMass = _mass;

    Vertex v0 {glm::vec3(0.f, 0.f, 1.f), _color};
    Vertex v1 {glm::vec3(1.f, 0.f, 0.f), _color};
    Vertex v2 {glm::vec3(0.f, 1.f, 0.f), _color};
    Vertex v3 {glm::vec3(-1.f, 0.f, 0.f), _color};
    Vertex v4 {glm::vec3(0.f, -1.f, 0.f), _color};
    Vertex v5 {glm::vec3(0.f, 0.f, -1.f), _color};

	mVertices.emplace_back(v0);
    mVertices.emplace_back(v1);
    mVertices.emplace_back(v2);
    mVertices.emplace_back(v3);
    mVertices.emplace_back(v4);
    mVertices.emplace_back(v5);

    SubDivide(0, 1, 2, _segment);
    SubDivide(0, 2, 3, _segment);
    SubDivide(0, 3, 4, _segment);
    SubDivide(0, 4, 1, _segment);
    SubDivide(5, 2, 1, _segment);
    SubDivide(5, 3, 2, _segment);
    SubDivide(5, 4, 3, _segment);
    SubDivide(5, 1, 4, _segment);


	for (auto element : mIndices)
    {
		glm::vec3 normal = glm::cross(mVertices[element.mIndex2].mPosition - mVertices[element.mIndex1].mPosition,
                                      mVertices[element.mIndex3].mPosition - mVertices[element.mIndex1].mPosition);
        mVertices[element.mIndex1].mNormal += glm::normalize(normal);
        mVertices[element.mIndex2].mNormal += glm::normalize(normal);
        mVertices[element.mIndex3].mNormal += glm::normalize(normal);
    }
    BindBuffer();
}

void Spheres::SubDivide(int _index1, int _index2, int _index3, int _n)
{
	if (_n > 0)
	{
		glm::vec3 v1 = mVertices[_index1].mPosition + mVertices[_index2].mPosition;
		v1 = glm::normalize(v1);
        glm::vec3 v2 = mVertices[_index1].mPosition + mVertices[_index3].mPosition;
        v2 = glm::normalize(v2);
        glm::vec3 v3 = mVertices[_index3].mPosition + mVertices[_index2].mPosition;
        v3 = glm::normalize(v3);

        int index4 = mVertices.size();

		mVertices.emplace_back(v1, mColor);
		mVertices.emplace_back(v2, mColor);
		mVertices.emplace_back(v3, mColor);

		int index5 = index4 + 1;
        int index6 = index4 + 2;

        SubDivide(_index1, index4, index5, _n - 1);
        SubDivide(_index3, index5, index6, _n - 1);
        SubDivide(_index2, index6, index4, _n - 1);
        SubDivide(index6, index5, index4, _n - 1);
	}
	else
	{
        int indice1 = mVertices.size();
		mVertices.emplace_back(glm::vec3{mVertices[_index1].mPosition}, mColor);
        int indice2 = mVertices.size();
		mVertices.emplace_back(glm::vec3{mVertices[_index2].mPosition}, mColor);
		int indice3 = mVertices.size();
		mVertices.emplace_back(glm::vec3{mVertices[_index3].mPosition}, mColor);

        mIndices.emplace_back(indice1, indice2, indice3);

	}
}

void Spheres::DrawSphere()
{
	glm::mat4 model = glm::mat4(1.f);
    model = glm::translate(model, mPosition);
    model = glm::scale(model, mSize);

    glUniformMatrix4fv(glGetUniformLocation(Shader::ShaderProgram, "modelMatrix"), 1, GL_FALSE, glm::value_ptr(model));
    glBindVertexArray(mVAO);
    glDrawElements(GL_TRIANGLES, mIndices.size()*3, GL_UNSIGNED_INT, (void*)(0 * sizeof(unsigned int)));
    glBindVertexArray(0);
}

void Spheres::UpdatePos(float _dt)
{
    mVelocity += mAcceleration * _dt;
	mPosition += mVelocity * _dt;
	mCollider.UpdatePosition(mPosition);
    mAcceleration = glm::vec3(0);
}

void Spheres::AddCollider(glm::vec3 _scale, ECollisionType _collisionType, glm::vec3 _offset)
{
	mCollider = Collision(GetPosition()+_offset, _scale, _offset, _collisionType);
}

void Spheres::BindBuffer()
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
