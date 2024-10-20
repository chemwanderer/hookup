#pragma once
#include "engine/engine_common.h"
#include "engine/engine_i_value.h"

#include <mutex>
#include <QString>

namespace engine {
	template<Container Vec>
	class PrmFactory;

	template<typename T>
	class Parameter :
		public IValue
	{
		friend class PrmFactory<std::vector<IValue*>>;

	public:
		virtual ~Parameter() noexcept override = default;
		Parameter(const Parameter&) = delete;

	private:
		explicit Parameter(const QString&, T) noexcept;
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

	private:
		const QString uniqueName_{};
		QString description_{};

		std::atomic<bool> isManual_{ true };

		mutable std::mutex mtx_;
		T value_;
	};

	template<typename T>
	inline Parameter<T>::Parameter(const QString& _uniqueName, T _initValue) noexcept :
		uniqueName_(_uniqueName),
		value_(_initValue)
	{}

	template<typename T>
	QByteArray Parameter<T>::submit() const {
		QByteArray result(sizeof(T), 0x00);
		mtx_.lock();
		*reinterpret_cast<T*>(result.data()) = value_;
		mtx_.unlock();
		return result;
	}

	template<typename T>
	void Parameter<T>::submit(void* _pDst) const {
		mtx_.lock();
		*reinterpret_cast<T*>(_pDst) = value_;
		mtx_.unlock();
	}

	template<typename T>
	void Parameter<T>::accept(const void* _pSrc) {
		mtx_.lock();
		value_ = *reinterpret_cast<const T*>(_pSrc);
		mtx_.unlock();
	}

	template<typename T>
	const QString& Parameter<T>::uniqueName() const {
		return uniqueName_;
	}

	template<typename T>
	const QString& Parameter<T>::description() const {
		return description_;
	}

	template<typename T>
	bool Parameter<T>::manualControl() const {
		return isManual_.load();
	}

	template<typename T>
	void Parameter<T>::setManualControl(bool _isManual) {
		isManual_.store(_isManual);
	}
}