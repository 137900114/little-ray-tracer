#pragma once
#include "Image.h"

class ImageMASS : public Image
{
public:
	ImageMASS(size_t width, size_t height, unsigned char sampleLevel = 2);

	bool set(int x, int y, Color c) override;

	Color get(int x, int y) override;

	void sample(Image* image);

	int get_width() override;

	int get_height() override;

	~ImageMASS() { delete[] data; }

private:
	size_t width;
	size_t height;
	unsigned char sampleLevel;
	Color* data;
};
