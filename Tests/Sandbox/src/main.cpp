
#include "Stella.h"
#include "Sandbox.h"

#include "Stella/Platform/Platform.h"

#define CL_HPP_TARGET_OPENCL_VERSION 120
#define CL_HPP_MINIMUM_OPENCL_VERSION 120
#include "CL/opencl.hpp"

#include "box2d/box2d.h"

#include "Stella/Platform/Platform.h" //tmp

//#include <GLFW/glfw3.h>
//#include <glad/glad.h>

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#include <iostream>
#include <fstream>
#include <vector>
#include <list>
#include <unordered_map>
#include <string>
#include <math.h>

#include <entt.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define Deg2Rad(degrees) ((float)(( (degrees) * M_PI ) / 180.0f))

using namespace Stella;
using namespace Sandbox;

class NativeScriptTest : NativeScriptBase
{

public:

	~NativeScriptTest() { }

    virtual void initialize() override
	{
		//printf("Initialize script!! - %d\n", get_entity_id());
	}

	virtual void on_update() override 
	{
		//Vector3.Lerp( m_Pos1, m_Pos2, ( Mathf.Sin( m_Speed * Time.time ) + 1.0f ) / 2.0f );
		const glm::vec3 start = glm::vec3(0.0f, 2.0f, 0.0f);
		const glm::vec3 end = glm::vec3(0.0f, -2.0f, 0.0f);

		float t = (glm::sin(m_speed * (float)glfwGetTime()) + 1.0f) / 2.0f;

		glm::vec2 lerp = Math2D::lerp(start, end, t);
		get_component<TransformComponent>().m_transform.m_translation = glm::vec3(lerp.x, lerp.y, 0.0f);
		//get_component<TransformComponent>().m_transform.m_rotation = ((float)glfwGetTime()) * glm::vec3(0.0f, 0.0f, 0.75f);
	}

    virtual void destroy() override
	{
		//printf("Destroy script!!\n");
	}

private:

	float m_speed = 1.25f;
};

// NOTE: Rotation is expected to be in radians

class OpenCLState
{
public:

	cl::Platform m_platform;
	cl::Device m_device;
	cl::Context m_context;
	cl::CommandQueue m_queue;

	OpenCLState()
	{
		// Pick Platform //
		std::vector<cl::Platform> platforms;
		cl::Platform::get(&platforms);
		STELLA_LOG(Logger::DEBUG, "(%d) Available OpenCL Platforms", platforms.size());

		pick_platform(m_platform, platforms);
		STELLA_LOG(Logger::DEBUG, "Using OpenCL platform: %s", m_platform.getInfo<CL_PLATFORM_NAME>().c_str());

		// Pick Device //
		std::vector<cl::Device> devices;
		m_platform.getDevices(CL_DEVICE_TYPE_ALL, &devices);
		STELLA_LOG(Logger::DEBUG, "(%d) Available OpenCL Devices", devices.size());

		pick_device(m_device, devices);
		
		STELLA_LOG(
			Logger::DEBUG,
			"Using Device: %s Max Compute Units: %d, Max Work Group: %d\n",
			m_device.getInfo<CL_DEVICE_NAME>().c_str(),
			m_device.getInfo<CL_DEVICE_MAX_COMPUTE_UNITS>(),
			m_device.getInfo<CL_DEVICE_MAX_WORK_GROUP_SIZE>()
		);

		// Create an OpenCL context and command queue on that device.
		m_context = cl::Context(m_device);
		m_queue = cl::CommandQueue(m_context, m_device);
	}

	~OpenCLState()
	{
		for (auto [key, kernel] : m_kernels) {
			delete kernel;
		}
	}

	cl::Kernel * get_kernel(std::string name)
	{
		return m_kernels[name];
	}

	cl::Kernel * create_kernel(std::string name, std::string source_file_path)
	{
		//make sure name doesn't exist already
		std::string source;
		std::ifstream file(source_file_path);
		if (!file){
			printf("No OpenCL file found - %s\n", source_file_path.c_str());
			return nullptr;
		}
		while (!file.eof()) {
			char line[256];
			file.getline(line, 255);
			source += line;
		}

		//printf("Source:\n%s\n", source.c_str());

		const char * kernel_source = source.c_str();

		// Create an OpenCL m_program by performing runtime source compilation for the chosen device
		cl::Program program = cl::Program(m_context, kernel_source);
		cl_int result = program.build({ m_device });
		if (result) printf("Error during compilation OpenCL code!!!\n (%d)\n", result);
		if (result == CL_BUILD_PROGRAM_FAILURE) print_error_log(program, m_device);

		// Create a kernel (entry point in the OpenCL source program)
		m_kernels[name] = new cl::Kernel(program, name.c_str());
		return m_kernels[name];
	}

private:

	void pick_platform(cl::Platform & platform, const std::vector<cl::Platform> & platforms)
	{
		platform = platforms[0];
	}

	void pick_device(cl::Device & device, const std::vector<cl::Device> & devices) 
	{
		device = devices[0];
	}

	void print_error_log(const cl::Program& program, const cl::Device& device) {
	
		// Get the error log and print to console
		std::string buildlog = program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(device);
		std::cerr << "Build log:" << std::endl << buildlog << std::endl;

		// Print the error log to a file
		FILE *log = fopen("errorlog.txt", "w");
		fprintf(log, "%s\n", buildlog.c_str());
		std::cout << "Error log saved in 'errorlog.txt'" << std::endl;

		exit(1);
	}

private: 

	std::unordered_map<std::string, cl::Kernel *> m_kernels;

};

const int image_width = 1280;
const int image_height = 720;

cl_float4* cpu_output;
cl::Buffer cl_output;

inline float clamp(float x){ return x < 0.0f ? 0.0f : x > 1.0f ? 1.0f : x; }
// convert RGB float in range [0,1] to int in range [0, 255]
inline int colorf_to_colori(float x){ return int(clamp(x) * 255 + .5); }


int main(int argc, char **argv)
{
	// Initialize //
	EngineState::initialize();
	Sandbox::initialize();

	OpenCLState cl_state;
	cl::Kernel * kernel = cl_state.create_kernel("render_kernel", "../opencl_kernel.cl");
	
	// allocate memory on CPU to hold image
	cpu_output = new cl_float3[image_width * image_height];

	// Create image buffer on the OpenCL device
	cl_output = cl::Buffer(cl_state.m_context, CL_MEM_WRITE_ONLY, image_width * image_height * sizeof(cl_float3));

	// specify OpenCL kernel arguments
	kernel->setArg(0, cl_output);
	kernel->setArg(1, image_width);
	kernel->setArg(2, image_height);

	// every pixel in the image has its own thread or "work item",
	// so the total amount of work items equals the number of pixels
	std::size_t global_work_size = image_width * image_height;
	std::size_t local_work_size = 64; 

	// launch the kernel
	cl_state.m_queue.enqueueNDRangeKernel(*kernel, NULL, global_work_size, local_work_size);
	cl_state.m_queue.finish();

	// read and copy OpenCL output to CPU
	cl_state.m_queue.enqueueReadBuffer(cl_output, CL_TRUE, 0, image_width * image_height * sizeof(cl_float3), cpu_output);

	uint8_t image_data[image_width * image_height * 3];
	int idx = 0;
	for (int i = 0; i < image_width * image_height; i++) {
		image_data[idx+0] = colorf_to_colori(cpu_output[i].s[0]);
		image_data[idx+1] = colorf_to_colori(cpu_output[i].s[1]);
		image_data[idx+2] = colorf_to_colori(cpu_output[i].s[2]);
		idx += 3;
	}
	delete cpu_output;
	/*
	Texture2DInfo info(
		TextureWrap::REPEAT, 
		TextureFilter::LINEAR_MIPMAP_LINEAR, 
		TextureFilter::LINEAR,
		TextureFormat::RGB,
		TextureFormat::RGB,
		TextureDataType::UNSIGNED_BYTE,
		true, 
		false,
		"container.jpg"
	);*/

	/*
	Texture2DInfo info(
		TextureWrap::REPEAT, 
		TextureFilter::LINEAR_MIPMAP_LINEAR, 
		TextureFilter::LINEAR, 
		TextureFormat::RGB,
		TextureFormat::RGB,
		TextureDataType::UNSIGNED_BYTE,
		true, 
		true,
		image_width, 
		image_height, 
		image_data
	);
	Texture2D * p_texture = EngineState::get().m_pTexture2DManager->create("container", info);*/


	// Game Objects //
	Entity left_platform = STELLA_ENGINE_GET_SCENE().create_entity("Left");
	left_platform.add_component<SpriteComponent>(SANDBOX_TEXTURE_NAME_TEST_PX_ART);
	left_platform.add_component<TransformComponent>();
	left_platform.get_component<TransformComponent>().m_transform.m_rotation = glm::vec3(0.0f, 0.0f, glm::radians(45.0f));
	left_platform.get_component<TransformComponent>().m_transform.m_scale = glm::vec3(18.0f, 1.0f, 0.0f);
	left_platform.get_component<TransformComponent>().m_transform.m_translation = glm::vec3(30.0f, -30.0f, 0.0f);
	left_platform.add_component<RigidBody2DComponent>(RigidBody2DComponent::Type::STATIC);

	Entity right_platform = STELLA_ENGINE_GET_SCENE().create_entity("Right");
	right_platform.add_component<SpriteComponent>(SANDBOX_TEXTURE_NAME_TEST_PX_ART);
	right_platform.add_component<TransformComponent>();
	right_platform.get_component<TransformComponent>().m_transform.m_rotation = glm::vec3(0.0f, 0.0f, glm::radians(-45.0f));
	right_platform.get_component<TransformComponent>().m_transform.m_scale = glm::vec3(18.0f, 1.0f, 0.0f);
	right_platform.get_component<TransformComponent>().m_transform.m_translation = glm::vec3(5.0f, -5.0f, 0.0f);
	right_platform.add_component<RigidBody2DComponent>(RigidBody2DComponent::Type::STATIC);

	for (int i = 0; i < 30; i++) {
		for (int j = 0; j < 30; j++) {
			Entity new_entity = STELLA_ENGINE_GET_SCENE().create_entity("Container" + std::to_string(i));
			new_entity.add_component<SpriteComponent>(SANDBOX_TEXTURE_NAME_RED_GEM);
			new_entity.add_component<TransformComponent>();
			new_entity.get_component<TransformComponent>().m_transform.m_translation = glm::vec3(i * 1.25, j * 1.25, 0.0f);
			//new_entity.get_component<TransformComponent>().m_transform.m_rotation = glm::vec3(0.0f, 0.0f, glm::radians(-48.0f));
			//new_entity.add_component<NativeScriptComponent>(NativeScriptComponent().bind<NativeScriptTest>(new_entity));
			new_entity.add_component<RigidBody2DComponent>(RigidBody2DComponent::Type::DYNAMIC);
		}
	}

	//Entity point_source = STELLA_ENGINE_GET_SCENE().create_entity("Point Source");
	//point_source.add_component<TransformComponent>();
	//point_source.get_component<TransformComponent>().m_transform.m_translation = glm::vec3(-5.0f, 5.0f, 0.0f);
	//point_source.add_component<NativeScriptComponent>(NativeScriptComponent().bind<PointSourceScript>(point_source));

	// Main Loop //
	while (EngineState::get().m_pWindow->is_open()) {
		EngineState::on_update();
		EngineState::on_render();
		EngineState::on_finish_frame();
	}

	// Cleanup //
	EngineState::destroy();
}
