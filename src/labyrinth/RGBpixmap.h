#ifndef RGBPIXMAP_H
#define RGBPIXMAP_H

class RGBmap{
public:
	unsigned char r,g,b;
};

class RGBpixmap{
	public:
		int nRows, nCols;
		RGBmap *pixel;
	int readBMPFile(char * fname);
	void setTexture(GLuint textureName);

};

#endif
