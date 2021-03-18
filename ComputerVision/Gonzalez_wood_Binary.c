//
//  Gonzalez_Wood_Binary.c
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

void Histogram(BYTE * Img, int * Histo, int W, int H){
    int imgSize=W*H;
    for(int i=0;i<imgSize;i++) Histo[Img[i]]++;
}

void AHistogram(int * AHisto, int * Histo){
    for(int i=1;i<256;i++){
        AHisto[i]=AHisto[i-1]+Histo[i];
    }
}

void Binarization(BYTE * Img, BYTE * Out, int W, int H, BYTE Threshold){
    int imgSize=W*H;
    for(int i=0;i<imgSize;i++){
        if(Img[i]<Threshold) Out[i]=0;
        else Out[i]=255;
    }
}

void Gonzalez(BYTE * Img, BYTE * Out, int W, int H, int * Histo){
    BYTE Low=0,High=255;
    for(int i=0;i<256;i++){
        if(Histo[i]!=0) {
            Low=i;
            break;
        }
    }
    for(int i=255;i>=0;i--){
        if(Histo[i]!=0) {
            High=i;
            break;
        }
    }
   
    int Told= INT16_MAX;
    int Tnew=(Low+High)/2;

    printf("%d %d\n", Low, High);
    printf("start loof\n");
    int t=1;
    while(Told-Tnew>=3&&Tnew-Told<=-3){
        printf("=%d=\n", t++);
        printf("Tnew : %d, Told : %d\n", Tnew, Told);
        
        int down=0,up=0;
        int di=0,ui=0;

        Binarization(Img, Out, W, H, Tnew);

        for(int i=0; i< W*H; i++){
            if(Out[i]<Tnew){
                down+=Img[i];
                di++;
            }
            else{
                up+=Img[i];
                ui++;
            }
        }
        Told = Tnew;
        Tnew = (down/di + up/ui)/2;
    }
    printf("Final Threshhold : %d\n", Tnew);
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
    
    int Histo[256] = {0};
    //Accumulate 누적
    int AHisto[256] = {0};
    
    //히스토그램
    Histogram(Image, Histo, hinfo.biWidth, hinfo.biHeight);
    
    //누적히스토그램
//    AHistogram(AHisto, Histo);
    
    //Gonzalez 이진화
    Gonzalez(Image, Output, hinfo.biWidth, hinfo.biHeight, Histo);
    //이진화 
    // Binarization(Output, Output, hinfo.biWidth, hinfo.biHeight, Threshold);
    
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
