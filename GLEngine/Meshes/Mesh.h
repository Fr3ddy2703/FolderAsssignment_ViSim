#pragma once


struct Vertex
{
	glm::vec3 mPosition = glm::vec3(0.f);
	glm::vec3 mColor = glm::vec3(0.f);
	glm::vec3 mNormal = glm::vec3(0.f);
	Vertex(glm::vec3 _position, glm::vec3 _rgb) : mPosition(_position), mColor(_rgb)
	{}
};

struct Triangle
{
	Triangle(unsigned int _indice1, unsigned int _indice2, unsigned int _indice3): mIndex1(_indice1), mIndex2(_indice2), mIndex3(_indice3){}
	unsigned int mIndex1, mIndex2, mIndex3;
};


class Mesh
{
public:
	Mesh() = default;
	~Mesh() = default;
	std::vector<Vertex> mVertices;
	std::vector<Triangle> mIndices;
	glm::vec3 mPosition = glm::vec3(1);
	glm::vec3 mSize = glm::vec3(1);
};
