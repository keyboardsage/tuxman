#ifndef TGA_FILE_READER_HPP
#define TGA_FILE_READER_HPP

#include <string>
#include <cstdio>
#include "TGAFileADT.hpp"

namespace Engine
{
	/**
	 * Loads TGA files into memory.
	 */
	class TGAFileReader: public TGAFileADT
	{
		public:
			TGAFileReader();
			explicit TGAFileReader(std::string tgaFilenameParam);
			virtual ~TGAFileReader();
			
			// methods
			void read();
			void read(std::string tgaFilename);

			// accessor methods
		
		protected:
			void readHeader();
			void readUncompressed();
			void readCompressed();

		private:
			FILE* tgaFile;
	};
}

#endif