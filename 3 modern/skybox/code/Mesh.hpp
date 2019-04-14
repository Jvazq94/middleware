/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
*                                                                             *
*  Started by Ángel on march of 2014                                          *
*                                                                             *
*  This is free software released into the public domain.                     *
*                                                                             *
*  angel.rodriguez@esne.edu                                                   *
*                                                                             *
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef MESH_HEADER
#define MESH_HEADER

#include <nanogui\opengl.h>
//#include <GL/glew.h>
#include <memory>
#include <iostream>
#include <Point.hpp>
#include <map>
#include "Color_Buffer.hpp"
#include "Color_Buffer_Rgba8888.hpp"
#include <Vector.hpp>
//#include <SFML/OpenGL.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>         // translate, rotate, scale, perspective
#include <glm/gtc/type_ptr.hpp> // value_ptr
#include "Camera.hpp"
#include "MeshData.hpp"
namespace example
{

	class   Color_Buffer_Rgba8888;
	typedef Color_Buffer_Rgba8888 Texture;
	using std::vector;
	using toolkit::Point4i;
	using toolkit::Point3f;
	using toolkit::Point2f;

	class Mesh
	{
	private:

		// Índices para indexar el array vbo_ids:

		enum
		{
			COORDINATES_VBO,
			TEXTURE_UVS_VBO,
			NORMALS_VBO,
			INDICES_IBO,
			VBO_COUNT
		};

		// Arrays de datos del cubo base:
		typedef Color_Buffer_Rgba8888 Color_Buffer;
		typedef Color_Buffer::Color   Color;
		typedef Point3f               Vertex;
		
		typedef vector<Vertex>      Vertex_Buffer;
		typedef vector<Point2f>      Text_Buffer;
		typedef vector<int>      Index_Buffer;
		typedef vector<Color>      Vertex_Colors;

		Vertex_Buffer     original_vertices;
		Vertex_Buffer     original_normals;
		Text_Buffer     tex_coord;
		Index_Buffer      original_indices;

		std::string error;

		std::map<std::string, std::shared_ptr< Mesh>> children;


		int numCubes = 0;

	public:
		///@param posicion de la mesh
		glm::vec3 position;
		///@param escala de la mesh
		glm::vec3 scale;
		///@param punerto al modelo de datos de la mesh
		MeshData * meshData;

	private:

		GLuint vbo_ids[VBO_COUNT];      // Ids de los VBOs que se usan
		GLuint vao_id;                  // Id del VAO del cubo

		bool   has_texture;
		GLuint texture_id;

	public:
		///\brief Mesh contructor del Mesh
		///@param texture_path path de textura del Mesh
		Mesh(glm::vec3 pos, glm::vec3 scale, bool rotar, const char * mesh_path, const char * texture_path = 0);
		///\brief ~Mesh destructor
		~Mesh();

	public:

		///\brief render renderiza todos los elementos del Mesh cada tick
		///@param camera la camara de la escena
		///@param model_view_matrix_id la matriz puede ser de la camara en el caso del padre o del padre en el caso ed los hijos
		///@param model_view model view de la camara o el padre
		///@param angle angulo para la rotacion
		void render(Camera camera, GLint model_view_matrix_id, GLint normal_matrix_id,  float angle);

		///\brief add se añaden los hijos al Mesh padre
		///@param mesh Mesh hijo
		void add(std::shared_ptr<Mesh> &mesh);



	private:
		///\brief load_texture leemos la textura del Mesh
		///@param texture_file_path path de textura del Mesh
		std::auto_ptr< Texture > load_texture(const char * texture_file_path);

	};

}

#endif