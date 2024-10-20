#pragma once
#include <QByteArray>
class QString;

namespace engine {
	class IValue
	{
	public:
		virtual ~IValue() noexcept = default;

	public:
		virtual QByteArray submit() const = 0;
		virtual void submit(void* _pDst) const = 0;
		virtual void accept(const void* _pSrc) = 0;

		virtual bool manualControl() const = 0;
		virtual void setManualControl(bool) = 0;

		virtual const QString& uniqueName() const = 0;
	};
}