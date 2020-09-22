#pragma once

namespace AD {
	class Light {
	private:

	public:
		Light();
		~Light();

		static Light* DeepCopy(Light* light);
	};
}