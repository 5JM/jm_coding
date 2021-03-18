//
//  Avr_Gauss_Convolution.c
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

//Average Convolution(박스 평활화) + 블러 처리
void AverageConv(BYTE * Img, BYTE * Out, int W, int H){
    const int size = 3;
    double Kernel[size][size] =
    //average equalization
                            { 0.1111, 0.1111, 0.1111,
                            0.1111, 0.1111, 0.1111,
                            0.1111, 0.1111, 0.1111 };
    
//    {
//        1,1,1,
//        1,1,1,
//        1,1,1
//    };
    int margin = size/2;
    double SumProduct = 0.0;
    for(int i = margin; i < H - margin; i++){//kernel center Y좌표
        for(int j = margin; j < W - margin; j++){//kernel center X좌표
            for(int m = -margin; m <= margin; m++){//kernel 중심 기준 세로방향
                for(int n = -margin; n <= margin; n++){//kernel 중심 기준 가로방향
                    SumProduct += Img[(i+m)*W+(j+n)]*Kernel[m+margin][n+margin];
                }
            }
            Out[i*W+j] = (BYTE)SumProduct;
            SumProduct = 0.0;
        }
    }
}

//GaussAvr Convolution(가우시안 평활화)
//+ 블러 처리( avr보다 원본에 더 가까이 because center에 가중치 )
void GaussAvrConv(BYTE * Img, BYTE * Out, int W, int H){
    const int size = 3;
    double Kernel[size][size] =
//    {
//        1,2,1,
//        2,4,2,
//        1,2,1
//    };
    {
        0.0625, 0.125, 0.0625,
        0.125, 0.25, 0.125,
        0.0625, 0.125, 0.0625,
    };
    int margin = size/2;
    double SumProduct = 0.0;
    for(int i = margin; i < H - margin; i++){//kernel center Y좌표
        for(int j = margin; j < W - margin; j++){//kernel center X좌표
            for(int m = -margin; m <= margin; m++){//kernel 중심 기준 세로방향
                for(int n = -margin; n <= margin; n++){//kernel 중심 기준 가로방향
                    SumProduct += Img[(i+m)*W+(j+n)]*Kernel[m+margin][n+margin];
                }
            }
            Out[i*W+j] = (BYTE)SumProduct;
            SumProduct = 0.0;
        }
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
   
    //Average Convolution
    AverageConv(Image, Output, hinfo.biWidth, hinfo.biHeight);
    
    //GaussAverage Convolution
    // GaussAvrConv(Image, Output, hinfo.biWidth, hinfo.biHeight);
    
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
