#ifndef TGA_FILE_ADT_HPP
#define TGA_FILE_ADT_HPP

#include <iostream>
using namespace std;
#include "OpenGL.hpp"

namespace Engine
{
	const int HEADER_SIZE = 18;
	const int NO_FORMAT = -1;

	/**
	 * Symbolizes the type of TGA file the TGA file is.  The value of the
	 * constants inside this enumeration correspond to the values for
	 * TGA image types in field 3 of the TGA 2.0 standard.
	 */
	enum TGA_FILE_TYPE {NO_TYPE = -1, UNCOMPRESSED_TRUE_COLOR = 2, COMPRESSED_TRUE_COLOR = 10};

	/**
	 * Symbolizes the different areas of the header.  The value of the
	 * constants inside this enumeration correspond to the values for
	 * TGA field offsets used in the TGA 2.0 standard.
	 */
	enum TGA_HEADER_AREAS
	{
		ID_LENGTH,
		COLOR_MAP_TYPE,
		IMAGE_TYPE,
		COLOR_MAP_SPECIFICATION,
		IMAGE_SPECIFICATION = 8
	};

	/**
	 * Loads TGA files into memory.
	 */
	class TGAFileADT
	{
		public:
			/**
			 * Creates a TGAFileADT object.
			 */
			TGAFileADT()
			{
				// initialize this object's members
				openglID = 0;
				width = 0;
				height = 0;
				imageData = NULL;
				tgaFilename = "";
				tgaFileType = NO_TYPE;
				bitsPerPixelDepth = 0;
				imageSize = 0;
				bytesInEachPixel = 0;
				format = NO_FORMAT;
			}

			/**
			 * Destroys this TGAFileADT object.
			 */
			~TGAFileADT()
			{
			}
			
			// methods

			// accessor methods

			/**
			 * Sets the filename that this object is manipulating.
			 * \param tgaFilenameParam filename that is to be manipulated.
			 */
			void setFilename(std::string tgaFilenameParam)
			{
				tgaFilename = tgaFilenameParam;
			}

			/**
			 * Gets the filename that this object is manipulating.
			 * \return filename that this object is manipulating.
			 */
			std::string getFilename() const
			{
				return tgaFilename;
			}

			/**
			 * Gets the OpenGL ID associated with this texture/image.
			 * \return The ID that OpenGL uses to reference this texture/image
			 * or 0 if it hasn't been set yet.
			 */
			GLuint* getOpenGLID()
			{
				return &openglID;
			}

			/**
			 * Gets the width of the image.
			 * \return width of the image or 0 if it hasn't been set yet.
			 */
			int getWidth() const
			{
				return width;
			}

			/**
			 * Gets the height of the image.
			 * \return height of the image or 0 if it hasn't been set yet.
			 */
			int getHeight() const
			{
				return height;
			}

			/**
			 * Gets the image's image data.
			 * \return the image's image data.  If there is no data then a NULL
			 * pointer will be returned.
			 */
			const unsigned char* getImageData() const
			{
				return imageData;
			}

			/**
			 * Gets the image's image type.
			 * \return the image's image type or NO-TYPE if a type has not been
			 * set yet.
			 */
			TGA_FILE_TYPE getFileType() const
			{
				return tgaFileType;
			}

			/**
			 * Gets the pixel depth used by this TGA image.
			 * \return the pixel depth as an integer.
			 */
			int getDepth() const
			{
				return bitsPerPixelDepth;
			}

			/**
			 * Gets the image size of the TGA image.
			 * \return the image size of the TGA image as a long integer.
			 */
			long getSize() const
			{
				return imageSize;
			}

			/**
			 * Gets the header of the TGA file.
			 * \return the header of the TGA file.
			 */
			const unsigned char* getHeader() const
			{
				return header;
			}

			/**
			 * Gets the format of the TGA file.
			 * \return the format of the TGA file.  Either GL_RGB, GL_RGBA,
			 * or NO_FORMAT if a format has not been set.
			 */
			const int getFormat() const
			{
				return format;
			}

		protected:
			GLuint openglID;
			int width;
			int height;
			unsigned char* imageData;
			std::string tgaFilename;
			TGA_FILE_TYPE tgaFileType;
			int bitsPerPixelDepth;
			int bytesInEachPixel; /**< the number of bytes that make up each pixel. */
			long imageSize;
			unsigned char* header;
			int format; /**< OpenGL format of the image, either GL_RGB or GL_RGBA */
	};
}

#endif