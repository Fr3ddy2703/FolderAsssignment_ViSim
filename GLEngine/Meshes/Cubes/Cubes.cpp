#include "pch.h"
#include "Cubes.h"
#include "../../Shaders/shader.h"

/* Creation of Cubes */
void Cube::CreateCube(glm::vec3 _size, glm::vec3 _pos, glm::vec3 _color)
{
    GetPosition() = mPosition;
    GetScale() = mSize;

    Vertex v0{glm::vec3(0.f, 0.f, 0.f), _color}; /* Front-Bot-left */
    Vertex v1{glm::vec3(1.f, 0.f, 0.f), _color}; /* Front-Bot-right */
    Vertex v2{glm::vec3(1.f, 1.f, 0.f), _color}; /* Front-Top-right */
    Vertex v3{glm::vec3(0.f, 1.f, 0.f), _color}; /* Front-Top-left */
    Vertex v4{glm::vec3(0.f, 0.f, -1.f), _color}; /* Back-Bot-left */
    Vertex v5{glm::vec3(1.f, 0.f, -1.f), _color}; /* Back-Bot-right */
    Vertex v6{glm::vec3(1.f, 1.f, -1.f), _color}; /* Back-Top-right */
    Vertex v7{glm::vec3(0.f, 1.f, -1.f), _color}; /* Back-Top-left */

    mVertices.emplace_back(v0);
    mVertices.emplace_back(v1);
    mVertices.emplace_back(v2);
    mVertices.emplace_back(v3);
    mVertices.emplace_back(v4);
    mVertices.emplace_back(v5);
    mVertices.emplace_back(v6);
    mVertices.emplace_back(v7);

    /* Front */
    mIndices.emplace_back(0, 1, 2);
    mIndices.emplace_back(2, 3, 0);

    /* Right */
    mIndices.emplace_back(1, 5, 6);
    mIndices.emplace_back(6, 2, 1);

    /* Left */
    mIndices.emplace_back(0, 3, 7);
    mIndices.emplace_back(7, 4, 0);

    /* Back */
    mIndices.emplace_back(4, 7, 6);
    mIndices.emplace_back(6, 5, 4);

    /* Top */
    mIndices.emplace_back(3, 2, 6);
    mIndices.emplace_back(6, 7, 3);

    /* Bottom */
    mIndices.emplace_back(0, 4, 5);
    mIndices.emplace_back(5, 1, 0);

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

/* Creation of Surface based on the point cloud */
void Cube::CreateSurfaceFromPointCLoud(std::vector<Vertex> _vertices, std::vector<Triangle> _indices)
{
	mVertices = _vertices;
    mIndices = _indices;
    BindBuffer();
}

glm::vec3& Cube::GetPosition()
{
    return mPosition;
}

glm::vec3& Cube::GetScale()
{
	return mSize;
}


void Cube::AddCollider(glm::vec3 _scale, ECollisionType _collisionType, glm::vec3 _offset)
{
    	mCollider = Collision(GetPosition()+_offset, _scale, _offset, _collisionType);
}

void Cube::Draw()
{
	glm::mat4 model = glm::mat4(1.f);
    model = glm::translate(model, mPosition);
    model = glm::scale(model, mSize);
    glUniformMatrix4fv(glGetUniformLocation(Shader::ShaderProgram, "modelMatrix"), 1, GL_FALSE, glm::value_ptr(model));
    glBindVertexArray(mVAO);
    glDrawElements(GL_TRIANGLES, mIndices.size()*3, GL_UNSIGNED_INT, (void*)(0 * sizeof(unsigned int)));
    glBindVertexArray(0);
}

void Cube::BindBuffer()
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