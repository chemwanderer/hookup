#pragma once

namespace gui {
	class IReadout
	{
	public:
		virtual ~IReadout() noexcept = default;

	public:
		virtual void updateReadouts() = 0;
	};
}