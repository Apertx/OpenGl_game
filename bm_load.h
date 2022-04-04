#ifndef BM_LOAD
#define BM_LOAD

#include <stdio.h>

typedef struct {
  unsigned int id;
  float ratio;
} bm_texture;

bm_texture loadBitmap(const char* imagepath) {
  bm_texture bmt;
	unsigned char header[54];
	unsigned int dataPos;
	unsigned int width;
	unsigned int height;
	unsigned int imageSize;

	FILE* file = fopen(imagepath, "rb");
	if(!file) {
	  printf("BMP load fail!");
	  return bmt;
	}
	if(fread(&header, 1, 54, file) != 54) {
    printf("BMP load fail");
	  return bmt;
	}
	dataPos = *(int*)&(header[0x0A]);
	imageSize = *(int*)&(header[0x22]);
	width = *(int*)&(header[0x12]);
	height = *(int*)&(header[0x16]);
  bmt.ratio = (float)width / height;
	if(imageSize == 0) imageSize = width * height * 3;
	if(dataPos == 0) dataPos = 54;

	unsigned char data[imageSize];
	fread(&data, 1, imageSize, file);
  unsigned int x_width = width * 3;
  unsigned int x_height = imageSize / 2;
  unsigned int x_j = imageSize;
  for(unsigned int j = 0; j < x_height; j += x_width) {
    x_j -= x_width;
    for(unsigned int i = 0; i < x_width; i += 3) {
      data[j + i] ^= data[x_j + i + 2];
      data[x_j + i + 2] ^= data[j + i];
      data[j + i] ^= data[x_j + i + 2];
      data[j + i + 1] ^= data[x_j + i + 1];
      data[x_j + i + 1] ^= data[j + i + 1];
      data[j + i + 1] ^= data[x_j + i + 1];
      data[j + i + 2] ^= data[x_j + i];
      data[x_j + i] ^= data[j + i + 2];
      data[j + i + 2] ^= data[x_j + i];
  	}
  }

	fclose(file);
	glBindTexture(GL_TEXTURE_2D, 0);
	glGenTextures(1, &bmt.id);
	glBindTexture(GL_TEXTURE_2D, bmt.id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, &data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glBindTexture(GL_TEXTURE_2D, 0);
	return bmt;
}

#endif
