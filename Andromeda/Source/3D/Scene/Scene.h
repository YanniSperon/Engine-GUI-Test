#pragma once

#include "Node.h"

#include <string>

namespace AD {
	class Scene {
	private:
		Node* rootNode;
		Node* activeCameraNode;

	public:
		Scene();
		~Scene();

		void Update(float deltaTime);
		void Begin();
		void Draw();
		void End();

		Node* GetRootNode();
		Node* CreateChildNode();
		void AddNode(Node* object);
		Node* GetNodeByName(const std::string& nodeName);

		void SetRootNode(Node* newNode);
		void SetActiveCamera(Node* newCamera);
	};
}