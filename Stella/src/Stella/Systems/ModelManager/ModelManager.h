#pragma once

#include "Stella/Renderer/MeshRenderer/Model.h"

#include <unordered_map>
#include <string>

namespace Stella
{
    
class ModelManager
{

public:

	~ModelManager();

	Model * create_model(std::string name, Model::PrimitiveType type);
	Model * create_model(std::string name, Model::BuiltinType type);
    Model * create_model(std::string name, std::string file_path, Model::PrimitiveType type);
    Model * get_model(std::string name);

private:

	std::unordered_map<std::string, Model *> m_Models;
};

} // namespace Stella
