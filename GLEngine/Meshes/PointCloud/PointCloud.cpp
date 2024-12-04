#include "pch.h"
#include "PointCloud.h"
#include "../../Shaders/shader.h"


/* Reading and creating the pointcloud for the project */
void PointCloud::CreatePointCloudFromFile(const char* _fileDirectory, float _scaleFactor)
{
   // Open the file
    std::ifstream file(_fileDirectory);
    if (!file.is_open())
    {
        std::cout << "Failed to open file: " << _fileDirectory << "\n";
        return; // Exit if the file cannot be opened
    }

    std::string line;
    int lineNumber = 0;
    
    // Read the first line, assuming it contains metadata (e.g., total number of points)
    if (std::getline(file, line))
    {
        lineNumber++;
        std::istringstream iss(line);
        int totalPoints;
        
        // Check if the first line is the total number of points
        if (!(iss >> totalPoints))
        {
            std::cout << "Failed to parse the total number of points from the first line: " << line << "\n";
            return; // Exit if the first line is not valid
        }
        std::cout << "Total points: " << totalPoints << "\n"; // Optionally print the total points
    }

    
    /* Setting a terrain offset so the pointcloud spawns in a preferable place for the camera */
    glm::vec3 terrainOffset(621600.f, 676.f,6759897.f);
    // Now continue reading the rest of the lines as the point cloud data
    while (std::getline(file, line))
    {
        lineNumber++;
        std::istringstream iss(line);
        float x, y, z;

        // Try to parse the x, y, z coordinates
        if (!(iss >> x >> y >> z))
        {
            std::cout << "Failed to parse line " << lineNumber << ": " << line << "\n";
            continue; // Skip this line if parsing fails
        }
        // Scale the coordinates and create a vertex
        glm::vec3 colorf = glm::vec3(Color::Red);
        /* Swapping the y and z coordinates so the terrain will be horizontal and not vertical */
		glm::vec3 vpos = (glm::vec3(x, z, y) - terrainOffset) * _scaleFactor;

        mVertices.push_back(Vertex(vpos, colorf, 0));


        mIndices.push_back(Triangle(mVertices.size() - 1, mVertices.size() - 1, mVertices.size() - 1));
    }
    BindBuffer();

    std::cout << "Finished reading point cloud from file, processed " << lineNumber - 1 << " data lines\n"; // Subtract 1 for metadata line

}

void PointCloud::Draw()
{
	glm::mat4 model = glm::mat4(1.f);
    model = glm::translate(model, mPosition);
    model = glm::scale(model, mSize);
    glUniformMatrix4fv(glGetUniformLocation(Shader::ShaderProgram, "modelMatrix"), 1, GL_FALSE, glm::value_ptr(model));
    glBindVertexArray(mVAO);
    glDrawElements(GL_POINTS, mIndices.size()*3, GL_UNSIGNED_INT, (void*)(0 * sizeof(unsigned int)));
    glBindVertexArray(0);
}


/* Binding buffers */
void PointCloud::BindBuffer()
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

