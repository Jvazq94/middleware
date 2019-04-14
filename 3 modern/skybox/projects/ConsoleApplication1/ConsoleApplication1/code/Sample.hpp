#include <nanogui\nanogui.h>
#include <nanogui\opengl.h>
#include <rapidxml.hpp>
#include "..\3 modern\skybox\code\ScreenDemo.hpp"


using namespace nanogui;
using namespace rapidxml;
using namespace std;
using namespace example;
namespace sample {

	bool bvar = false;
	float posX = 0;
	float posY = 0;
	float posZ = -2.0;
	float scaleMesh = 1.0f;

	std::string meshPath = "../../../../assets/chair.obj";
	std::string texturePath = "../../../../assets/wood-crate-1.tga";
	std::string xmlPath = "SceneData.xml";
	std::string name = "Mesh";
	class LoadXML {
	private:
		typedef rapidxml::xml_node<> xmlNode;
		typedef rapidxml::xml_attribute<> xmlAttrib;

		FormHelper * gui = nullptr;
		ScreenDemo * screen = nullptr;



	

	public:

		LoadXML(FormHelper * guiPuntero, ScreenDemo * screenPuntero) { gui = guiPuntero, screen = screenPuntero; }

		bool readXML(const string & sceneFilePath)
		{

			//gui = guiPuntero;
			//screen = screenPuntero;
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
					if (string(componentNode->name()) == "pathMesh")
					{
						gui->addVariable(componentNode->name(), meshPath);
					}
					if (string(componentNode->name()) == "texturePath")
					{
						gui->addVariable(componentNode->name(), texturePath);
					}

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