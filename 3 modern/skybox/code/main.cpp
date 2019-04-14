/*
src/example2.cpp -- C++ version of an example application that shows
how to use the form helper class. For a Python implementation, see
'../python/example2.py'.

NanoGUI was developed by Wenzel Jakob <wenzel.jakob@epfl.ch>.
The widget drawing code is based on the NanoVG demo application
by Mikko Mononen.

All rights reserved. Use of this source code is governed by a
BSD-style license that can be found in the LICENSE.txt file.
*/

#include <nanogui/nanogui.h>
#include <iostream>
#include <glm/glm.hpp>                          // vec3, vec4, ivec4, mat4
#include <glm/gtc/matrix_transform.hpp>         // translate, rotate, scale, perspective
#include <glm/gtc/type_ptr.hpp>                 // value_ptr
#include "View.hpp"
#include <memory>
#include "View.hpp"
#include <rapidxml.hpp>
#include <fstream>
#include <nanogui\nanogui.h>
#include "ScreenDemo.hpp"
#include "Sample.hpp"

using namespace example;


bool bvar = false;
float posX = 0;
float posY = 0;
float posZ = -2.0;
float scaleMesh = 1.0f;

std::string meshPath = "../../../../assets/chair.obj";
std::string texturePath = "../../../../assets/wood-crate-1.tga";
std::string xmlPath = "SceneData.xml";
std::string name = "Mesh";


using namespace nanogui;
using namespace rapidxml;
using namespace std;
namespace example {
	class LoadXML {
	private:
		typedef rapidxml::xml_node<> xmlNode;
		typedef rapidxml::xml_attribute<> xmlAttrib;

		FormHelper * gui;
		ScreenDemo * screen;
		
	public:
	
		LoadXML(){}

		bool readXML(const string & sceneFilePath, FormHelper * guiPuntero, ScreenDemo * screenPuntero)
		{
			gui = guiPuntero;
			screen = screenPuntero;
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

		bool parseScene(xmlNode * sceneNode)
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

						if (!parseGrup(child))
						{
							assert(false);
							return false;
						}
					}

				}
			}


			return true;
		}

		bool parseGrup(xmlNode * entitiesNode)
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

					if (string(entityNode->name()) != "grup")
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
						gui->addGroup(name);
						//meshName = name;

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
				//createMesh(name, meshPosition, meshScale, rot, meshString, textString);

			}

			// si todo va bien
			return true;
		}

		bool parseComponents(xmlNode * componentsNode)
		{
			std::cout << "\nComienza el parseo del Componentes";
			string value;
			ScreenDemo * thisScreen = screen;
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

					if (string(componentNode->name()) == "name")
					{
						gui->addVariable(componentNode->name(), name);
					}

					if (string(componentNode->name()) == "positionX")
					{
						gui->addVariable(componentNode->name(), posX)->setSpinnable(true);
					}
					if (string(componentNode->name()) == "positionY")
					{
						gui->addVariable(componentNode->name(), posY)->setSpinnable(true);
					}
					if (string(componentNode->name()) == "positionZ")
					{
						gui->addVariable(componentNode->name(), posZ)->setSpinnable(true);
					}
					if (string(componentNode->name()) == "scale")
					{
						gui->addVariable(componentNode->name(), scaleMesh)->setSpinnable(true);
					}
					if (string(componentNode->name()) == "rotate")
					{
						gui->addVariable(componentNode->name(), bvar);
					}
					if (string(componentNode->name()) == "createMesh")
					{
						gui->addButton("Create Mesh", [thisScreen]() {
							glm::vec3 position(posX, posY, posZ);
							glm::vec3 scaleVec(scaleMesh, scaleMesh, scaleMesh);
							thisScreen->getView()->createMesh(name, position, scaleVec, bvar, meshPath, texturePath);
							std::cout << "Create Texture" << std::endl;


						});
					}
					if (string(componentNode->name()) == "xmlPath")
					{
						gui->addVariable(componentNode->name(), xmlPath);
					}
					if (string(componentNode->name()) == "loadXML")
					{
						gui->addButton("Load XML", [thisScreen]() {

							thisScreen->getView()->loadXML(xmlPath);
							std::cout << "LoadXML" << std::endl;



						});
					}
					if (string(componentNode->name()) == "saveScene")
					{
						gui->addButton("Save Scene", [thisScreen]() {

							thisScreen->getView()->saveXML(xmlPath);
							std::cout << "Save Scene" << std::endl;


						});
					}

					if (string(componentNode->name()) == "deleteScene")
					{
						gui->addButton("Delete Content", [thisScreen]() {

							thisScreen->getView()->deleteAll();



						});
					}
					


				}





				// recorremos los atributos de los componentes



			}
			return true;
		}

	};
}

int main(int /* argc */, char ** /* argv */) {
	nanogui::init();

	/* scoped variables */ {
		
		ScreenDemo *screen = nullptr;

		
	
			// NanoGUI presents many options for you to utilize at your discretion.
			// See include/nanogui/screen.h for what all of these represent.
			screen = new ScreenDemo("NanoGUI test [GL 4.1]", 1200, 900);
		



		bool enabled = true;
		FormHelper *gui = new FormHelper(screen);
		nanogui::ref<nanogui::Window> window = gui->addWindow(Eigen::Vector2i(10, 10), "Menu");
		sample::LoadXML loadXML(gui,screen);
		
		if (!loadXML.readXML("FronXML.xml"))
		{
			assert(false);
			return false;
		}
		
		screen->setVisible(true);
		screen->performLayout();
		window->center();

		screen->drawAll();

		nanogui::mainloop();
	}

	nanogui::shutdown();
	return 0;
}

//
//#include "View.hpp"
//#include <GL/glew.h>            // Debe incluirse antes que gl.h
//#include <SFML/Window.hpp>
//#include <SFML/OpenGL.hpp>
//
//using namespace sf;
//using namespace example;
//
//int main ()
//{
//    Window window(VideoMode(800, 600), "OpenGL Examples: Skybox", Style::Default, ContextSettings(32));
//
//    // Una vez se ha creado el contexto de OpenGL ya se puede inicializar Glew:
//
//    GLenum glew_initialization =  glewInit ();
//
//    assert(glew_initialization == GLEW_OK);
//
//    // Glew se inicializa antes de crear view porque view ya usa extensiones de OpenGL:
//
//    View view (800, 600);
//
//    window.setVerticalSyncEnabled (true);
//
//    bool running = true;
//
//    do
//    {
//        Event event;
//
//        while (window.pollEvent (event))
//        {
//            switch (event.type)
//            {
//                case Event::Closed:
//                {
//                    running = false;
//                    break;
//                }
//
//                case Event::Resized:
//                {
//                    Vector2u window_size = window.getSize ();
//
//                    view.resize (window_size.x, window_size.y);
//
//                    break;
//                }
//
//                case Event::KeyPressed:
//                {
//                    view.on_key ();
//                    break;
//                }
//
//            }
//        }
//
//        view.update ();
//        view.render ();
//
//        window.display ();
//    }
//    while (running);
//
//    return (EXIT_SUCCESS);
//}



//else
//{


//	gui->addGroup("Mesh Paths");
//	gui->addVariable("Mesh Path", meshPath);
//	gui->addVariable("Texture Path", texturePath);

//	gui->addGroup("Mesh information");
//	gui->addVariable("Name", name);
//	gui->addVariable("PositionX", posX)->setSpinnable(true);
//	gui->addVariable("PositionY", posY)->setSpinnable(true);
//	gui->addVariable("PositionZ", posZ)->setSpinnable(true);
//	gui->addVariable("Scale ", scaleMesh);
//	gui->addVariable("Rotate", bvar);


//	//gui->addGroup("Other widgets");
//	gui->addButton("Create Mesh", [screen]() {
//		glm::vec3 position(posX, posY, posZ);
//		glm::vec3 scaleVec(scaleMesh, scaleMesh, scaleMesh);
//		screen->getView()->createMesh(name, position, scaleVec, bvar, meshPath, texturePath);
//		std::cout << "Create Texture" << std::endl;


//	});

//	gui->addGroup("Load Scene from XML");
//	gui->addVariable("XML Path", xmlPath);

//	gui->addButton("Load XML", [screen]() {

//		screen->getView()->loadXML(xmlPath);
//		std::cout << "LoadXML" << std::endl;



//	});

//	gui->addButton("Save Scene", [screen]() {

//		screen->getView()->saveXML(xmlPath);
//		std::cout << "Save Scene" << std::endl;


//	});

//	gui->addButton("Delete Content", [screen]() {

//		screen->getView()->deleteAll();



//	});
//}