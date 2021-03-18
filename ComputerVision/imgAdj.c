//
//  imgAdj.c
//  CVEx1
//
//  Created by 오재무 on 2021/03/18.
//

#include <stdio.h>
#include <stdlib.h>
//구조체 안의 padding 지우기
#pragma pack(2)

typedef unsigned char BYTE;
typedef unsigned short WORD;
typedef unsigned int DWORD;

typedef struct tagBITMAPFILEHEADER
{
    WORD bfType;
    DWORD bfSize;
    WORD bfReserved1;
    WORD bfReserved2;
    DWORD bfOffBits;
} BITMAPFILEHEADER;
typedef struct tagBITMAPINFOHEADER
{
    DWORD biSize;
    int biWidth;
    int biHeight;
    WORD biPlanes;
    WORD biBitCount;
    DWORD biCompression;
    DWORD biSizeImage;
    int biXPixPerMeter;
    int biYPixPerMeter;
    DWORD biClrUsed;
    DWORD biClrImporant;
} BITMAPINFOHEADER;
typedef struct tagRGBQUAD
{
    BYTE rgbBlue;
    BYTE rgbGreen;
    BYTE rgbRed;
    BYTE rgbReserved;
} RGBQUAD;

void InverseImage(BYTE * Img, BYTE * Out,int W, int H){
    int imgSize=W*H;
    for(int i=0;i<imgSize;i++){
        Out[i]=255-Img[i];
    }
}

void BrightnessAdj(BYTE * Img, BYTE * Out,int W, int H,int val){
    int imgSize=W*H;
    for(int i=0;i<imgSize;i++){
        if(Img[i]+val>255) Out[i]=255;
        else if(Img[i]+val<0) Out[i]=0;
        else Out[i]=Img[i]+val;
    }
}

void ContrastAdj(BYTE * Img, BYTE * Out,int W, int H, double val){
    int imgSize=W*H;
    for(int i=0;i<imgSize;i++){
        if(Img[i]*val>255.0) Out[i]=255;
        else Out[i]=(BYTE)(Img[i]*val);
    }
}

int main() {
    for(int i=0;i<10;i++)
        printf("=");
    printf("%10s","Processing");
    for(int i=0;i<10;i++)
        printf("=");
    printf("\n");
    
    BITMAPFILEHEADER hf;
    BITMAPINFOHEADER hinfo;
    RGBQUAD hRGB[256];
    FILE *fp;
   
    fp=fopen("/Users/5mac/Desktop/2021_1/ComputerVision/CVEx1/CVEx1/LENNA.bmp", "rb");
    if(fp==NULL) {
        printf("Null");
        return -1;
    }
    fread(&hf,sizeof(BITMAPFILEHEADER), 1, fp);
    fread(&hinfo,sizeof(BITMAPINFOHEADER), 1, fp);
    fread(hRGB, sizeof(RGBQUAD), 256, fp);
    int imgSize = hinfo.biWidth * hinfo.biHeight;
    
    BYTE * Image = (BYTE *)calloc(sizeof(BYTE), imgSize);
    BYTE * Output = (BYTE *)calloc(sizeof(BYTE), imgSize);

    fread(Image, sizeof(BYTE), imgSize, fp);
    fclose(fp);
    
    //영상처리
    //InverseImage(Image, Output, hinfo.biWidth, hinfo.biHeight);
    //BrightnessAdj(Image, Output, hinfo.biWidth, hinfo.biHeight, -70);
    //ContrastAdj(Image,Output, hinfo.biWidth, hinfo.biHeight, 1.5);
    
    fp=fopen("output.bmp", "wb");
    fwrite(&hf, sizeof(BYTE), sizeof(BITMAPFILEHEADER), fp);
    fwrite(&hinfo, sizeof(BYTE), sizeof(BITMAPINFOHEADER), fp);
    fwrite(hRGB, sizeof(RGBQUAD), 256, fp);
    fwrite(Output, sizeof(BYTE), imgSize, fp);
    fclose(fp);
    
    free(Image);
    free(Output);
    
    for(int i=0;i<10;i++)
        printf("=");
    printf("%10s","이미지처리완료");
    for(int i=0;i<10;i++)
        printf("=");
    printf("\n");
    
    return 0;
}
