#include "pch.h"
#include "Spheres.h"
#include "../../Shaders/Shader.h"

glm::vec3& Spheres::GetPosition()
{
	return position;
}

glm::vec3& Spheres::GetScale()
{
	return size;
}

void Spheres::CreateSphere(glm::vec3 size, float segment, glm::vec3 pos, float mass,glm::vec3 speed, glm::vec3 color)
{
    GetScale() = size;
    GetPosition() = pos;
	velocity = speed;
    m = mass;

    Vertex v0 {glm::vec3(0.f, 0.f, 1.f), color};
    Vertex v1 {glm::vec3(1.f, 0.f, 0.f), color};
    Vertex v2 {glm::vec3(0.f, 1.f, 0.f), color};
    Vertex v3 {glm::vec3(-1.f, 0.f, 0.f), color};
    Vertex v4 {glm::vec3(0.f, -1.f, 0.f), color};
    Vertex v5 {glm::vec3(0.f, 0.f, -1.f), color};

	bVertices.emplace_back(v0);
    bVertices.emplace_back(v1);
    bVertices.emplace_back(v2);
    bVertices.emplace_back(v3);
    bVertices.emplace_back(v4);
    bVertices.emplace_back(v5);

    SubDivide(0, 1, 2, segment);
    SubDivide(0, 2, 3, segment);
    SubDivide(0, 3, 4, segment);
    SubDivide(0, 4, 1, segment);
    SubDivide(5, 2, 1, segment);
    SubDivide(5, 3, 2, segment);
    SubDivide(5, 4, 3, segment);
    SubDivide(5, 1, 4, segment);


	for (auto element : bIndices)
    {
		glm::vec3 normal = glm::cross(bVertices[element.index2].position - bVertices[element.index1].position,
                                      bVertices[element.index3].position - bVertices[element.index1].position);
        bVertices[element.index1].normal += glm::normalize(normal);
        bVertices[element.index2].normal += glm::normalize(normal);
        bVertices[element.index3].normal += glm::normalize(normal);
    }

	BindBuffer();
}

void Spheres::SubDivide(int index1, int index2, int index3, int n)
{
	if (n > 0)
	{
		glm::vec3 v1 = bVertices[index1].position + bVertices[index2].position;
		v1 = glm::normalize(v1);
        glm::vec3 v2 = bVertices[index1].position + bVertices[index3].position;
        v2 = glm::normalize(v2);
        glm::vec3 v3 = bVertices[index3].position + bVertices[index2].position;
        v3 = glm::normalize(v3);

        int index4 = bVertices.size();

		bVertices.emplace_back(v1, color);
		bVertices.emplace_back(v2, color);
		bVertices.emplace_back(v3, color);

		int index5 = index4 + 1;
        int index6 = index4 + 2;

        SubDivide(index1, index4, index5, n - 1);
        SubDivide(index3, index5, index6, n - 1);
        SubDivide(index2, index6, index4, n - 1);
        SubDivide(index6, index5, index4, n - 1);
	}
	else
	{
        int indice1 = bVertices.size();
		bVertices.emplace_back(glm::vec3{bVertices[index1].position}, color);
        int indice2 = bVertices.size();
		bVertices.emplace_back(glm::vec3{bVertices[index2].position}, color);
		int indice3 = bVertices.size();
		bVertices.emplace_back(glm::vec3{bVertices[index3].position}, color);

        bIndices.emplace_back(indice1, indice2, indice3);

	}
}

void Spheres::BindBuffer()
{
        // VAO
    glGenBuffers(1, &VBO);

    // VAO
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // VBO
    glGenBuffers(1, &EBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, bVertices.size() * sizeof(Vertex), bVertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, bIndices.size()*sizeof(Triangle), bIndices.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, position)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, Color)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, normal)));

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Spheres::DrawSphere()
{
	glm::mat4 model = glm::mat4(1.f);
    model = glm::translate(model, position);
    model = glm::scale(model, size);
    glUniformMatrix4fv(glGetUniformLocation(Shader::ShaderProgram, "modelMatrix"), 1, GL_FALSE, glm::value_ptr(model));
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, bIndices.size()*3, GL_UNSIGNED_INT, (void*)(0 * sizeof(unsigned int)));
    glBindVertexArray(0);
}

void Spheres::UpdatePos(float dt)
{
	position += velocity * dt;
	Collider.UpdatePosition(position);
}

void Spheres::AddCollider(glm::vec3 scale, ECollisionType collisionType, glm::vec3 offset)
{
	Collider = Collision(GetPosition()+offset, scale, offset, collisionType);
}