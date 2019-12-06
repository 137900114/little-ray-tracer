#include "Image.h"
#include <iostream>

typedef unsigned UINT;
typedef unsigned char BYTE;

//PNG文件的标头
const char PNG_HEAD[] = {0x89,0x50,0x4E,0x47,0x0D,0x0A,0x1A,0x0A};
const char PNG_TILE[] = {0x00,0x00,0x00,0x00,0x49,0x45,0x4E,0x44,0xAE,0x42,0x60,0x82};
const UINT CRC_TABLE[] = { 0, 0x1db71064, 0x3b6e20c8, 0x26d930ac, 0x76dc4190, 0x6b6b51f4, 0x4db26158, 0x5005713c, 
							0xedb88320, 0xf00f9344, 0xd6d6a3e8, 0xcb61b38c, 0x9b64c2b0, 0x86d3d2d4, 0xa00ae278, 0xbdbdf21c };

const UINT IHDR = 0x52444849, IDAT = 0x54414449, IEND = 0x444E4549;
struct CHUNK_HEADER{
	UINT ChunkLength;
	UINT ChunkType;
};

enum PNG_COLOR_SPACE {
	RGBCOLOR = 2,       //真彩图像
};

//文件头数据块
struct IHDR_CHUNK {
	CHUNK_HEADER ChunkHeader;
	UINT Width;
	UINT Height;//图像宽和深度
	BYTE Depth;
	BYTE ColorType;//png 所用的颜色空间的类型
	BYTE DONTCARE[3];//我们不关心的乱七八糟的东西默认值为{0，0，0}
};


inline UINT getSmallEdian(UINT i);

class PNGImage : public Image {
public:
	PNGImage(UINT width,UINT height,PNG_COLOR_SPACE type);
	
	void save(const char* filename);
	int get_width() override;
	int get_height() override;
	Color get(int x,int y);
	bool set(int x,int y, Color color);
	void flip_vertically();
	~PNGImage() {delete[] data;}
private:
	static void writeWithCRC(std::ostream& out,BYTE b,UINT& c);
	inline static void writeWithCRC(std::ostream& out,const BYTE* b,UINT length,UINT& c);
	static UINT writeHead(std::ostream& out , CHUNK_HEADER head);
	BYTE* data;
	UINT width,height;
	PNG_COLOR_SPACE ColorType;

};
