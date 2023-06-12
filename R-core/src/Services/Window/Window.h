#pragma once

#include "Core/Service.h"

namespace RC {

	struct WindowInput {
		uint32_t  Height;
		uint32_t  Width;

		WindowInput(uint32_t width = 1600, uint32_t height = 900)
			: Width(width), Height(height)
		{
		}

	};

	class Window : public Service {
	public:
		Window(const WindowInput& input);
		~Window() = default;

		virtual void Init() override = 0;
		virtual void OnUpdate() override = 0;

		static std::shared_ptr<Window> Create(const WindowInput& input);

	protected:
		uint32_t  m_width;
		uint32_t  m_height;

	};
}