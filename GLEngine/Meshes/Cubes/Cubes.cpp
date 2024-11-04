#include "pch.h"
#include "Cubes.h"
#include "../../Shaders/Shader.h"

void Cube::CreateCube(glm::vec3 size, glm::vec3 pos, glm::vec3 color)
{
    GetPosition() = pos;
    GetScale() = size;

    Vertex v0{glm::vec3(0.f, 0.f, 0.f), color}; /* Front-Bot-left */
    Vertex v1{glm::vec3(1.f, 0.f, 0.f), color}; /* Front-Bot-right */
    Vertex v2{glm::vec3(1.f, 1.f, 0.f), color}; /* Front-Top-right */
    Vertex v3{glm::vec3(0.f, 1.f, 0.f), color}; /* Front-Top-left */
    Vertex v4{glm::vec3(0.f, 0.f, -1.f), color}; /* Back-Bot-left */
    Vertex v5{glm::vec3(1.f, 0.f, -1.f), color}; /* Back-Bot-right */
    Vertex v6{glm::vec3(1.f, 1.f, -1.f), color}; /* Back-Top-right */
    Vertex v7{glm::vec3(0.f, 1.f, -1.f), color}; /* Back-Top-left */

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
        glm::vec3 normal = glm::cross(mVertices[element.index2].position - mVertices[element.index1].position,
                                      mVertices[element.index3].position - mVertices[element.index1].position);
        mVertices[element.index1].normal += glm::normalize(normal);
        mVertices[element.index2].normal += glm::normalize(normal);
        mVertices[element.index3].normal += glm::normalize(normal);
    }

    BindBuffer();
}

glm::vec3& Cube::GetPosition()
{
    return position;
}

glm::vec3& Cube::GetScale()
{
	return size;
}

void Cube::BindBuffer()
{
    // VAO
    glGenBuffers(1, &VBO);

    // VAO
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // VBO
    glGenBuffers(1, &EBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, mVertices.size() * sizeof(Vertex), mVertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mIndices.size()*sizeof(Triangle), mIndices.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, position)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, Color)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, normal)));

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}


void Cube::AddCollider(glm::vec3 scale, ECollisionType collisionType, glm::vec3 offset)
{
    	Collider = Collision(GetPosition()+offset, scale, offset, collisionType);
}

void Cube::Draw()
{
    glm::mat4 model = glm::mat4(1.f);
    model = glm::translate(model, position);
    model = glm::scale(model, size);
    glUniformMatrix4fv(glGetUniformLocation(Shader::ShaderProgram, "modelMatrix"), 1, GL_FALSE, glm::value_ptr(model));
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, mIndices.size()*3, GL_UNSIGNED_INT, (void*)(0 * sizeof(unsigned int)));
    glBindVertexArray(0);
}