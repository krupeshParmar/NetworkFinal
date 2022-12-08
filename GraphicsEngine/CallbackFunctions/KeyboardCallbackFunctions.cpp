//#include "../inc_opengl.h"
//#include "../global/GlobalThings.h"  // For the light manager, etc.
//#include <glm/glm.hpp>
//#include <glm/vec3.hpp>
//#include <GLFW/glfw3.h>
//#include "../GameObject/GameObject.h"
//#include "../SceneEditor.h"
//
//// Extern is so the compiler knows what TYPE this thing is
//// The LINKER needs the ACTUAL declaration 
//// These are defined in theMainFunction.cpp
//extern GameObject* EDITOR_CAMERA;// = glm::vec3(0.0, 0.0, -25.0f);
//extern glm::vec3 g_cameraTarget;// = glm::vec3(0.0f, 0.0f, 0.0f);
//
//
//enum eEditMode
//{
//    MOVING_CAMERA,
//    MOVING_LIGHT,
//    MOVING_SELECTED_OBJECT  // For later, maybe??
//};
//
//eEditMode theEditMode = MOVING_CAMERA;
//unsigned int selectedLightIndex = 0;
//
//void key_callback(GLFWwindow* window,
//    int key, int scancode,
//    int action, int mods)
//{
//    if (key == GLFW_KEY_F && action == GLFW_PRESS)
//    {
//        EDITOR_CAMERA
//    }
//    return;
//    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
//    {
//        glfwSetWindowShouldClose(window, GLFW_TRUE);
//    }
//
//    if (key == GLFW_KEY_C && action == GLFW_PRESS)
//    {
//        theEditMode = MOVING_CAMERA;
//    }
//    else if (key == GLFW_KEY_L && action == GLFW_PRESS)
//    {
//        theEditMode = MOVING_LIGHT;
//    }
//
//    switch (theEditMode)
//    {
//    case MOVING_CAMERA:
//    {
//        break;
//        // Move the camera
//        // AWSD   AD - left and right
//        //        WS - forward and back
//        const float CAMERA_MOVE_SPEED = 0.1f;
//        if (key == GLFW_KEY_LEFT)     // Left
//        {
//            SceneEditor::EDITOR_CAMERA->transform->position.x -= CAMERA_MOVE_SPEED;
//        }
//        if (key == GLFW_KEY_RIGHT)     // Right
//        {
//            SceneEditor::EDITOR_CAMERA->transform->position.x += CAMERA_MOVE_SPEED;
//        }
//        if (key == GLFW_KEY_UP)     // Forward
//        {
//            SceneEditor::EDITOR_CAMERA->transform->position.z += CAMERA_MOVE_SPEED;
//        }
//        if (key == GLFW_KEY_DOWN)     // Backwards
//        {
//            SceneEditor::EDITOR_CAMERA->transform->position.z -= CAMERA_MOVE_SPEED;
//        }
//        if (key == GLFW_KEY_PAGE_UP)     // Down
//        {
//            SceneEditor::EDITOR_CAMERA->transform->position.y -= CAMERA_MOVE_SPEED;
//        }
//        if (key == GLFW_KEY_PAGE_DOWN)     // Up
//        {
//            SceneEditor::EDITOR_CAMERA->transform->position.y += CAMERA_MOVE_SPEED;
//        }
//    }//case MOVING_CAMERA:
//    break;
//
//    case MOVING_LIGHT:
//    {
//        const float LIGHT_MOVE_SPEED = 0.1f;
//        if (key == GLFW_KEY_LEFT)     // Left
//        {
//            ::g_pTheLightManager->list_Lights[selectedLightIndex]->position.x -= LIGHT_MOVE_SPEED;
//        }
//        if (key == GLFW_KEY_RIGHT)     // Right
//        {
//            ::g_pTheLightManager->list_Lights[selectedLightIndex]->position.x += LIGHT_MOVE_SPEED;
//        }
//        if (key == GLFW_KEY_UP)     // Forward
//        {
//            ::g_pTheLightManager->list_Lights[selectedLightIndex]->position.z += LIGHT_MOVE_SPEED;
//        }
//        if (key == GLFW_KEY_DOWN)     // Backwards
//        {
//            ::g_pTheLightManager->list_Lights[selectedLightIndex]->position.z -= LIGHT_MOVE_SPEED;
//        }
//        if (key == GLFW_KEY_PAGE_UP)     // Down
//        {
//            ::g_pTheLightManager->list_Lights[selectedLightIndex]->position.y -= LIGHT_MOVE_SPEED;
//        }
//        if (key == GLFW_KEY_PAGE_DOWN)     // Up
//        {
//            ::g_pTheLightManager->list_Lights[selectedLightIndex]->position.y += LIGHT_MOVE_SPEED;
//        }
//
//        if (key == GLFW_KEY_END)
//        {
//            // Select previous light
//            if (selectedLightIndex > 0)
//            {
//                selectedLightIndex--;
//            }
//        }
//        if (key == GLFW_KEY_HOME)
//        {
//            if (selectedLightIndex < (::g_pTheLightManager->list_Lights.size() - 1))
//            {
//                // Select previous light
//                selectedLightIndex++;
//            }
//        }
//
//        // Change attenuation
//        if (key == GLFW_KEY_1)
//        {
//            // Linear Decrease by 1% 
//            ::g_pTheLightManager->list_Lights[selectedLightIndex]->atten.y *= 0.99f;
//        }
//        if (key == GLFW_KEY_2)
//        {
//            // Linear Increase by 1%
//            ::g_pTheLightManager->list_Lights[selectedLightIndex]->atten.y *= 1.01f;
//        }
//        if (key == GLFW_KEY_3)
//        {
//            // Quadratic Decrease by 0.01% 
//            ::g_pTheLightManager->list_Lights[selectedLightIndex]->atten.z *= 0.999f;
//        }
//        if (key == GLFW_KEY_4)
//        {
//            // Quadratic Increase by 0.01%
//            ::g_pTheLightManager->list_Lights[selectedLightIndex]->atten.z *= 1.001f;
//        }
//
//    }//case MOVING_LIGHT:
//    break;
//
//    }//switch (theEditMode)
//
//    return;
//}
