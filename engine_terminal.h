#pragma once
//////////////////////////////////////////////////////////
//														//
// engine::Terminal is a central singlton for the		//
// backend management. It creates the instances of		//
// routines and shared parameters and owns the			//
// corresponding pointers.								//
//														//
//////////////////////////////////////////////////////////
#include <vector>
#include <mutex>
#include <QString>

namespace engine {
	class IValue;

	namespace routine {
		class Base;
	}

	class Terminal
	{
	public:
		// create an instance of terminal, configurate
		// parameters and routines from .json file
		static bool build();

		// start the routines
		static void launch();

		// set quit flags for all routines and wait
		// for all routines to stop
		static void halt();

		// search the shared parameter by its string id
		static IValue* parameter(QStringView _uniqueName);

		// search the shared parameter by its string id
		static IValue* parameter(const QString& _uniqueName);

	public:
		~Terminal() noexcept;

	private:
		Terminal() noexcept = default;

	private:
		static Terminal* pTerminal_;

	private:
		static Terminal* terminal();

	private:
		std::vector<routine::Base*> routines_{};
		std::vector<IValue*> parameters_{};
		std::vector<QString> defaultLaunchToggleIDs_{};

		bool isBuilt_{ false };
	};

	inline Terminal* Terminal::terminal() {
		if (!pTerminal_) { pTerminal_ = new Terminal; }
		return pTerminal_;
	}
}