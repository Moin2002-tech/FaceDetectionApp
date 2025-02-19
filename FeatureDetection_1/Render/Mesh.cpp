/*
* Mesh.cpp
* Created on: 1/9/2024
* Author : Moin Shaikh
*/

#include"Mesh.hpp"

#include<fstream>

using std::string;

namespace Render
{

	void Mesh::writeObj(Mesh mesh, string fileName)
	{
		std::ofstream objectFile(fileName);
		for (const auto& v : mesh.vertex) 
		{
			//objFile << "v " << v.position[0] << " " << v.position[1] << " " << v.position[2] << std::endl;
			objectFile << "v " << v.position[0] << " " << v.position[1] << " " << v.position[2] << " " << v.color[0] << " " << v.color[1] << " " << v.color[2] << " " << std::endl;
		}
		for (const auto& v : mesh.tvi) {
			objectFile << "f " << v[0] + 1 << " " << v[1] + 1 << " " << v[2] + 1 << std::endl;
		}
		objectFile.close();
		return;

		// obj starts counting triangles at 1
		
	}
}