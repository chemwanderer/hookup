#pragma once
#include <QString>

namespace gui::pact {
	static constexpr QStringView __guiPath__ =		u"C:/hookup_data/config/gui.json";

	static constexpr QStringView __mainWindowTitle__ = u"Hookup";
	
	static constexpr QStringView __widgetsKey__ =	u"WIDGETS";
	static constexpr QStringView __blocksKey__ =	u"BLOCKS";

	namespace block {
		static constexpr QStringView __titleKey__ =			u"Title";
		static constexpr QStringView __widgets__ =			u"Widgets";
	}

	namespace item {
		static constexpr QStringView __typeKey__ =			u"Type";
		static constexpr QStringView __uniqueNameKey__ =	u"Unique Name";
		static constexpr QStringView __descriptionKey__ =	u"Description";

		namespace osc {
			static constexpr QStringView __typeSignature__ =	u"Oscillator";

			static constexpr QStringView __ampPrmID__ =			u"Amplitude ID";
			static constexpr QStringView __periodPrmID__ =		u"Period ID";
			static constexpr QStringView __valuePrmID__ =		u"Value ID";
		}

		namespace toggle {
			static constexpr QStringView __typeSignature__ =	u"Toggle";

			static constexpr QStringView __stateEstID__ =		u"Established State ID";
			static constexpr QStringView __stateReadID__ =		u"Read State ID";
			static constexpr QStringView __feedbackStatusID__ = u"Feedback Status ID";
		}

		namespace readonly {
			static constexpr QStringView __typeSignature__ =	u"Read Only Value";

			static constexpr QStringView __valueIDKey__ =		u"Value ID";
			static constexpr QStringView __precisionKey__ =		u"Precision";
			static constexpr QStringView __formatKey__ =		u"Format";

			static constexpr QStringView __floating__ =			u"Floating";
			static constexpr QStringView __exponential__ =		u"Exponential";
			static constexpr QStringView __feedbackStatusID__ = u"Feedback Status ID";
		}

		namespace controlled {
			static constexpr QStringView __typeSignature__ =	u"Controlled Value";

			static constexpr QStringView __valueReadIDKey__ =	u"Read Value ID";
			static constexpr QStringView __valueEstIDKey__ =	u"Established Value ID";
			static constexpr QStringView __feedbackStatusID__ = u"Feedback Status ID";
		}
	}
}