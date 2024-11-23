#pragma once
#include "engine/engine_common.h"
#include "engine/engine_i_value.h"

#include <mutex>
#include <QString>

namespace engine {
	template<Container _Vec>
	class PrmFactory;

	template<typename _T>
	class Parameter :
		public IValue
	{
		friend class PrmFactory<std::vector<IValue*>>;

	public:
		virtual ~Parameter() noexcept override = default;
		Parameter(const Parameter&) = delete;

	private:
		explicit Parameter(const QString&, _T) noexcept;
		Parameter() { assert(false); }

	public:
		virtual QByteArray submit() const override;
		virtual void submit(void* _pDst) const override;
		virtual void accept(const void* _pSrc) override;

		virtual bool manualControl() const override;
		virtual void setManualControl(bool) override;

		virtual const QString& uniqueName() const override;

	public:
		const QString& description() const;
		const QString& quantityName() const;
		const QString& units() const;

	private:
		const QString uniqueName_{};
		QString description_{};

		QString quantityName_{ QStringLiteral("[quantity]")};
		QString units_{ QStringLiteral("[units]") };

		std::atomic<bool> isManual_{ true };
		std::atomic<_T> value_;
	};

	template<typename _T>
	inline Parameter<_T>::Parameter(const QString& _uniqueName, _T _initValue) noexcept :
		uniqueName_(_uniqueName),
		value_(_initValue)
	{}

	template<typename _T>
	QByteArray Parameter<_T>::submit() const {
		QByteArray result(sizeof(_T), 0x00);
		*reinterpret_cast<_T*>(result.data()) = value_.load();
		return result;
	}

	template<typename _T>
	void Parameter<_T>::submit(void* _pDst) const {
		*reinterpret_cast<_T*>(_pDst) = value_.load();
	}

	template<typename _T>
	void Parameter<_T>::accept(const void* _pSrc) {
		value_.store(*reinterpret_cast<const _T*>(_pSrc));
	}

	template<typename _T>
	const QString& Parameter<_T>::uniqueName() const {
		return uniqueName_;
	}

	template<typename _T>
	const QString& Parameter<_T>::description() const {
		return description_;
	}

	template<typename _T>
	bool Parameter<_T>::manualControl() const {
		return isManual_.load();
	}

	template<typename _T>
	void Parameter<_T>::setManualControl(bool _isManual) {
		isManual_.store(_isManual);
	}

	template<typename _T>
	inline const QString& Parameter<_T>::units() const {
		return units_;
	}

	template<typename _T>
	inline const QString& Parameter<_T>::quantityName() const {
		return quantityName_;
	}
}
