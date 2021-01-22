#pragma once

namespace AD {
	class PhysicsBody {
	private:
		
	public:
		PhysicsBody();
		~PhysicsBody();

		virtual void Update(float deltaTime);
	};
}