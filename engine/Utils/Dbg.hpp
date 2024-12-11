#pragma once

#include <imgui/imgui.h>

#define chk(name, initialValue) static bool name = (initialValue); ImGui::Checkbox(#name, &name); if (name)
#define chkbox(name) static bool name = false; ImGui::Checkbox(#name, &name);
#define infloat(name, initialValue) static float name = initialValue; ImGui::InputFloat(#name, &name)
#define insliderfloat(name, initialValue, min, max) static float name = initialValue; ImGui::SliderFloat(#name, &name, min, max)
#define inint(name, initialValue) static int name = initialValue; ImGui::InputInt(#name, &name)