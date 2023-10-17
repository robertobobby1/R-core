#pragma once
#include "Core/Service.h"

namespace RC {

	class ImGui : public Service {
	public: 
		ImGui();
		~ImGui() = default;

		virtual void Init() override = 0;
		virtual void OnUpdate() override = 0;


	};
}