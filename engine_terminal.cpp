#include "engine_terminal.h"

#include "engine/routine/engine_routine_factory.h"
#include "engine/engine_parameter_factory.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>

namespace engine {
	Terminal* Terminal::pTerminal_ = nullptr;

	Terminal::~Terminal() noexcept {
		for (auto* pPrm : parameters_) {
			if (pPrm) { delete pPrm; }
		}

		for (auto* pRoutine : routines_) {
			if (pRoutine) { delete pRoutine; }
		}
	}

	IValue* Terminal::parameter(QStringView _uniqueName) {
		auto& rParams = terminal()->parameters_;
		auto paramIt = std::find_if(rParams.begin(), rParams.end(),
			[_uniqueName](IValue* _pValue)->bool {return _pValue->uniqueName() == _uniqueName; });

		if (rParams.end() != paramIt) { return *paramIt; }
		return nullptr;
	}

	IValue* Terminal::parameter(const QString& _uniqueName) {
		auto& rParams = terminal()->parameters_;
		auto paramIt = std::find_if(rParams.begin(), rParams.end(),
			[_uniqueName](IValue* _pValue)->bool {return _pValue->uniqueName() == _uniqueName; });

		if (rParams.end() != paramIt) { return *paramIt; }
		return nullptr;
	}

	bool Terminal::build() {
		auto* pTerminal = terminal();
		if (pTerminal->isBuilt_) { return false; }

		QFile assemblyFile;
		assemblyFile.setFileName(pact::__assemblyPath__.toString());
		if (!assemblyFile.open(QFile::ReadOnly)) { return false; }

		QByteArray assemblyBytes = assemblyFile.readAll();

		QJsonParseError error(QJsonParseError::NoError);
		QJsonDocument assemblyDoc = QJsonDocument::fromJson(assemblyBytes, &error);
		if (error.error != QJsonParseError::NoError) { return false; }

		// ***
		// parameters & routines building:
		QJsonObject assemblyObj = assemblyDoc.object();
		{
			QJsonArray parameters = assemblyObj.value(pact::__parametersKey__).toArray();
			PrmFactory<std::vector<IValue*>> prmFactory(&pTerminal->parameters_, &parameters);
		}
		{
			QJsonArray routines = assemblyObj.value(pact::__routinesKey__).toArray();
			routine::Factory<std::vector<routine::Base*>> routineFactory(&pTerminal->routines_, &routines);
		}
		// - parameters & routines building
		// ***

		// ***
		// designate default-launched routines:
		QJsonArray defaultLaunch = assemblyObj.value(pact::__defaultLaunchKey__).toArray();
		for (auto valueRef : defaultLaunch) {
			if (valueRef.isString()) {
				QString toggleID = valueRef.toString();
				pTerminal->defaultLaunchToggleIDs_.push_back(toggleID);
			}
		}
		// - designate default-launched routines
		// ***

		pTerminal->isBuilt_ = true;

		return true;
	}

	void Terminal::launch() {
		auto* pTerminal = terminal();
		for (auto* pRoutine : pTerminal->routines_) {
			pRoutine->start();
		}

		bool state = true;
		for (const auto& id : pTerminal->defaultLaunchToggleIDs_) {
			auto* pPrm = dynamic_cast<Parameter<bool>*>(parameter(id));
			if (pPrm) { pPrm->accept(&state); }
		}
	}

	void Terminal::halt() {
		auto* pTerminal = terminal();
		for (auto* pRoutine : pTerminal->routines_) {
			pRoutine->brake();
		}

		bool stopped = false;
		while (!stopped) {
			stopped = true;
			for (auto* pRoutine : pTerminal->routines_) {
				if (routine::Status::quitted != pRoutine->status()) {
					stopped = false;
					break;
				}
			}
		}

		delete pTerminal;
		pTerminal_ = nullptr;
	}
}