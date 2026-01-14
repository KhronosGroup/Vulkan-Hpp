#include <vulkan/vulkan.hpp>
#include <vulkan/vulkan_raii.hpp>

#include <GLFW/glfw3.h>
#include <glslang/Public/ShaderLang.h>
#include <glslang/SPIRV/GlslangToSpv.h>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <map>
#include <memory>
#include <string>
#include <thread>
#include <limits>
#include <vector>
#include <numeric>
#include <sstream>
#include <iostream>

#include "math.hpp"
#include "utils.hpp"
#include "shaders.hpp"
#include "geometries.hpp"