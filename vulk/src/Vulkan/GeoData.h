#pragma once
#include "Backend/vulkBuffer.h"

inline std::vector<Vertex> vertices = {
    { {  0.5f,  0.5f, 0.0f }, 0.0f, { 0.0f, 0.0f, 0.0f }, 0.0f, { 0.0f, 0.0f, 1.0f, 1.0f } }, // Mavi
    { { -0.5f,  0.5f, 0.0f }, 0.0f, { 0.0f, 0.0f, 0.0f }, 0.0f, { 0.0f, 1.0f, 0.0f, 1.0f } }, // Yeþil
    { { -0.5f, -0.5f, 0.0f }, 0.0f, { 0.0f, 0.0f, 0.0f }, 0.0f, { 1.0f, 0.0f, 0.0f, 1.0f } }, // Kýrmýzý
    { {  0.5f, -0.5f, 0.0f }, 0.0f, { 0.0f, 0.0f, 0.0f }, 0.0f, { 1.0f, 1.0f, 0.0f, 1.0f } }  // Sarý
};

inline std::vector<uint32_t> recindices = {
    0,1,2,2,3,0
};