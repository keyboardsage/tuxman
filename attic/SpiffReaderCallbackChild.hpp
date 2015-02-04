#ifndef XSPFPLAYLIST_HPP_
#define XSPFPLAYLIST_HPP_

#include "Spiff.h"

namespace Chromium2
{
	/**
	 * This class helps implement the functionality of the
	 * Spiff library.
	 */
	class SpiffReaderCallbackChild: public Spiff::SpiffReaderCallback
	{
		public:
			SpiffReaderCallbackChild() { }
			~SpiffReaderCallbackChild() { }
			
		private:
			void addTrack(Spiff::SpiffTrack* track);
			void setProps(Spiff::SpiffProps* track);
	};
}

#endif /*XSPFPLAYLIST_HPP_*/
