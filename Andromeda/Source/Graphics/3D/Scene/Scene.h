#pragma once

#include "Node.h"

#include <string>
#include <memory>

namespace AD {
	class Scene {
	private:
		Node* m_RootNode;
		Node* m_ActiveCameraNode;

	public:
		Scene();
		~Scene();

		void Update(float deltaTime);
		void Draw();

		Node* GetRootNode();
		void AddNode(Node* object);
		Node* GetNodeByID(int id);

		void SetActiveCamera(Node* newCameraNode);

		int GetNumNodes();
	};
}