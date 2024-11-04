#pragma once


struct Vertex
{
	glm::vec3 position = glm::vec3(0.f);
	glm::vec3 Color = glm::vec3(0.f);
	glm::vec3 normal = glm::vec3(0.f);
	Vertex(glm::vec3 position, glm::vec3 rgb) : position(position), Color(rgb)
	{
	}
};

struct Triangle
{
	Triangle(unsigned int indice1, unsigned int indice2, unsigned int indice3): index1(indice1), index2(indice2), index3(indice3){}
	unsigned int index1, index2, index3;
};


class Mesh
{
public:
	std::vector<Vertex> Vertices;
	std::vector<Triangle> Indices;
	glm::vec3 position = glm::vec3(1);
	glm::vec3 size = glm::vec3(1);
};
