//
//  main.c
//  CVEx1
//
//  Created by 오재무 on 2021/03/10.
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

void Histogram(BYTE * Img, int * Histo, int W, int H){
    int imgSize=W*H;
    for(int i=0;i<imgSize;i++) Histo[Img[i]]++;
}

void AHistogram(int * AHisto, int * Histo){
    for(int i=1;i<256;i++){
        AHisto[i]=AHisto[i-1]+Histo[i];
    }
}

void HistogramStretching(BYTE * Image, BYTE * Output,int * Histo, int W,int H){
    int imgSize = W*H;
    //Low, High 구하기
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
    for(int i=0;i<imgSize;i++){
        Output[i]=(BYTE)((Image[i]-Low)/(double)(High-Low)*255.0);
    }
}

void HistogramEqualization(BYTE * Image, BYTE * Output,int * AHisto, int W,int H){
    int Nt=W*H;//픽셀 갯수
    int Gmax=255;//최대 밝기
    double Ratio = Gmax/(double)Nt;
    int imgSize=W*H;
    
    BYTE NormSum[256];
    //정규화합
    for(int i=0;i<256;i++){
        NormSum[i]=(BYTE)(Ratio * AHisto[i]);
    }
    for(int i=0;i<imgSize;i++){
        Output[i]=NormSum[Image[i]];
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

//Prewitt_X Convolution + 경계선 검출
void Prewitt_X_Conv(BYTE * Img, BYTE * Out, int W, int H){
    const int size = 3;
    double Kernel[size][size] =
    //Prewitt_X
                            {
                            -1.0, 0.0, 1.0,
                            -1.0, 0.0, 1.0,
                            -1.0, 0.0, 1.0 };
    
    int margin = size/2;
    double SumProduct = 0.0;
    for(int i = margin; i < H - margin; i++){//kernel center Y좌표
        for(int j = margin; j < W - margin; j++){//kernel center X좌표
            for(int m = -margin; m <= margin; m++){//kernel 중심 기준 세로방향
                for(int n = -margin; n <= margin; n++){//kernel 중심 기준 가로방향
                    SumProduct += Img[(i+m)*W+(j+n)]*Kernel[m+margin][n+margin];
                }
            }
            //0~765 ====> 0~255
            Out[i*W+j] = (BYTE)(labs((long)SumProduct) / size);
            SumProduct = 0.0;
        }
    }
}

//Prewitt_Y Convolution + 경계선 검출
void Prewitt_Y_Conv(BYTE * Img, BYTE * Out, int W, int H){
    const int size = 3;
    double Kernel[size][size] =
    //Prewitt_Y
                            {
                            -1.0, -1.0, -1.0,
                            0.0, 0.0, 0.0,
                            1.0, 1.0, 1.0 };
    
    int margin = size/2;
    double SumProduct = 0.0;
    for(int i = margin; i < H - margin; i++){//kernel center Y좌표
        for(int j = margin; j < W - margin; j++){//kernel center X좌표
            for(int m = -margin; m <= margin; m++){//kernel 중심 기준 세로방향
                for(int n = -margin; n <= margin; n++){//kernel 중심 기준 가로방향
                    SumProduct += Img[(i+m)*W+(j+n)]*Kernel[m+margin][n+margin];
                }
            }
            Out[i*W+j] = (BYTE)(labs((long)SumProduct) / size);
            SumProduct = 0.0;
        }
    }
}

//Sobel_X Convolution + 경계선 검출
void Sobel_X_Conv(BYTE * Img, BYTE * Out, int W, int H){
    const int size = 3;
    double Kernel[size][size] =
    //Sobel_X_Conv
                            {
                            -1.0, 0.0, 1.0,
                            -2.0, 0.0, 2.0,
                            -1.0, 0.0, 1.0 };
    
    int margin = size/2;
    double SumProduct = 0.0;
    for(int i = margin; i < H - margin; i++){//kernel center Y좌표
        for(int j = margin; j < W - margin; j++){//kernel center X좌표
            for(int m = -margin; m <= margin; m++){//kernel 중심 기준 세로방향
                for(int n = -margin; n <= margin; n++){//kernel 중심 기준 가로방향
                    SumProduct += Img[(i+m)*W+(j+n)]*Kernel[m+margin][n+margin];
                }
            }
            Out[i*W+j] = (BYTE)(labs((long)SumProduct) / size);
            SumProduct = 0.0;
        }
    }
}

//Sobel_Y Convolution + 경계선 검출
void Sobel_Y_Conv(BYTE * Img, BYTE * Out, int W, int H){
    const int size = 3;
    double Kernel[size][size] =
    //Sobel_Y_Conv
                            {
                            -1.0, -2.0, -1.0,
                            0.0, 0.0, 0.0,
                            1.0, 2.0, 1.0 };
    
    int margin = size/2;
    double SumProduct = 0.0;
    for(int i = margin; i < H - margin; i++){//kernel center Y좌표
        for(int j = margin; j < W - margin; j++){//kernel center X좌표
            for(int m = -margin; m <= margin; m++){//kernel 중심 기준 세로방향
                for(int n = -margin; n <= margin; n++){//kernel 중심 기준 가로방향
                    SumProduct += Img[(i+m)*W+(j+n)]*Kernel[m+margin][n+margin];
                }
            }
            Out[i*W+j] = (BYTE)(labs((long)SumProduct) / size);
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
//    fp=fopen("/Users/5mac/Desktop/2021_1/ComputerVision/CVEx1/CVEx1/outputPrX.bmp", "rb");
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
    BYTE * Temp = (BYTE *)calloc(sizeof(BYTE), imgSize);

    fread(Image, sizeof(BYTE), imgSize, fp);
    fclose(fp);
    
//    printf("%lu\n",sizeof(BITMAPFILEHEADER));
//    printf("%lu\n",sizeof(BITMAPINFOHEADER));
//    printf("%d\n",hf.bfType);
//    printf("%d\n",hf.bfType);
//    printf("%d\n",hf.bfSize);
//    printf("%d\n",hf.bfReserved1);
//    printf("%d\n",hf.bfReserved2);
//    printf("%d\n",hf.bfOffBits);
    
    int Histo[256] = {0};
    //Accumulate 누적
    int AHisto[256] = {0};
    
    //히스토그램
    Histogram(Image, Histo, hinfo.biWidth, hinfo.biHeight);
    
    //누적히스토그램
//    AHistogram(AHisto, Histo);
    
    //스트레칭, strectching
    //HistogramStretching(Image,Output,Histo,hinfo.biWidth, hinfo.biHeight);
    
//    평활화, equalization
//    HistogramEqualization(Image, Output, AHisto, hinfo.biWidth, hinfo.biHeight);
    
    //Gonzalez 이진화
//    Gonzalez(Image, Output, hinfo.biWidth, hinfo.biHeight, Histo);
    
    //Average Convolution
//    AverageConv(Image, Output, hinfo.biWidth, hinfo.biHeight);
    
    //Prewitt
//    Prewitt_X_Conv(Image, Temp, hinfo.biWidth, hinfo.biHeight);
//    Prewitt_Y_Conv(Image, Output, hinfo.biWidth, hinfo.biHeight);
    
    //Sobel
    Sobel_X_Conv(Image, Temp, hinfo.biWidth, hinfo.biHeight);
    Sobel_Y_Conv(Image, Output, hinfo.biWidth, hinfo.biHeight);
    
    for(int i=0; i < imgSize; i++){
        if(Temp[i] > Output[i]) Output[i] = Temp[i];
    }
//    Gonzalez(Output, Output, hinfo.biWidth, hinfo.biHeight, Histo);
    Binarization(Output, Output, hinfo.biWidth, hinfo.biHeight, 40);
    //Gauss
//    GaussAvrConv(Image, Output, hinfo.biWidth, hinfo.biHeight);
    //이진화
//    Binarization(Image, Output, hinfo.biWidth, hinfo.biHeight, Thresh);
    
    //중간 영상처리
    //InverseImage(Image, Output, hinfo.biWidth, hinfo.biHeight);
    //BrightnessAdj(Image, Output, hinfo.biWidth, hinfo.biHeight, -70);
    //ContrastAdj(Image,Output, hinfo.biWidth, hinfo.biHeight, 1.5);
    
    fp=fopen("outputSobel.bmp", "wb");
    fwrite(&hf, sizeof(BYTE), sizeof(BITMAPFILEHEADER), fp);
    fwrite(&hinfo, sizeof(BYTE), sizeof(BITMAPINFOHEADER), fp);
    fwrite(hRGB, sizeof(RGBQUAD), 256, fp);
    fwrite(Output, sizeof(BYTE), imgSize, fp);
    fclose(fp);
    free(Image);
    free(Output);
    free(Temp);
    
    for(int i=0;i<10;i++)
        printf("=");
    printf("%10s","이미지처리완료");
    for(int i=0;i<10;i++)
        printf("=");
    printf("\n");
    
    return 0;
}
