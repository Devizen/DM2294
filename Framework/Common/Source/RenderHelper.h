#ifndef RENDER_HELPER_H
#define RENDER_HELPER_H

#include "Vertex.h"
#include "SingletonTemplate.h"

class Mesh;

class RenderHelper : public Singleton <RenderHelper>
{
	friend class SceneText;
public:
	static void RenderMesh(Mesh* _mesh);
	static void RenderMeshWithLight(Mesh* _mesh);
	static void RenderText(Mesh* _mesh, const std::string& _text, Color _color);

	void renderPlayerHealth(void);
	void renderKOCount(void);
};

#endif // RENDER_HELPER_H