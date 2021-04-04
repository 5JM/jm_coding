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

void Binarization(BYTE * Img, BYTE * Out, int W, int H, BYTE Threshold){
    int imgSize=W*H;
    for(int i=0;i<imgSize;i++){
        if(Img[i]<Threshold) Out[i]=0;
        else Out[i]=255;
    }
}

void InverseImage(BYTE * Img, BYTE * Out,int W, int H){
    int imgSize=W*H;
    for(int i=0;i<imgSize;i++){
        Out[i]=255-Img[i];
    }
}

// GlassFire 알고리즘을 이용한 라벨링 함수
void m_BlobColoring(BYTE* CutImage, int height, int width)
{
    int i, j, m, n, top, area, Out_Area, index, BlobArea[1000];
    long k;
    short curColor = 0, r, c;
    //    BYTE** CutImage2;
    Out_Area = 1;


    // 스택으로 사용할 메모리 할당
//    short* stackx = new short[height * width];
//    short* stacky = new short[height * width];
//    short* coloring = new short[height * width];

    short * stackx = (short *)calloc(sizeof(short), height * width);
    short * stacky = (short *)calloc(sizeof(short), height * width);
    short * coloring = (short *)calloc(sizeof(short), height * width);
    
    int arr_size = height * width;

    // 라벨링된 픽셀을 저장하기 위해 메모리 할당
    for (k = 0; k < height * width; k++) coloring[k] = 0;  // ∏ﬁ∏∏Æ √ ±‚»≠

    for (i = 0; i < height; i++)
    {
        index = i * width;
        for (j = 0; j < width; j++)
        {
            // 이미 방문한 점이거나 픽셀값이 255가 아니라면 처리x
            if (coloring[index + j] != 0 || CutImage[index + j] != 255) continue;
            r = i; c = j; top = 0; area = 1;
            curColor++;

            while (1)
            {
            GRASSFIRE:
                for (m = r - 1; m <= r + 1; m++)
                {
                    index = m * width;
                    for (n = c - 1; n <= c + 1; n++)
                    {
                        //관심 픽셀이 영상 경계를 벗어나면 처리x
                        if (m < 0 || m >= height || n < 0 || n >= width) continue;

                        if ((int)CutImage[index + n] == 255 && coloring[index + n] == 0)
                        {
                            coloring[index + n] = curColor; // 현재 라벨로 마크
                            if (push(stackx, stacky, arr_size, (short)m, (short)n, &top) == -1) continue;
                            r = m; c = n; area++;
                            goto GRASSFIRE;
                        }
                    }
                }
                if (pop(stackx, stacky, &r, &c, &top) == -1) break;
            }
            if (curColor < 1000) BlobArea[curColor] = area;
        }
    }

    float grayGap = 255.0f / (float)curColor;

    // 가장 면적이 넓은 영역을 찾아내기 위함
    for (i = 1; i <= curColor; i++)
    {
        if (BlobArea[i] >= BlobArea[Out_Area]) Out_Area = i;
    }
    // CutImage 배열 클리어
    for (k = 0; k < width * height; k++) {
        CutImage[k] = 255;
    }
    // coloring에 저장된 라벨링 결과중 ( Out_Area에 저장된 )영역이 가장 큰 것만 CutImage에 저장
    for (k = 0; k < width * height; k++)
    {
        if (coloring[k] == Out_Area) CutImage[k] = 0;  // 가장 큰 것만 저장
//        CutImage[k] = (unsigned char)(coloring[k] * grayGap); //라벨링된 결과를 그대로 출력
    }

    free(coloring);
    free(stackx);
    free(stacky);
//    delete[] coloring;
//    delete[] stackx;
//    delete[] stacky;
}

void DrawRectOutline(BYTE * Img, int W, int H, int LU_X, int LU_Y, int RD_X, int RD_Y){
    //윗 선 그리기
    for(int i = LU_X; i < RD_X; i++) Img[LU_Y * W + i] = 255;
    //아래 선 그리기
    for(int i = LU_X; i < RD_X; i++) Img[RD_Y * W + i] = 255;
    //왼쪽 선 그리기
    for(int i = LU_Y; i < RD_Y; i++) Img[i * W + LU_X] = 255;
    //오른쪽 선 그리기
    for(int i = LU_Y; i < RD_Y; i++) Img[i * W + RD_X] = 255;
}

void DrawCrossLine(BYTE * Img, int W, int H, int Cx, int Cy){
    //horizontal line
    for(int i = 0; i<W-1;i++) Img[Cy*W+i]=255;
    //vertical line
    for(int i = 0; i<H-1;i++) Img[i*W+Cx]=255;
}

void Obtain2DCenter(BYTE * Image,int W, int H, int * Cx, int * Cy){
    //무게중심
    int SumX = 0, SumY = 0;
    int count = 0;
  
    for(int i=0;i<H;i++){
        for(int j = 0;j<W;j++){
            if(Image[i*W+j]==0){//동공
                SumX+=j;
                SumY+=i;
                count++;
            }
        }
    }
    if(count==0) count = 1;
    *Cx = SumX / count;
    *Cy = SumY / count;
}

void Obtain2DBoundingBox(BYTE*Image, int W, int H, int *luX, int *luY, int *rdX, int *rdY){
    int flag = 0;
    
    //luY
    for(int i = 0; i < H; i++){
        for(int j = 0; j < W; j++){
            if(Image[i*W+j]==0){
                *luY=i;
                flag=1;
                break;
            }
        }
        if(flag==1) break;
    }
    
    //rdY
    flag=0;
    for(int i = H-1; i >= 0; i--){
        for(int j = 0; j < W; j++){
            if(Image[i*W+j]==0){
                *rdY=i;
                flag=1;
                break;
            }
        }
        if(flag==1) break;
    }
    
    //luX
    flag=0;
    for(int j = 0; j < W; j++){
        for(int i = 0; i < H; i++){
            if(Image[i*W+j]==0){
                *luX=j;
                flag=1;
                break;
            }
        }
        if(flag==1) break;
    }
    
    //rdX
    flag=0;
    for(int j = W-1; j >= 0 ; j--){
        for(int i = 0; i < H; i++){
            if(Image[i*W+j]==0){
                *rdX=j;
                flag=1;
                break;
            }
        }
        if(flag==1) break;
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
    fp=fopen("/Users/5mac/Desktop/2021_1/ComputerVision/CVEx1/CVEx1/pupil2.bmp", "rb");
//    fp=fopen("/Users/5mac/Desktop/2021_1/ComputerVision/CVEx1/CVEx1/coin.bmp", "rb");
//    fp=fopen("/Users/5mac/Desktop/2021_1/ComputerVision/CVEx1/CVEx1/LENNA_gauss.bmp", "rb");
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
   
    //이진화
    Binarization(Image, Output, hinfo.biWidth, hinfo.biHeight, 50);
    
    //중간 영상처리
    InverseImage(Output, Output, hinfo.biWidth, hinfo.biHeight);
   
    m_BlobColoring(Output, hinfo.biHeight, hinfo.biWidth);
    
    
    int Cx, Cy;
    int luX, luY, rdX, rdY;
    //이진 영상의 무게중심
    Obtain2DCenter(Output, hinfo.biWidth, hinfo.biHeight, &Cx, &Cy);
    //이진영상의 외접 사각형 좌표
    Obtain2DBoundingBox(Output, hinfo.biWidth, hinfo.biHeight, &luX, &luY, &rdX, &rdY);

    DrawCrossLine(Image, hinfo.biWidth, hinfo.biHeight, Cx, Cy);
    DrawRectOutline(Image, hinfo.biWidth, hinfo.biHeight, luX, luY, rdX, rdY);

    fp=fopen("output_bin_pupil2_test.bmp", "wb");
    fwrite(&hf, sizeof(BYTE), sizeof(BITMAPFILEHEADER), fp);
    fwrite(&hinfo, sizeof(BYTE), sizeof(BITMAPINFOHEADER), fp);
    fwrite(hRGB, sizeof(RGBQUAD), 256, fp);
//    fwrite(Output, sizeof(BYTE), imgSize, fp);
    fwrite(Image, sizeof(BYTE), imgSize, fp);
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

