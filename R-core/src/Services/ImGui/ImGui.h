#pragma once
#include "Core/Service.h"

namespace RC {

	class ImGui : public Service {
	public: 
		ImGui();
		~ImGui() = default;

		virtual void Init() override;
		virtual void OnUpdate() override;

	};
}