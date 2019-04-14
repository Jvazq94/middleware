#ifndef  _MESH_DATA
#define _MESH_DATA

#include <nanogui\opengl.h>
//#include <GL/glew.h>
#include <memory>
#include <iostream>
#include <Point.hpp>
#include <map>
#include <Vector.hpp>
//#include <SFML/OpenGL.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>         // translate, rotate, scale, perspective
#include <glm/gtc/type_ptr.hpp> // value_ptr
#include <vector>
using namespace std;
//using namespace glm;



namespace example {

	using std::vector;
	using toolkit::Point4i;
	using toolkit::Point3f;
	using toolkit::Point2f;

	class MeshData {

	private:

		typedef Point3f               Vertex;

		typedef vector<Vertex>      Vertex_Buffer;
		typedef vector<Point2f>      Text_Buffer;
		typedef vector<int>      Index_Buffer;

		Vertex_Buffer     vertices;
		Vertex_Buffer      normals;
		Text_Buffer		 tex_coord;
		Index_Buffer       indices;

		std::string name;
		glm::vec3 position;
		glm::vec3 scale;
		const char * mesh_path;
		const char * texture_path;
		bool rotate = true;
		string meshString;
		string textureString;

	public:
		///\brief devuelve el nombre
		string  getName() { return name; }
		///\brief devuelve la posicion
		glm::vec3 getPosition() { return position; }
		///\brief devuelve la escala
		glm::vec3 getScale() { return scale; }
		///\brief devuelve el path del mesh
		const char * getMeshPath() { return mesh_path; }
		///\brief devuelve el path de textura
		const char * getTexturePath() { return texture_path; }
		///\brief devuelve los vertices
		Vertex_Buffer getVertices() { return vertices; }
		///\brief devuelve las normales
		Vertex_Buffer getNormals() { return normals; }
		///\brief devuelve las coordenadas de textura
		Text_Buffer getTextureCoords() { return tex_coord; }
		///\brief devuelve los indices
		Index_Buffer getIndices() { return indices; }
		///\brief devuelve la rotacion
		bool getRotate() { return rotate; }
		///\brief devuelve el path del mes en string
		string getMeshString() { return meshString; }
		///\brief devuelve el path de textura en string
		string getTextureString() { return textureString; }


		///\brief setea el nombre
		void setName(string newName) {  name = newName; }
		///\brief setea la posicion
		void setPosition(glm::vec3 newPosition) {  position = newPosition; }
		///\brief setea la escala
		void setScale(glm::vec3 newScale) {  scale = newScale; }
		///\brief setea el path del mesh
		void setMashPath(const char * meshPath) {  mesh_path = meshPath; }
		///\brief setea el path de textura
		void setTexturePath(const char * textPath) {  texture_path = textPath; }
		///\brief setea los vertices
		void setVertices(Vertex_Buffer newVertices) {  vertices = newVertices; }
		///\brief setea las normales
		void setNormals(Vertex_Buffer newNormals) {  normals = newNormals; }
		///\brief setea la coordenadas de textura
		void setTextureCoords(Text_Buffer textureCoords) {  tex_coord = textureCoords; }
		///\brief setea los incices
		void setIndices(Index_Buffer newIndices) {  indices = newIndices; }
		///\brief setea la rotacion
		void setRotate(bool rotar) { rotate = rotar; }
		///\brief setea el path del mes en string
		void setMeshString(string meshS) { meshString = meshS; }
		///\brief setea le path de textura en string
		void setTextString(string textS) { textureString = textS; }

		MeshData(){}







	};


}
#endif // ! _MESH_DATA

