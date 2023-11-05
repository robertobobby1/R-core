#pragma once 

#include "Services/ImGui/RCGui.h"

namespace RC {
	class SkeletonGui : public RCGui {
	public:
		SkeletonGui() : RCGui() {}
		~SkeletonGui() = default;

		virtual bool IsUniqueService() const override { return true; }

		virtual void Begin() override;
		virtual void End() override;
	};
}