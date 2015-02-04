/* Author: Alwyn Malachi Berkeley
 * Created: 9-11-06
 * Modified: N/A
 * Description: Defines a class for manipulating files. */

#ifndef FILE_MANIPULATION_HPP
#define FILE_MANIPULATION_HPP

#include <iostream>
#include <fstream>
#include <memory>
#include <vector>

// import the headers that hold directory functions
#ifdef WINDOWS
	#include "windows.h"
#elif LINUX
	#include "errno.h"
	#include "dirent.h"
#endif

namespace Pacman {
	typedef std::vector<std::string> DirectoryList;
	typedef std::vector<std::string> LineList;

	class FileManipulation {
		public:
			FileManipulation();
			explicit FileManipulation(const std::string &strFileNameParam);
			~FileManipulation();
			
			// methods
			const std::auto_ptr<DirectoryList> getDirectoryContents();
			const std::auto_ptr<LineList> readFile();
			void appendFile(); // add exception
			void writeFile(); // add exception
			void removeLine(); // add exception
			void clearFile();
			const bool isExist();

			// accessor methods
			void setFileName(const std::string &strFileNameParam);
			const std::string& getFileName() const;
			
			void setFilePath(const std::string &strFilePathParam);
			const std::string& getFilePath() const;
		
		protected:
			const bool openFile();
			void closeFile();
			
		private:
			std::fstream FileStream;
			std::string strFileName;
			std::string strFilePath;
	};
}

#endif
