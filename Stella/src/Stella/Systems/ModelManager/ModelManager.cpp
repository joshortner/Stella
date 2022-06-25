#include "ModelManager.h"
#include "Stella/EngineState.h"
#include "Stella/Core/Assert/Assert.h"
#include "Stella/Core/Logger/Logger.h"

#include <glm/glm.hpp>
#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

#include <vector>

namespace Stella
{

ModelManager::~ModelManager()
{
    for (auto & [key, p_model] : m_Models) {
        Model::destroy(p_model);
    }
}

Model * ModelManager::create_model(std::string name, Model::PrimitiveType type)
{
    STELLA_ASSERT((m_Models.find(name) == m_Models.end()), "Model name already in use: %s", name.c_str());
    m_Models[name] = Model::create(type);
    return m_Models[name];
}

Model * ModelManager::create_model(std::string name, Model::BuiltinType type)
{
	STELLA_ASSERT((m_Models.find(name) == m_Models.end()), "Model name already in use: %s", name.c_str());
    
	Model * model = nullptr;

	switch (type)
	{
		case Model::BuiltinType::QUAD:
			m_Models[name] = Model::quad();
			break;

		case Model::BuiltinType::CUBE:
			m_Models[name] = Model::cube();
			break;
		
		default:
			STELLA_ASSERT((0), "Unknown builtin model! - %d", type);
	}

    return model;
}

Model * ModelManager::create_model(std::string name, std::string file_path, Model::PrimitiveType type)
{
    tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;

	Model * model = create_model(name, type);
	bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, nullptr, nullptr, file_path.c_str(), nullptr, true);
	STELLA_ASSERT((ret), "Unable to load model: %s at %s", name.c_str(), file_path.c_str());
	STELLA_ENGINE_LOG("Model Loaded: %s", file_path.c_str());

	// Load vertices //
	/*
	float max_vertex_value = 0.0f;
	for (int i = 0; i < attrib.vertices.size(); i += 3) {
		model->vertices.push_back(
			glm::vec3(
				attrib.vertices[i],
				attrib.vertices[i+1],
				attrib.vertices[i+2]
			)
		);

		if (max_vertex_value < attrib.vertices[i]) {
			max_vertex_value = attrib.vertices[i];
		}
		if (max_vertex_value < attrib.vertices[i+1]) {
			max_vertex_value = attrib.vertices[i+1];
		}
		if (max_vertex_value < attrib.vertices[i+2]) {
			max_vertex_value = attrib.vertices[i+2];
		}
	}

	//Normalize vertices
	for (int i = 0; i < model->vertices.size(); i++) {
		model->vertices[i] = model->vertices[i] / max_vertex_value;
	}

	// Load UVs //
	for (int i = 0; i < attrib.texcoords.size(); i += 2) {
		model->uvs.push_back(
			glm::vec2(
				attrib.texcoords[i],
				attrib.texcoords[i+1]
			)
		);
	}

	// Load normals //
	for (int i = 0; i < attrib.normals.size(); i += 3) {
		model->normals.push_back(
			glm::vec3(
				attrib.normals[i],
				attrib.normals[i+1],
				attrib.normals[i+2]
			)
		);
	}*/

	// Load Data //
	//TODO Figure out how to do indexed
	float max_vertex_value = 0.0f;
	for (int s = 0; s < shapes.size(); s++) {
		for (int i = 0; i < shapes[s].mesh.indices.size(); i++) {
			int v_idx = shapes[s].mesh.indices[i].vertex_index;
			int uv_idx = shapes[s].mesh.indices[i].texcoord_index;
			int n_idx = shapes[s].mesh.indices[i].normal_index;

			if (v_idx >= 0) {
				model->vertices.push_back(
					glm::vec3(
						attrib.vertices[(v_idx * 3)],
						attrib.vertices[(v_idx * 3) + 1],
						attrib.vertices[(v_idx * 3) + 2]
					)
				);

				if (max_vertex_value < attrib.vertices[(v_idx * 3)]) {
					max_vertex_value = attrib.vertices[(v_idx * 3)];
				}
				if (max_vertex_value < attrib.vertices[(v_idx * 3) + 1]) {
					max_vertex_value = attrib.vertices[(v_idx * 3) + 1];
				}
				if (max_vertex_value < attrib.vertices[(v_idx * 3) + 2]) {
					max_vertex_value = attrib.vertices[(v_idx * 3) + 2];
				}
			}

			if (uv_idx >= 0) {
				model->uvs.push_back(
					glm::vec2(
						attrib.texcoords[(uv_idx * 2)],
						attrib.texcoords[(uv_idx * 2) + 1]
					)
				);
			}

			if (n_idx >= 0) {
				model->normals.push_back(
					glm::vec3(
						attrib.normals[(n_idx * 3)],
						attrib.normals[(n_idx * 3) + 1],
						attrib.normals[(n_idx * 3) + 2]
					)
				);
			}

			//model->indices.push_back(shapes[s].mesh.indices[i].vertex_index);
		}
	}

	//Normalize vertices
	for (int i = 0; i < model->vertices.size(); i++) {
		model->vertices[i] = model->vertices[i] / max_vertex_value;
	}

	model->build();

	STELLA_ENGINE_LOG("Model Built Success - %s", name.c_str());
	STELLA_ENGINE_LOG(" Vertices: %d", model->vertices.size());
	STELLA_ENGINE_LOG(" UVs     : %d", model->uvs.size());
	STELLA_ENGINE_LOG(" Indices : %d", model->indices.size());
	STELLA_ENGINE_LOG(" Normals : %d", model->normals.size());

	return model;
}

Model * ModelManager::get_model(std::string name)
{
    STELLA_ASSERT((m_Models.find(name) != m_Models.end()), "Model doesn't exist: %s", name.c_str());
    return m_Models[name];
}

    
} // namespace Stella
