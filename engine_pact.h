#pragma once
#include <QString>

namespace engine::pact {
	static constexpr QStringView __assemblyPath__ =		u"C:/hookup_data/config/assembly.json";

	static constexpr QStringView __parametersKey__ =	u"Parameters";
	static constexpr QStringView __routinesKey__ =		u"Routines";
	static constexpr QStringView __defaultLaunchKey__ =	u"Default Launch Toggles";

	namespace parameter {
		static constexpr QStringView __typeKey__ =			u"Type";
		static constexpr QStringView __idKey__ =			u"ID";
		static constexpr QStringView __descriptionKey__ =	u"Description";
		static constexpr QStringView __isManual__ =			u"Manual Control";
		
		namespace type {
			static constexpr QStringView __toggleStatusType__ =		u"Toggle Status";
			static constexpr QStringView __routineStatusType__ =	u"Routine Status";
			static constexpr QStringView __booleanType__ =			u"Boolean";
			static constexpr QStringView __integerType__ =			u"Integer";
			static constexpr QStringView __realNumberType__ =		u"Real";
		}
	}

	namespace routine {
		static constexpr QStringView __typeKey__ =			u"Type";
		static constexpr QStringView __idKey__ =			u"ID";
		static constexpr QStringView __descriptionKey__ =	u"Description";

		static constexpr QStringView __toggleIDKey__ =		u"Toggle ID";
		static constexpr QStringView __statusIDKey__ =		u"Status ID";
		
		namespace mock {
			static constexpr QStringView __type__ =					u"Mock";
			static constexpr QStringView __devicesArrayKey__ =		u"Devices";

			namespace device {
				static constexpr QStringView __typeKey__ =				u"Type";

				namespace led {
					static constexpr QStringView __type__ =					u"LED";
					static constexpr QStringView __stateReadID__ =			u"Read State ID";
					static constexpr QStringView __stateEstID__ =			u"Established State ID";
				}

				namespace osc {
					static constexpr QStringView __type__ =					u"Oscillator";
					static constexpr QStringView __valueID__ =				u"Measured Value ID";
					static constexpr QStringView __amplitudeID__ =			u"Amplitude ID";
					static constexpr QStringView __periodID__ =				u"Period ID";
				}
			}
		}

		namespace mbed {
			static constexpr QStringView __type__ =					u"MBED";
			static constexpr QStringView __portNameKey__ =			u"Port Name";
			static constexpr QStringView __devicesArrayKey__ =		u"Devices";

			namespace device {
				static constexpr QStringView __typeKey__ =				u"Type";
				static constexpr QStringView __idKey__ =				u"ID";
				static constexpr QStringView __feedbackStatusID__ =		u"Feedback Status ID";

				namespace toggle {
					static constexpr QStringView __type__ =					u"Toggle";
					static constexpr QStringView __stateReadID__ =			u"Read State ID";
					static constexpr QStringView __stateEstID__ =			u"Established State ID";
				}

				namespace adc {
					static constexpr QStringView __type__ =					u"ADC";

					static constexpr QStringView __channel0PrmID__ =		u"Channel 0 Parameter ID";
					static constexpr QStringView __channel1PrmID__ =		u"Channel 1 Parameter ID";
					static constexpr QStringView __channel2PrmID__ =		u"Channel 2 Parameter ID";
					static constexpr QStringView __channel3PrmID__ =		u"Channel 3 Parameter ID";
					static constexpr QStringView __channel4PrmID__ =		u"Channel 4 Parameter ID";
					static constexpr QStringView __channel5PrmID__ =		u"Channel 5 Parameter ID";
				}

				namespace mcp4725 {
					static constexpr QStringView __type__ =					u"MCP4725";
					static constexpr QStringView __valueEstPrmID__ =		u"Established Value ID";
				}
			}
		}

		namespace udp {
			static constexpr QStringView __hostName__ =					u"Connected Host Name";
			static constexpr QStringView __sendingPort__ =				u"Sending Port";
			static constexpr QStringView __readingPort__ =				u"Reading Port";

			namespace client {
				static constexpr QStringView __type__ =					u"UDP Client";
				static constexpr QStringView __prmIDs__ =				u"Parameter IDs";
			}

			namespace server {
				static constexpr QStringView __type__ =					u"UDP Server";
			}
		}

		namespace inner {
			namespace osc {
				static constexpr QStringView __type__ =					u"Oscillation";

				static constexpr QStringView __controlledID__ =			u"Controlled Value ID";
				static constexpr QStringView __amplitudeID__ =			u"Amplitude ID";
				static constexpr QStringView __offsetID__ =				u"Offset ID";
				static constexpr QStringView __periodID__ =				u"Period ID";
			}
		}
	}
}