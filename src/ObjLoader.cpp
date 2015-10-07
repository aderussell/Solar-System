#include "ObjLoader.h"
#include <sstream>
#include <fstream>
#include <iostream>

#include "String_Additions.h"
#include "ShaderManager.h"



ObjLoader::ObjLoader(LPSTR filename)
{
	meshGroup = LoadMesh(filename);
}


ObjLoader::ObjLoader(LPSTR filename, LPSTR mtlFilename)
{
	meshGroup = LoadMesh2(filename, mtlFilename);
}

ObjLoader::~ObjLoader()
{
	//if (meshGroup) delete meshGroup;
}

std::unordered_map<std::wstring, MeshMaterial *> ObjLoader::loadMeshMaterial(LPSTR filename)
{
	std::string filenameString(filename);
	std::string filePath = folderForPath(filenameString);
	std::wstring wFilePath(filePath.begin(), filePath.end());

	std::wifstream          fileStream;
	std::wstring            line;

	std::unordered_map<std::wstring, MeshMaterial *> materials;

	MeshMaterial *mesh = NULL;

	fileStream.open(filename);
	bool isOpen = fileStream.is_open();		//debugging only.


	while(std::getline(fileStream, line)) {

		// removing front whitespace
		line = TrimStart(line);

		WCHAR oldStyleStr[200];
		wcscpy(oldStyleStr, line.c_str());

		// if it's a comment (start with a #) the go to next line
		if (oldStyleStr[0] == '#') continue;

		if (line.compare(0, 6, L"newmtl") == 0) {
			int length = line.length();
			std::wstring name = line.substr(7, length - 7);

			mesh = new MeshMaterial();
			std::pair<std::wstring, MeshMaterial *> pair(name, mesh);
			materials.insert(pair);

			
			mesh->name = name;
		}


		if (line.compare(0,2,L"Ka") == 0) {
			WCHAR first[5];
			float r,g,b;
			swscanf(oldStyleStr, L"%2s%f%f%f", first, &r, &g, &b);
			XMFLOAT3 v(r,g,b);
			mesh->ka = v;
		}

		if (line.compare(0,2,L"Kd") == 0) {
			WCHAR first[5];
			float r,g,b;
			swscanf(oldStyleStr, L"%2s%f%f%f", first, &r, &g, &b);
			XMFLOAT3 v(r,g,b);
			mesh->kd = v;
		}

		if (line.compare(0,2,L"Ks") == 0) {
			WCHAR first[5];
			float r,g,b;
			swscanf(oldStyleStr, L"%2s%f%f%f", first, &r, &g, &b);
			XMFLOAT3 v(r,g,b);
			mesh->ks = v;
		}

		if (line.compare(0,5,L"Illum") == 0) {
			mesh->illum = (IllumType)(oldStyleStr[6] - 48);
		}

		if (line.compare(0, 6, L"map_Kd") == 0) {
			int length = line.length();
			std::wstring name = line.substr(7, length - 7);
			std::wstring texturePath = wFilePath + name;
			mesh->map_kd = texturePath;
			mesh->hasTexture = true;
		}


	}
	return materials;
}


Mesh *ObjLoader::LoadSingleMesh(LPSTR filename, bool loadMaterials)
{
	std::string filenameString(filename);
	std::string filePath = folderForPath(filenameString);

	Mesh *meshGroup = new Mesh();


	std::vector<XMFLOAT3> verticies;
	std::vector<XMFLOAT3> normals;
    std::vector<XMFLOAT2> TexUV;

	//std::vector<int> startOfGroup;
	//std::vector<int> startOfRange;
	//std::vector<std::wstring> materialAtRange;

	std::vector<int> vertexIndicies;
	std::vector<int> normalIndicies;
    std::vector<int> texUVIndicies;

	std::wifstream          fileStream;
	std::wstring            line;


	fileStream.open(filename);
	bool isOpen = fileStream.is_open();		//debugging only.

	int lineNumber       = 0;
	int faceIndex = 0;
	while(std::getline(fileStream, line))
	{
		// trim leading whitespace
		line = TrimStart(line);
		
		WCHAR oldStyleStr[200];
		wcscpy(oldStyleStr, line.c_str());

		// if it's a comment (start with a #) the go to next line
		if (oldStyleStr[0] == '#') continue;

		if (line.compare(0, 6, L"mtllib") == 0) {

			continue;
			//if (loadMaterials) {
			//	int length = line.length();
			//	std::wstring wname = line.substr(7, length - 7);
		
			//	std::string name(wname.begin(), wname.end());

			//	std::string materialPath = filePath + name;

			//	LPSTR materialPath2 = const_cast<char *>(materialPath.c_str());

			//	std::unordered_map<std::wstring, MeshMaterial *> newMaterials = loadMeshMaterial(materialPath2);

			//	meshGroup->addMaterials(newMaterials);
			//}
		}

		//******************************************************************//
		// If true, we have found a vertex.  Read it in as a 2 character	//
		// string, followed by 3 decimal numbers.	Suprisingly the C++		//
		// string has no split() method.   I am using really old stuff,		//
		// fscanf.  There  must be a better way, use regular expressions?	//
		//******************************************************************//
		if (line.compare(0, 2, L"v ") == 0) {
			WCHAR first[5];
			float x, y, z;

			swscanf(oldStyleStr, L"%2s%f%f%f", first, &x, &y, &z); 

			XMFLOAT3 v;
			v.x = x; v.y = y; v.z = z;
			verticies.push_back(v);
		}


		if (line.compare(0, 2, L"vn") == 0) {
			WCHAR first[5];
			float x, y, z;

			swscanf(oldStyleStr, L"%2s%f%f%f", first, &x, &y, &z); 

			XMFLOAT3 v;
			v.x = x; v.y = y; v.z = z;
			normals.push_back(v);
		}


		if (line.compare(0, 2, L"vt") == 0) {
			WCHAR first[4];
			float x, y;

			swscanf(oldStyleStr, L"%2s%f%f", first, &x, &y); 

			XMFLOAT2 v;
			v.x = x; v.y = y;
			TexUV.push_back(v);
		}

		//******************************************************************//
		// If true, we have found a face.   Read it in as a 2 character		//
		// string, followed by 3 decimal numbers.	Suprisingly the C++		//
		// string has no split() method.   I am using really old stuff,		//
		// fscanf.  There must be a better way, use regular expressions?	//
		//																	//
		// It assumes the line is in the format								//
		// f v1/vt1/vn1 v2/vt2/vn2 v3/vt3/vn3 ...							//
		//******************************************************************// 
		if (line.compare(0, 2, L"f ") == 0) {
			WCHAR first[5];
			WCHAR slash1[5];
			WCHAR slash2[5];
			WCHAR slash3[5];
			WCHAR slash4[5];
			WCHAR slash5[5];
			WCHAR slash6[5];

			UINT v1, vt1, vn1, v2, vt2, vn2, v3, vt3, vn3; 

			swscanf(oldStyleStr, L"%2s%d%1s%d%1s%d%d%1s%d%1s%d%d%1s%d%1s%d", first, 
										&v1, slash1, &vt1, slash2, &vn1, 
				                        &v2, slash3, &vt2, slash4, &vn2, 
										&v3, slash5, &vt3, slash6, &vn3); 

			vertexIndicies.push_back(v1 - 1);
			vertexIndicies.push_back(v2 - 1);
			vertexIndicies.push_back(v3 - 1);


			normalIndicies.push_back(vn1 - 1);
			normalIndicies.push_back(vn2 - 1);
			normalIndicies.push_back(vn3 - 1);

			texUVIndicies.push_back(vt1 - 1);
			texUVIndicies.push_back(vt2 - 1);
			texUVIndicies.push_back(vt3 - 1);

			faceIndex += 3;
		}

		if (line.compare(0, 6, L"usemtl") == 0) {
			continue;
			/*int length = line.length();
			std::wstring name = line.substr(7, length - 7);

			materialAtRange.push_back(name);
			startOfRange.push_back(faceIndex);*/
		}

		lineNumber++;
	}


	std::vector<SimpleVertex> vertices;
	std::vector<USHORT> indices;

	USHORT index = 0;

	///////////////////////// END SCANNING FILE
	for (unsigned int i = 0; i < vertexIndicies.size(); i++)
	{
		

		SimpleVertex sv;
		sv.Pos.x = verticies[vertexIndicies[i]].x;
		sv.Pos.y = verticies[vertexIndicies[i]].y;
		sv.Pos.z = verticies[vertexIndicies[i]].z;

		sv.VecNormal.x = normals[normalIndicies[i]].x;
		sv.VecNormal.y = normals[normalIndicies[i]].y;
		sv.VecNormal.z = normals[normalIndicies[i]].z;

		sv.TexUV.x = TexUV[texUVIndicies[i]].x;
		sv.TexUV.y = TexUV[texUVIndicies[i]].y;

		vertices.push_back(sv);

		indices.push_back(index);
		index++;
	}

	meshGroup->vertices = vertices;
	meshGroup->indices = indices;
	//meshGroup->startOfRange = startOfRange;
	//meshGroup->startOfGroup = startOfGroup;
	//meshGroup->materialAtRange = materialAtRange;


	return meshGroup;
}


MeshGroup *ObjLoader::LoadMesh(LPSTR filename, bool loadMaterials)
{
	std::string filenameString(filename);
	std::string filePath = folderForPath(filenameString);

	MeshGroup *meshGroup = new MeshGroup();


	std::vector<XMFLOAT3> verticies;
	std::vector<XMFLOAT3> normals;
    std::vector<XMFLOAT2> TexUV;

	std::vector<int> startOfGroup;
	std::vector<int> startOfRange;
	std::vector<std::wstring> materialAtRange;

	std::vector<int> vertexIndicies;
	std::vector<int> normalIndicies;
    std::vector<int> texUVIndicies;

	std::wifstream          fileStream;
	std::wstring            line;


	fileStream.open(filename);
	bool isOpen = fileStream.is_open();		//debugging only.

	int lineNumber       = 0;
	int faceIndex = 0;
	while(std::getline(fileStream, line))
	{
		// trim leading whitespace
		line = TrimStart(line);
		
		WCHAR oldStyleStr[200];
		wcscpy(oldStyleStr, line.c_str());

		// if it's a comment (start with a #) the go to next line
		if (oldStyleStr[0] == '#') continue;

		if (oldStyleStr[0] == 'g') {
			startOfGroup.push_back(faceIndex);
		}

		if (line.compare(0, 6, L"mtllib") == 0) {

			if (loadMaterials) {
				int length = line.length();
				std::wstring wname = line.substr(7, length - 7);
		
				std::string name(wname.begin(), wname.end());

				std::string materialPath = filePath + name;

				LPSTR materialPath2 = const_cast<char *>(materialPath.c_str());

				std::unordered_map<std::wstring, MeshMaterial *> newMaterials = loadMeshMaterial(materialPath2);

				meshGroup->addMaterials(newMaterials);
			}
		}

		//******************************************************************//
		// If true, we have found a vertex.  Read it in as a 2 character	//
		// string, followed by 3 decimal numbers.	Suprisingly the C++		//
		// string has no split() method.   I am using really old stuff,		//
		// fscanf.  There  must be a better way, use regular expressions?	//
		//******************************************************************//
		if (line.compare(0, 2, L"v ") == 0) {
			WCHAR first[5];
			float x, y, z;

			swscanf(oldStyleStr, L"%2s%f%f%f", first, &x, &y, &z); 

			XMFLOAT3 v;
			v.x = x; v.y = y; v.z = z;
			verticies.push_back(v);
		}


		if (line.compare(0, 2, L"vn") == 0) {
			WCHAR first[5];
			float x, y, z;

			swscanf(oldStyleStr, L"%2s%f%f%f", first, &x, &y, &z); 

			XMFLOAT3 v;
			v.x = x; v.y = y; v.z = z;
			normals.push_back(v);
		}


		if (line.compare(0, 2, L"vt") == 0) {
			WCHAR first[4];
			float x, y;

			swscanf(oldStyleStr, L"%2s%f%f", first, &x, &y); 

			XMFLOAT2 v;
			v.x = x; v.y = y;
			TexUV.push_back(v);
		}

		//******************************************************************//
		// If true, we have found a face.   Read it in as a 2 character		//
		// string, followed by 3 decimal numbers.	Suprisingly the C++		//
		// string has no split() method.   I am using really old stuff,		//
		// fscanf.  There must be a better way, use regular expressions?	//
		//																	//
		// It assumes the line is in the format								//
		// f v1/vt1/vn1 v2/vt2/vn2 v3/vt3/vn3 ...							//
		//******************************************************************// 
		if (line.compare(0, 2, L"f ") == 0) {
			WCHAR first[5];
			WCHAR slash1[5];
			WCHAR slash2[5];
			WCHAR slash3[5];
			WCHAR slash4[5];
			WCHAR slash5[5];
			WCHAR slash6[5];

			UINT v1, vt1, vn1, v2, vt2, vn2, v3, vt3, vn3; 

			swscanf(oldStyleStr, L"%2s%d%1s%d%1s%d%d%1s%d%1s%d%d%1s%d%1s%d", first, 
										&v1, slash1, &vt1, slash2, &vn1, 
				                        &v2, slash3, &vt2, slash4, &vn2, 
										&v3, slash5, &vt3, slash6, &vn3); 

			vertexIndicies.push_back(v1 - 1);
			vertexIndicies.push_back(v2 - 1);
			vertexIndicies.push_back(v3 - 1);


			normalIndicies.push_back(vn1 - 1);
			normalIndicies.push_back(vn2 - 1);
			normalIndicies.push_back(vn3 - 1);

			texUVIndicies.push_back(vt1 - 1);
			texUVIndicies.push_back(vt2 - 1);
			texUVIndicies.push_back(vt3 - 1);

			faceIndex += 3;
		}

		if (line.compare(0, 6, L"usemtl") == 0) {
			int length = line.length();
			std::wstring name = line.substr(7, length - 7);

			materialAtRange.push_back(name);
			startOfRange.push_back(faceIndex);
		}

		lineNumber++;
	}


	std::vector<SimpleVertex> vertices;
	std::vector<USHORT> indices;

	USHORT index = 0;

	///////////////////////// END SCANNING FILE
	for (unsigned int i = 0; i < vertexIndicies.size(); i++)
	{
		

		SimpleVertex sv;
		sv.Pos.x = verticies[vertexIndicies[i]].x;
		sv.Pos.y = verticies[vertexIndicies[i]].y;
		sv.Pos.z = verticies[vertexIndicies[i]].z;

		sv.VecNormal.x = normals[normalIndicies[i]].x;
		sv.VecNormal.y = normals[normalIndicies[i]].y;
		sv.VecNormal.z = normals[normalIndicies[i]].z;

		sv.TexUV.x = TexUV[texUVIndicies[i]].x;
		sv.TexUV.y = TexUV[texUVIndicies[i]].y;

		vertices.push_back(sv);

		indices.push_back(index);
		index++;
	}

	meshGroup->vertices = vertices;
	meshGroup->indices = indices;
	meshGroup->startOfRange = startOfRange;
	meshGroup->startOfGroup = startOfGroup;
	meshGroup->materialAtRange = materialAtRange;


	return meshGroup;
}







MeshGroup *ObjLoader::LoadMesh2(LPSTR filename, LPSTR mtlFilename)
{
	MeshGroup *meshGroup = new MeshGroup();


	std::vector<XMFLOAT3> verticies;
	std::vector<XMFLOAT3> normals;
    std::vector<XMFLOAT2> TexUV;

	std::vector<int> startOfGroup;
	std::vector<int> startOfRange;
	std::vector<std::wstring> materialAtRange;

	std::vector<int> vertexIndicies;
	std::vector<int> normalIndicies;
    std::vector<int> texUVIndicies;




	std::wifstream          fileStream;
	std::wstring            line;

	// load the materials from the material file
	if (mtlFilename != NULL) {
		meshGroup->materials = loadMeshMaterial(mtlFilename);
	}

	fileStream.open(filename);
	bool isOpen = fileStream.is_open();		//debugging only.

	int lineNumber       = 0;
	int faceIndex = 0;
	while(std::getline(fileStream, line))
	{
		// trim leading whitespace
		line = TrimStart(line);
		
		WCHAR oldStyleStr[200];
		wcscpy(oldStyleStr, line.c_str());

		// if it's a comment (start with a #) the go to next line
		if (oldStyleStr[0] == '#') continue;

		if (oldStyleStr[0] == 'g') {
			startOfGroup.push_back(faceIndex);
		}


		//******************************************************************//
		// If true, we have found a vertex.  Read it in as a 2 character	//
		// string, followed by 3 decimal numbers.	Suprisingly the C++		//
		// string has no split() method.   I am using really old stuff,		//
		// fscanf.  There  must be a better way, use regular expressions?	//
		//******************************************************************//
		if (line.compare(0, 2, L"v ") == 0) {
			WCHAR first[5];
			float x, y, z;

			swscanf(oldStyleStr, L"%2s%f%f%f", first, &x, &y, &z); 

			XMFLOAT3 v;
			v.x = x; v.y = y; v.z = z;
			verticies.push_back(v);
		}


		if (line.compare(0, 2, L"vn") == 0) {
			WCHAR first[5];
			float x, y, z;

			swscanf(oldStyleStr, L"%2s%f%f%f", first, &x, &y, &z); 

			XMFLOAT3 v;
			v.x = x; v.y = y; v.z = z;
			normals.push_back(v);
		}


		if (line.compare(0, 2, L"vt") == 0) {
			WCHAR first[4];
			float x, y;

			swscanf(oldStyleStr, L"%2s%f%f", first, &x, &y); 

			XMFLOAT2 v;
			v.x = x; v.y = y;
			TexUV.push_back(v);
		}

		//******************************************************************//
		// If true, we have found a face.   Read it in as a 2 character		//
		// string, followed by 3 decimal numbers.	Suprisingly the C++		//
		// string has no split() method.   I am using really old stuff,		//
		// fscanf.  There must be a better way, use regular expressions?	//
		//																	//
		// It assumes the line is in the format								//
		// f v1/vt1/vn1 v2/vt2/vn2 v3/vt3/vn3 ...							//
		//******************************************************************// 
		if (line.compare(0, 2, L"f ") == 0) {
			WCHAR first[5];
			WCHAR slash1[5];
			WCHAR slash2[5];
			WCHAR slash3[5];
			WCHAR slash4[5];
			WCHAR slash5[5];
			WCHAR slash6[5];

			UINT v1, vt1, vn1, v2, vt2, vn2, v3, vt3, vn3; 

			swscanf(oldStyleStr, L"%2s%d%1s%d%1s%d%d%1s%d%1s%d%d%1s%d%1s%d", first, 
										&v1, slash1, &vt1, slash2, &vn1, 
				                        &v2, slash3, &vt2, slash4, &vn2, 
										&v3, slash5, &vt3, slash6, &vn3); 

			vertexIndicies.push_back(v1 - 1);
			vertexIndicies.push_back(v2 - 1);
			vertexIndicies.push_back(v3 - 1);


			normalIndicies.push_back(vn1 - 1);
			normalIndicies.push_back(vn2 - 1);
			normalIndicies.push_back(vn3 - 1);

			texUVIndicies.push_back(vt1 - 1);
			texUVIndicies.push_back(vt2 - 1);
			texUVIndicies.push_back(vt3 - 1);

			faceIndex += 3;
		}

		if (line.compare(0, 6, L"usemtl") == 0) {
			int length = line.length();
			std::wstring name = line.substr(7, length - 7);

			materialAtRange.push_back(name);
			startOfRange.push_back(faceIndex);
		}

		lineNumber++;
	}


	std::vector<SimpleVertex> vertices;
	std::vector<USHORT> indices;

	USHORT index = 0;

	///////////////////////// END SCANNING FILE
	for (unsigned int i = 0; i < vertexIndicies.size(); i++)
	{
		

		SimpleVertex sv;
		sv.Pos.x = verticies[vertexIndicies[i]].x;
		sv.Pos.y = verticies[vertexIndicies[i]].y;
		sv.Pos.z = verticies[vertexIndicies[i]].z;

		sv.VecNormal.x = normals[normalIndicies[i]].x;
		sv.VecNormal.y = normals[normalIndicies[i]].y;
		sv.VecNormal.z = normals[normalIndicies[i]].z;

		sv.TexUV.x = TexUV[texUVIndicies[i]].x;
		sv.TexUV.y = TexUV[texUVIndicies[i]].y;

		vertices.push_back(sv);

		indices.push_back(index);
		index++;
	}

	meshGroup->vertices = vertices;
	meshGroup->indices = indices;
	meshGroup->startOfRange = startOfRange;
	meshGroup->startOfGroup = startOfGroup;
	meshGroup->materialAtRange = materialAtRange;


	return meshGroup;
}