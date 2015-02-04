#include <string>
#include <cstdio>
#include <cstdlib>
#include <stdexcept>
#include <algorithm>
using namespace std;
#include "TGAFileReader.hpp"

namespace Engine
{
	/**
	 * Creates a TGAFileReader object.
	 */
	TGAFileReader::TGAFileReader()
	{
		// initialize this object's members
		tgaFile = NULL;
		header = NULL;
	}

	/**
	 * Creates a TGAFileReader object and loads a TARGA file.
	 */
	TGAFileReader::TGAFileReader(std::string tgaFilenameParam)
	{
		// load the TGA file
		read(tgaFilenameParam);
	}

	/**
	 * Cleans up and destroys a TGAFileReader object.
	 */
	TGAFileReader::~TGAFileReader()
	{
		if (tgaFile) free(tgaFile);
		if (imageData) delete [] imageData;
	}

	/**
	 * Loads a TARGA file using the filename that was set.
	 * \note if the header is invalid a runtime_error exception will be thrown.
	 * \note throws a logic error if no filename was set previous
	 * to calling this method.
	 */
	void TGAFileReader::read()
	{
		// throw a logic error if the filename was never set
		if (tgaFilename == "") throw(logic_error("Error: TGA file's filename is not set."));

		// call the other load method
		read(tgaFilename);
	}

	/**
	 * Loads a TARGA file.
	 * \note if the tga file could not open the std::exception object is thrown.
	 * \note if the header is invalid a runtime_error exception will be thrown.
	 */
	void TGAFileReader::read(std::string tgaFilenameParam)
	{
		// set the filename
		tgaFilename = tgaFilenameParam;

		// open the tga file
		// TODO: implement the more secure "fopen_s" on WIN32
		tgaFile = fopen(tgaFilename.c_str(), "rb");
		if (tgaFile == NULL) throw(exception("Error: TGA file could not be opened."));

		// read the TGA's header & file data
		readHeader();
		switch(tgaFileType)
		{
			case COMPRESSED_TRUE_COLOR:
				readCompressed();
				break;
			case UNCOMPRESSED_TRUE_COLOR:
				readUncompressed();
				break;
		}
	}

	/**
	 * Reads the header of a TGA file.
	 * \note if the header is invalid a runtime_error exception will be thrown.
	 * \return the type of TGA
	 */
	void TGAFileReader::readHeader()
	{
		// read the header
		header = new unsigned char[HEADER_SIZE];
		fread(header, 1, HEADER_SIZE, tgaFile);

		// return whether the header is part of a compressed or uncompressed file
		const unsigned char COMPRESSED_HEADER = 10;
		const unsigned char UNCOMPRESSED_HEADER = 2;
		switch(header[IMAGE_TYPE])
		{
			case COMPRESSED_TRUE_COLOR:
				tgaFileType = COMPRESSED_TRUE_COLOR;
				break;
			case UNCOMPRESSED_TRUE_COLOR:
				tgaFileType = UNCOMPRESSED_TRUE_COLOR;
				break;
			default:
				fclose(tgaFile);
				throw(runtime_error("Error: Invalid header in TGA file."));
				break;
		}

		// calculate and save the width, height, and bit-per-pixel data
		const int IMAGE_WIDTH_POSITION = IMAGE_SPECIFICATION + 4;
		const int IMAGE_HEIGHT_POSITION = IMAGE_WIDTH_POSITION + 2;
		const int IMAGE_BPP_POSITION = IMAGE_HEIGHT_POSITION + 2;
		int thisTargasWidth[2] = {header[IMAGE_WIDTH_POSITION], header[IMAGE_WIDTH_POSITION + 1]};
		int thisTargasHeight[2] = {header[IMAGE_HEIGHT_POSITION], header[IMAGE_HEIGHT_POSITION + 1]};
		width = thisTargasWidth[1] * 256 + thisTargasWidth[0];
		height = thisTargasHeight[1] * 256 + thisTargasHeight[0];
		bitsPerPixelDepth = header[IMAGE_BPP_POSITION]; // should be 24-bits or 32-bits

		// calculate the image's size.
		const int BYTE = 8;
		bytesInEachPixel = bitsPerPixelDepth / BYTE; // this is bytes not bits per pixel
		imageSize = width * height * bytesInEachPixel;

		// set the format
		switch (bytesInEachPixel)
		{
			case 3:
				format = GL_RGB;
				break;
			case 4:
				format = GL_RGBA;
				break;
		}
	}

	/**
	 * Reads the data held inside an uncompressed TGA file.
	 */
	// TODO: allow reading flipped or mirrored data.
	void TGAFileReader::readUncompressed()
	{
		// allocate memory to hold the tga file data
		imageData = new unsigned char[sizeof(unsigned char) * imageSize];

		// read the tga file data
		// TODO: check that image data is readable
		fread(imageData, sizeof(unsigned char), imageSize, tgaFile);

		// TARGA file format saves in BGR format so reverse the data
		// so it is RGB format
		for(long index = 0; index < imageSize; index += bytesInEachPixel)
		{
			swap(imageData[index], imageData[index + 2]);
		}

		// closes the file handle/descriptor
		fclose(tgaFile);
	}

	/**
	 * Reads the data held inside an compressed TGA file.
	 */
	void TGAFileReader::readCompressed()
	{
		// TODO: implement compressed targa image loading
	}
}