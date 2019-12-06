#include "ImageMASS.h"

bool ImageMASS::set(int x, int y, Color c){
	if (x >= width * sampleLevel || y >= height * sampleLevel || x < 0 || y < 0) {
		std::cerr << "ImageMass set: x , y " << x << "," << y << "out of range" << std::endl;
		return false;
	}
	data[y * width + x] = c;

	return true;
}

ImageMASS::ImageMASS(size_t Width, size_t Height, unsigned char sampleLevel) :sampleLevel(sampleLevel) {
    width = Width * sampleLevel;
	height = Height * sampleLevel;
	data = new Color[width * height];
}

Color ImageMASS::get(int x, int y){
	if (x >= width * sampleLevel|| y >= height * sampleLevel|| x < 0 || y < 0) {
		std::cerr << "ImageMass get: x , y " << x << "," << y << "out of range" << std::endl;
	}
	return data[width * y + x];
}

int ImageMASS::get_width() {
	return width;
}
int ImageMASS::get_height(){
	return height;
}

void ImageMASS::sample(Image* image) {
	int w = width / sampleLevel, h = height / sampleLevel;
	for (int x = 0; x != w; x++)
		for (int y = 0; y != h; y++) {
			Vec4f temp(0,0,0,0);
			for (int i = 0; i != sampleLevel; i++) {
				for (int j = 0; j != sampleLevel; j++) {
					int index = x * sampleLevel + i + y * sampleLevel * width + j * width;
					temp = temp + ColorToVec4f(data[index]);
				}
			}
			temp = temp / ((int)sampleLevel * (int)sampleLevel);
			image->set(x,y,temp);
		}
}