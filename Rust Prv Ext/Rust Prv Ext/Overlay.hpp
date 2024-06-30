#include "Render/render.hpp"

class Overlay {
public:
	void DrawOverlay()
	{
		if (Renderer->Setup() != RENDER_INFORMATION::RENDER_SETUP_SUCCESSFUL)
		std::printf(" [render] -> failed to setup render.\n ");
	Renderer->Render();
	}
};