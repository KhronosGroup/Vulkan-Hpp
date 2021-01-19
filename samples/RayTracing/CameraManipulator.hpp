// Copyright(c) 2019, NVIDIA CORPORATION. All rights reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//

#pragma once

#if defined( _MSC_VER )
#  pragma warning( push )
#  pragma warning( disable : 4127 )  // conditional expression is constant (glm)
#endif

#include <glm/glm.hpp>

#if defined( _MSC_VER )
#  pragma warning( pop )
#endif

#include <vulkan/vulkan.hpp>

namespace vk
{
  namespace su
  {
    class CameraManipulator
    {
      public:
        enum class Action { None, Orbit, Dolly, Pan, LookAround };
        enum class Mode { Examine, Fly, Walk, Trackball };
        enum class MouseButton { None, Left, Middle, Right };
        enum class ModifierFlagBits : uint32_t { Shift = 1, Ctrl = 2, Alt = 4 };
        using ModifierFlags = vk::Flags<ModifierFlagBits>;

      public:
        CameraManipulator();

        glm::vec3 const& getCameraPosition() const;
        glm::vec3 const& getCenterPosition() const;
        glm::mat4 const& getMatrix() const;
        Mode getMode() const;
        glm::ivec2 const& getMousePosition() const;
        float getRoll() const;
        float getSpeed() const;
        glm::vec3 const& getUpVector() const;
        glm::u32vec2 const& getWindowSize() const;
        Action mouseMove(glm::ivec2 const& position, MouseButton mouseButton, ModifierFlags & modifiers);
        void setLookat(const glm::vec3& cameraPosition, const glm::vec3& centerPosition, const glm::vec3& upVector);
        void setMode(Mode mode);
        void setMousePosition(glm::ivec2 const& position);
        void setRoll(float roll);   // roll in radians
        void setSpeed(float speed);
        void setWindowSize(glm::ivec2 const& size);
        void wheel(int value);

      private:
        void dolly(glm::vec2 const& delta);
        void motion(glm::ivec2 const& position, Action action = Action::None);
        void orbit(glm::vec2 const& delta, bool invert = false);
        void pan(glm::vec2 const& delta);
        double projectOntoTBSphere(const glm::vec2& p);
        void trackball(glm::ivec2 const& position);
        void update();

      private:
        glm::vec3 m_cameraPosition  = glm::vec3(10, 10, 10);
        glm::vec3 m_centerPosition  = glm::vec3(0, 0, 0);
        glm::vec3 m_upVector        = glm::vec3(0, 1, 0);
        float     m_roll            = 0;                      // Rotation around the Z axis in RAD
        glm::mat4 m_matrix          = glm::mat4(1);

        glm::u32vec2 m_windowSize   = glm::u32vec2(1, 1);

        float       m_speed         = 30.0f;
        glm::ivec2  m_mousePosition = glm::ivec2(0, 0);

        Mode m_mode                 = Mode::Examine;
    };
  } // namespace su
}  // namespace vk
