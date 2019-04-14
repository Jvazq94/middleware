
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
*                                                                             *
*  Started by Ángel on april of 2014                                          *
*                                                                             *
*  This is free software released into the public domain.                     *
*                                                                             *
*  angel.rodriguez@esne.edu                                                   *
*                                                                             *
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <nanogui\opengl.h>
#include "View.hpp"
#include <iostream>
#include <cassert>
#include <glm/glm.hpp>                          // vec3, vec4, ivec4, mat4
#include <glm/gtc/matrix_transform.hpp>         // translate, rotate, scale, perspective
#include <glm/gtc/type_ptr.hpp>                 // value_ptr
//#include <SFML/OpenGL.hpp>
//#include <SFML/Window.hpp>
#include "Vertex_Shader.hpp"
#include <Windows.h>
#include <fstream>


#define PI 3.14159265

namespace example
{
	using namespace rapidxml;
	using namespace std;

	const string View::vertex_shader_code =
		"#version 330\n"
		""
		"struct Light"
		"{"
		"    vec4 position;"
		"    vec3 color;"
		"};"
		""
		"uniform Light light;"
		"uniform float ambient_intensity;"
		"uniform float diffuse_intensity;"
		""
		"uniform mat4 model_view_matrix;"
		"uniform mat4 projection_matrix;"
		"uniform mat4     normal_matrix;"
		""
		"uniform vec3 material_color;"
		""
		"layout (location = 0) in vec3 vertex_coordinates;"
		"layout (location = 1) in vec2 texCoordinates;"
		"layout (location = 2) in vec3 vertex_normal;"
		""
		"out vec3 front_color;"
		"out vec2 TexCoords;"
		"out float intensity;"
		""
		"void main()"
		"{"
		"    vec4  normal   = normal_matrix * vec4(vertex_normal, 1.0);"
		"    vec4  position = model_view_matrix * vec4(vertex_coordinates, 1.0);"
		""
		"    vec4  light_direction = light.position - position;"
		"    float light_intensity = diffuse_intensity * max (dot (normalize (normal.xyz), normalize (light_direction.xyz)), 0.0);"
		""
		"	 TexCoords = texCoordinates;"
		"    front_color = diffuse_intensity * light_intensity * light.color ;"
		"	 intensity = ambient_intensity;"
		"    gl_Position = projection_matrix * position;"
		"}";


	const string View::fragment_shader_code =
		"#version 330\n"
		""
		"uniform sampler2D sampler2d;"
		"in  vec3    front_color;"
		"in vec2 TexCoords;"
		"in float intensity;"
		"out vec4 fragment_color;"
		""
		"void main()"
		"{"
		"    fragment_color = vec4((intensity * texture(sampler2d, TexCoords.st).rgb) , 1) + vec4(front_color * texture(sampler2d, TexCoords.st).rgb,1);"
		"}";



	//\brief View construcotr de View
	//@param with ancho de la ventana
	//@param height alto de la ventada
	View::View(int width, int height)
		:
		angle(0)

	{


		//createMesh("mesh", glm::vec3(0, 0, -1), glm::vec3(.2f, .2f, .2f),false, "../../../../assets/chair.obj", "../../../../assets/wood-crate-1.tga");
		//loadXML("SceneData.xml");

		glEnable(GL_CULL_FACE);
		glEnable(GL_DEPTH_TEST);


		//se atachan los shaders
		shader_program.attach(Vertex_Shader(Shader::Source_Code::from_string(vertex_shader_code)));
		shader_program.attach(Fragment_Shader(Shader::Source_Code::from_string(fragment_shader_code)));
		shader_program.link();

		//se setean las matrices				
		model_view_matrix_id = shader_program.get_uniform_id("model_view_matrix");
		projection_matrix_id = shader_program.get_uniform_id("projection_matrix");
		normal_matrix_id = shader_program.get_uniform_id("normal_matrix");


		configure_light();

		resize(width, height);

	}
	//\brief update funcion que se ejecuta cada tick
	void View::update()
	{
		angle += 0.1f;

	}

	//\brief render renderiza todos los elementos de la escena
	void View::render()
	{


		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);


		shader_program.use();
		configure_light();

		glm::mat4 mPadre = glm::mat4();

		for (auto const& i : meshMap) {

			meshMap[i.first]->render(camera, model_view_matrix_id, normal_matrix_id, angle * 2);

		}



	}

	//\brief resize re-setean todos los parametros
	//@param with ancho de la ventana
	//@param height alto de la ventana
	void View::resize(int width, int height)
	{

		width_original = width;
		height_original = height;
		camera.set_ratio(float(width) / height);



		shader_program.use();
		glm::mat4 projection_matrix = camera.get_projection();
		glUniformMatrix4fv(projection_matrix_id, 1, GL_FALSE, glm::value_ptr(projection_matrix));
		glViewport(0, 0, width, height);


	}

	void View::configure_light()
	{
		GLint light_position = shader_program.get_uniform_id("light.position");
		GLint light_color = shader_program.get_uniform_id("light.color");
		GLint ambient_intensity = shader_program.get_uniform_id("ambient_intensity");
		GLint diffuse_intensity = shader_program.get_uniform_id("diffuse_intensity");

		glUniform4f(light_position, 10.f, 20.f, 10.f, 1.f);
		glUniform3f(light_color, 1.f, 1.f, 1.f);
		glUniform1f(ambient_intensity, 0.2f);
		glUniform1f(diffuse_intensity, 0.8f);
	}


	//\brief on_key se llama cuando se pulsa alguna tecla para movel la camara
	void View::on_key()
	{
		if (GetKeyState('W') & 0x8000)
		{

			glm::vec3 position(camera.get_target() - camera.get_location());
			camera.move(position);

		}
		if (GetKeyState('S') & 0x8000)
		{

			glm::vec3 position(-camera.get_target() + camera.get_location());
			camera.move(position);
		}
		if (GetKeyState('D') & 0x8000)
		{
			glm::mat4 camera_rotation;
			camera.rotate(glm::mat4(glm::rotate(camera_rotation, glm::degrees(0.15f), glm::vec3(0.f, 1.f, 0.f))));


		}
		if (GetKeyState('A') & 0x8000)
		{
			glm::mat4 camera_rotation;
			camera.rotate(glm::mat4(glm::rotate(camera_rotation, glm::degrees(-0.15f), glm::vec3(0.f, 1.f, 0.f))));
		}
		if (GetKeyState('U') & 0x8000) {

			meshMap["mesh"]->meshData->setScale(glm::vec3(1, 1, 1));
		}

	}

	void View::createMesh(string name, glm::vec3 position, glm::vec3 scale, bool rotar, string mesh_path, string texture_path) {
		meshPath = new char[mesh_path.length() + 1];
		meshPath = mesh_path.c_str();
		textMeshPath = new char[texture_path.length() + 1];
		textMeshPath = texture_path.c_str();
		shared_ptr<Mesh> mesh(new Mesh(position, scale, rotar, meshPath, textMeshPath));
		meshMap[name] = mesh;
		mesh->meshData->setMeshString(mesh_path);
		mesh->meshData->setTextString(texture_path);
	}


	bool View::loadXML(const string & sceneFilePath)
	{
		std::cout << "\nComienza la lectura del XML";
		using std::fstream;

		// abrimos el fichero xml
		fstream xmlFile(sceneFilePath, fstream::in);

		// si lo ha abierto correctamente
		if (xmlFile.good())
		{
			// vamos a llenar un vector con el contenido del xml
			std::vector<char> xmlContent;

			bool end = false;

			while (!end)
			{
				// extrae cada caracter del archivo hasta que llega al -1 que es el final
				int character = xmlFile.get();

				if (character != -1)
				{
					xmlContent.push_back((char)character);
				}
				else
				{
					end = true;
				}
			}

			// cerramos el archivo
			xmlFile.close();
			// añadimos un caracter nulo al final
			xmlContent.push_back(0);

			xml_document<> document;
			document.parse<0>(xmlContent.data());// parseamos la información
			xmlNode * root = document.first_node();


			if (root && string(root->name()) == "scene")
			{

				if (!parseScene(root))
				{
					assert(false);
					return false;
				}
			}

			return true;
		}

		assert(false);
		return false;
	}

	bool View::parseScene(xmlNode * sceneNode)
	{
		std::cout << "\nComienza el parseo del Scene";
		// recorremos todos los hijos del nodo Scene
		for
			(
				xmlNode * child = sceneNode->first_node();
				child;
				child = child->next_sibling()
				)
		{

			if (child->type() == node_element)
			{

				if (string(child->name()) == "entities")
				{

					if (!parseEntities(child))
					{
						assert(false);
						return false;
					}
				}

			}
		}


		return true;
	}

	bool View::parseEntities(xmlNode * entitiesNode)
	{
		std::cout << "\nComienza el parseo del Entidades";
		string name;

		// recorremos las entity dentro del <tag> "entities"
		for
			(
				xmlNode * entityNode = entitiesNode->first_node();
				entityNode;
				entityNode = entityNode->next_sibling()
				)
		{

			if (entityNode->type() == node_element)
			{

				if (string(entityNode->name()) != "entity")
				{
					assert(false);
					return false;
				}
			}


			// recorremos los atributos de cada entity
			for
				(
					xmlAttrib * attribute = entityNode->first_attribute();
					attribute;
					attribute = attribute->next_attribute()
					)
			{

				if (string(attribute->name()) == "name")
				{

					name = attribute->value();
					meshName = name;
				}
			}


			if (name.empty())
			{
				assert(false);
				return false;
			}



			// recorremos los components de la entity
			for
				(
					xmlNode * componentsNode = entityNode->first_node();
					componentsNode;
					componentsNode = componentsNode->next_sibling()
					)
			{

				if (componentsNode->type() == node_element)
				{

					if (string(componentsNode->name()) == "components")
					{

						if (!parseComponents(componentsNode))
						{
							assert(false);
							return false;
						}


					}
				}
			}
			createMesh(name, meshPosition, meshScale,rot, meshString, textString);

		}

		// si todo va bien
		return true;
	}

	bool View::parseComponents(xmlNode * componentsNode)
	{
		std::cout << "\nComienza el parseo del Componentes";
		string value;
		// recorremos todos los componentes 
		for
			(
				xmlNode * componentNode = componentsNode->first_node();
				componentNode;
				componentNode = componentNode->next_sibling()
				)
		{
			

			if (componentNode->type() == node_element)
			{
				value = componentNode->value();

				if (string(componentNode->name()) == "position")
				{
					// parseamos la posicion

					// se optiene la cantidad de comas que hay
					size_t comaTimes = std::count(value.begin(), value.end(), ',');
					// donde va a estar la siguiente coma
					size_t comaPos = 0;
					// substring para extraer los datos
					string subString;

					vector<float> nums;

					for (size_t i = 0; i < comaTimes + 1; ++i)
					{
						// buscamos la posicion de la coma
						comaPos = value.find(',');
						std::cout << comaPos;
						// extraemos el trozo de texto que necesitamos
						subString = value.substr(0, comaPos);
						// transformamos el string a float
						nums.push_back(std::stof(subString));
						// borramos el trozo de texto que ya no necesitamos
						value.erase(0, comaPos + 1);
					}
					meshPosition = glm::vec3(nums[0], nums[1], nums[2]);
					/*assert(false);
					return false;*/
				}
				 if (string(componentNode->name()) == "scale")
				{
					// parseamos la posicion

					// se optiene la cantidad de comas que hay
					size_t comaTimes = std::count(value.begin(), value.end(), ',');
					// donde va a estar la siguiente coma
					size_t comaPos = 0;
					// substring para extraer los datos
					string subString;

					vector<float> nums;

					for (size_t i = 0; i < comaTimes + 1; ++i)
					{
						// buscamos la posicion de la coma
						comaPos = value.find(',');
						std::cout << comaPos;
						// extraemos el trozo de texto que necesitamos
						subString = value.substr(0, comaPos);
						// transformamos el string a float
						nums.push_back(std::stof(subString));
						// borramos el trozo de texto que ya no necesitamos
						value.erase(0, comaPos + 1);
					}
					meshScale = glm::vec3(nums[0], nums[1], nums[2]);
					/*assert(false);
					return false;*/
				}
				 if (string(componentNode->name()) == "mesh") {
					 
					 meshString = value;
					

				}
				 if (string(componentNode->name()) == "texture") {
					 
					 textString = value;

				}


			}





			// recorremos los atributos de los componentes


			
		}
		return true;
	}

	void View::saveXML(const string & sceneFilePath) {

		ofstream xmlF;
		xmlF.open(sceneFilePath);

		if (xmlF.good()) {

			xmlF << "<?xml version=\"1.0\"?>"<<"\n";
			xmlF << "<scene>" << "\n";
			xmlF << "<entities>" << "\n\n";
			for (auto const& i : meshMap ) {
				xmlF << "<entity name =\"" << i.first << "\">"<<"\n";
				xmlF << "<components>" << "\n";
				xmlF << "<position>" << i.second->meshData->getPosition().x << "," << i.second->meshData->getPosition().y << "," << i.second->meshData->getPosition().z << "</position>"<<"\n";
				xmlF << "<scale>" << i.second->meshData->getScale().x << "," << i.second->meshData->getScale().y << "," << i.second->meshData->getScale().z << "</scale>" << "\n";
				xmlF << "<mesh>" << i.second->meshData->getMeshString() << "</mesh>" << "\n";
				xmlF << "<texture>" << i.second->meshData->getTextureString() << "</texture>" << "\n";
				xmlF << "</components>" << "\n";
				xmlF << "</entity>"<<"\n";
			}
			xmlF << "</entities>" << "\n";
			xmlF << "</scene>" << "\n";
			

		}
	}
}
