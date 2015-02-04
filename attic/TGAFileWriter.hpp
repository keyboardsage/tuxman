#ifndef TGA_FILE_WRITER_HPP
#define TGA_FILE_WRITER_HPP

#include "TGAFileADT.hpp"

namespace Engine
{
	/**
	 * Saves a TGA image to a file.
	 */
	class TGAFileWriter: public TGAFileADT
	{
		public:
			TGAFileWriter();
			explicit TGAFileWriter(std::string tgaFilename); // TODO: add a way to give data
			~TGAFileWriter();
			
			// methods
			
			// accessor methods
			
		private:
	};
}

#endif