/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
*                                                                             *
*  Started by Ángel on march of 2014                                          *
*                                                                             *
*  This is free software released into the public domain.                     *
*                                                                             *
*  angel.rodriguez@esne.edu                                                   *
*                                                                             *
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#define TINYOBJLOADER_IMPLEMENTATION

#include "Mesh.hpp"

#include "Color_Buffer_Rgba8888.hpp"
#include <string>
#include <tiny_obj_loader.h>



extern "C"
{
#include <targa.h>
}

using namespace tinyobj;
using namespace std;
using namespace toolkit;
namespace example
{


	//\brief Mesh contructor del Mesh
	//@param texture_path path de textura del Mesh
	Mesh::Mesh(glm::vec3 pos, glm::vec3 scal, bool rotar, const char * mesh_path, const char * texture_path)
		:
		has_texture(false)
	{

		meshData = new MeshData();

		meshData->setPosition(pos);
		meshData->setScale(scal);
		meshData->setMashPath(mesh_path);
		meshData->setTexturePath(texture_path);
		meshData->setRotate(rotar);

		/*position = pos;
		scale = scal;*/
		attrib_t             attributes;
		vector< shape_t    > shapes;
		vector< material_t > materials;

		

		string obj_file_path = meshData->getMeshPath();


		string err;
		//Se comprueba la lectura del tinyobj
		if (!tinyobj::LoadObj(&attributes, &shapes, &materials, &error, obj_file_path.c_str()) || !error.empty())
		{
			return;
		}

		if (shapes.size() == 0) { error = string("There're no shapes in ") + obj_file_path; return; }
		if (attributes.vertices.size() == 0) { error = string("There're no vertices in ") + obj_file_path; return; }
		if (attributes.normals.size() == 0) { error = string("There're no normals in ") + obj_file_path; return; }


		vector<float>  vertices_aux(shapes[0].mesh.indices.size() * 3);
		vector<float>  text_coords(shapes[0].mesh.indices.size() * 2);






		size_t vMax = shapes[0].mesh.indices.size();

		original_vertices.resize(vMax);
		original_normals.resize(vMax);
		tex_coord.resize(vMax);

		//Se igualan los datos que necesitamos a los vectores auxiliares
		for (size_t i = 0; i < vMax; i++)
		{
			int nVertices = shapes[0].mesh.indices[i].vertex_index * 3;
			int nNormals = shapes[0].mesh.indices[i].normal_index * 3;
			int textCoords = shapes[0].mesh.indices[i].texcoord_index * 2;

			original_vertices[i].coordinates()[0] = attributes.vertices[nVertices + 0];
			original_vertices[i].coordinates()[1] = attributes.vertices[nVertices + 1];
			original_vertices[i].coordinates()[2] = attributes.vertices[nVertices + 2];

			original_normals[i].coordinates()[0] = attributes.normals[nNormals + 0];
			original_normals[i].coordinates()[1] = attributes.normals[nNormals + 1];
			original_normals[i].coordinates()[2] = attributes.normals[nNormals + 2];

			tex_coord[i].coordinates()[0] = attributes.texcoords[textCoords+ 0];
			tex_coord[i].coordinates()[1] = attributes.texcoords[textCoords + 1];
			
			original_indices.push_back(i);
		}

		meshData->setVertices(original_vertices);
		meshData->setNormals(original_normals);
		meshData->setIndices(original_indices);
		meshData->setTextureCoords(tex_coord);

		// Se generan índices para los VBOs del cubo:

		glGenBuffers(VBO_COUNT, vbo_ids);
		glGenVertexArrays(1, &vao_id);

		// Se activa el VAO del cubo para configurarlo:

		glBindVertexArray(vao_id);

		// Se suben a un VBO los datos de coordenadas y se vinculan al VAO:

		glBindBuffer(GL_ARRAY_BUFFER, vbo_ids[COORDINATES_VBO]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Point3f) * meshData->getVertices().size(),meshData->getVertices().data(), GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

		// Se suben a un VBO los datos de color y se vinculan al VAO:

		glBindBuffer(GL_ARRAY_BUFFER, vbo_ids[TEXTURE_UVS_VBO]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Point2f) * meshData->getTextureCoords().size(), meshData->getTextureCoords().data(), GL_STATIC_DRAW);

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

		//Normales
		glBindBuffer(GL_ARRAY_BUFFER, vbo_ids[NORMALS_VBO]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Point3f)* original_normals.size(), meshData->getNormals().data(), GL_STATIC_DRAW);

		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);

		// Se suben a un IBO los datos de índices:

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_ids[INDICES_IBO]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * meshData->getIndices().size(), meshData->getIndices().data(), GL_STATIC_DRAW);

		glBindVertexArray(0);

		// Se carga la textura si es necesario:

		if (meshData->getTexturePath() != 0)
		{
			std::auto_ptr< Texture > texture = load_texture(meshData->getTexturePath());

			has_texture = texture.get() != 0;

			if (has_texture)
			{
				// Se habilitan las texturas, se genera un id para un búfer de textura,
				// se selecciona el búfer de textura creado y se configuran algunos de
				// sus parámetros:

				glEnable(GL_TEXTURE_2D);
				glGenTextures(1, &texture_id);
				glBindTexture(GL_TEXTURE_2D, texture_id);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

				// Se suben los colores de la textura a la memoria de vídeo:

				glTexImage2D
				(
					GL_TEXTURE_2D,
					0,
					GL_RGBA,
					texture->get_width(),
					texture->get_height(),
					0,
					GL_RGBA,
					GL_UNSIGNED_BYTE,
					texture->colors()
				);
			}
		}
	}

	//\brief ~Mesh destructor
	Mesh::~Mesh()
	{
		if (has_texture)
		{
			glDeleteTextures(1, &texture_id);
		}

		glDeleteVertexArrays(1, &vao_id);
		glDeleteBuffers(VBO_COUNT, vbo_ids);
	}

	//\brief render renderiza todos los elementos del Mesh cada tick
	//@param camera la camara de la escena
	//@param model_view_matrix_id la matriz puede ser de la camara en el caso del padre o del padre en el caso ed los hijos
	//@param model_view model view de la camara o el padre
	//@param angle angulo para la rotacion
	void Mesh::render(Camera camera, GLint model_view_matrix_id, GLint normal_matrix_id, float angle)
	{

		// Se selecciona la textura si es necesario:

		if (has_texture)
		{
			glBindTexture(GL_TEXTURE_2D, texture_id);
		}
		glm::mat4 model_view_matrix;// = camera.get_model_view();
		model_view_matrix = glm::translate(model_view_matrix, meshData->getPosition());

		if(meshData->getRotate())
		model_view_matrix = glm::rotate(model_view_matrix, angle, glm::vec3(.0f, 1.0f, 0.0f));

		model_view_matrix = glm::scale(model_view_matrix, meshData->getScale());

		glUniformMatrix4fv(model_view_matrix_id, 1, GL_FALSE, glm::value_ptr(model_view_matrix));

		glm::mat4 normal_matrix = glm::transpose(glm::inverse(model_view_matrix));

		glUniformMatrix4fv(normal_matrix_id, 1, GL_FALSE, glm::value_ptr(normal_matrix));

		// Se selecciona el VAO que contiene los datos del objeto y se dibujan sus elementos:

		glBindVertexArray(vao_id);
		glDrawElements(GL_TRIANGLES, sizeof(Index_Buffer) * meshData->getIndices().size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		//Recorremos todos los hijos para llamar a sus renders


	}
	
	//\brief load_texture leemos la textura del Mesh
	//@param texture_file_path path de textura del Mesh
	std::auto_ptr< Texture > Mesh::load_texture(const char * texture_file_path)
	{
		std::auto_ptr< Texture > texture;
		tga_image                loaded_image;

		if (tga_read(&loaded_image, texture_file_path) == TGA_NOERR)
		{
			// Si se ha podido cargar la imagen desde el archivo, se crea un búfer para una textura:

			texture.reset(new Texture(loaded_image.width, loaded_image.height));

			// Se convierte el formato de píxel de la imagen cargada a RGBA8888:

			tga_convert_depth(&loaded_image, texture->bits_per_color());
			tga_swap_red_blue(&loaded_image);

			// Se copian los pixels del búfer de la imagen cargada al búfer de la textura:

			Texture::Color * loaded_image_pixels = reinterpret_cast< Texture::Color * >(loaded_image.image_data);
			Texture::Color * loaded_image_pixels_end = loaded_image_pixels + loaded_image.width * loaded_image.height;
			Texture::Color * image_buffer_pixels = texture->colors();

			while (loaded_image_pixels <  loaded_image_pixels_end)
			{
				*image_buffer_pixels++ = *loaded_image_pixels++;
			}

			tga_free_buffers(&loaded_image);
		}

		return (texture);
	}

	//\brief add se añaden los hijos al Mesh padre
	//@param mesh Mesh hijo
	void Mesh::add(std::shared_ptr<Mesh> &mesh) {

		std::string name = "mesh" + std::to_string(numCubes);
		children[name] = mesh;
		numCubes++;
	}
}