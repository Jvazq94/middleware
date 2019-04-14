#pragma once	
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
*                                                                             *
*  Started by Ángel on april of 2014                                          *
*                                                                             *
*  This is free software released into the public domain.                     *
*                                                                             *
*  angel.rodriguez@esne.edu                                                   *
*                                                                             *
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef VIEW_HEADER
#define VIEW_HEADER

#include <nanogui\opengl.h>
#include <string>
//#include <GL/glew.h>						// Debe incluirse antes que gl.h
#include "Camera.hpp"
#include "Fragment_Shader.hpp"
#include "Shader_Program.hpp"
#include "Mesh.hpp"
#include <Vector.hpp>
#include <glm/glm.hpp>                          // vec3, vec4, ivec4, mat4
#include <glm/gtc/matrix_transform.hpp>         // translate, rotate, scale, perspective
#include <glm/gtc/type_ptr.hpp>                 // value_ptr
#include <map>
#include <rapidxml.hpp>
#include <fstream>

using namespace rapidxml;
using namespace std;

namespace example
{

	class View
	{
	private:


		typedef rapidxml::xml_node<> xmlNode;
		typedef rapidxml::xml_attribute<> xmlAttrib;

		Camera camera;
		
		map<string, shared_ptr<Mesh>> meshMap;


		int width_original;
		int height_original;


		static const std::string vertex_shader_code;
		static const std::string fragment_shader_code;
		static const std::string effect_vertex_shader_code;
		static const std::string effect_fragment_shader_code;

		GLint model_view_matrix_id;
		GLint projection_matrix_id;
		GLint      normal_matrix_id;

		GLuint framebuffer_id;
		GLuint depthbuffer_id;
		GLuint out_texture_id;

		GLuint triangle_vbo0;
		GLuint triangle_vbo1;
		GLuint cube_program_id;
		GLuint effect_program_id;
		float light_direction[4];
		float angle;
		
		Shader_Program shader_program;


		string meshName;
		glm::vec3 meshPosition;
		glm::vec3 meshScale;
		const char * meshPath;
		const char * textMeshPath;
		string meshString;
		string textString;
		bool rot = false;






	public:
		///\brief View construcotr de View
		///@param with ancho de la ventana
		///@param height alto de la ventada
		View(int width, int height);
		///\brief update funcion que se ejecuta cada tick
		void update();
		///\brief render renderiza todos los elementos de la escena
		void render();

		///\brief resize re-setean todos los parametros
		///@param with ancho de la ventana
		///@param height alto de la ventana
		void resize(int width, int height);
		///\brief configure_light configura la luz de la escena
		void configure_light();
		///\brief createMesh crea una mesh nueva y la añade a el map de meshes
		///@param name nombre de la mesh
		///@param position posicion de la mesh
		///@param scale escala de la mesh
		///@param rotar si las mesh rota o no
		///@param mesh_path el path de la mesh
		///@param texture_path el path de la textura
		void createMesh(string name, glm::vec3 position, glm::vec3 scale,bool rotar, string mesh_path, string texture_path = 0);

	public:

		///\brief on_key se llama cuando se pulsa alguna tecla para movel la camara
		void on_key();

		///\brief loadXML lee el XML
		bool loadXML(const string & scene);
		///\brief parseScene parsea la escena del XML
		bool parseScene(xmlNode * sceneNode);
		///\brief parseEntities parsea las entidades del XML
		bool parseEntities(xmlNode * entitiesNode);
		///\brief parseComponents parsea los componentes del XML
		bool parseComponents(xmlNode * componentsNode);
		///\brief saveXML guarda los datos de la escena en un XML
		void saveXML(const string & sceneFilePath);
		///\brief deleteALL borra todos los componentes de la escena
		void deleteAll() {
			meshMap.clear();
		}

	};

}

#endif
