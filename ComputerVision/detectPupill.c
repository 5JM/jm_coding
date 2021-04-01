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

void Swap(BYTE * a, BYTE * b){
    BYTE temp = *a;
    *a = *b;
    *b = temp;
}

BYTE Sorting(BYTE * arr, int size){
    const int center = size / 2;
    for(int i = 0; i < size - 1; i++){
        for(int j = i+1; j < size; j++){
            if(arr[i] > arr[j]) Swap(&arr[i], &arr[j]);
        }
    }
    return arr[center];
}
//pepper noise 전용
BYTE MaxPooling(BYTE * arr, int size){
    const int max = size - 1;
    for(int i = 0; i < size - 1; i++){
        for(int j = i+1; j < size; j++){
            if(arr[i] > arr[j]) Swap(&arr[i], &arr[j]);
        }
    }
    return arr[max];
}

//salt noise
BYTE MinPooling(BYTE * arr, int size){
    for(int i = 0; i < size - 1; i++){
        for(int j = i+1; j < size; j++){
            if(arr[i] > arr[j]) Swap(&arr[i], &arr[j]);
        }
    }
    return arr[0];
}
//noise removal
//median filtering
void  Median(BYTE * Img, BYTE * Out, int W, int H){
    BYTE Temp[9];
    int size = 3;
    int margin = size / 2;
    
    for(int i = margin; i < H - margin; i++){
        for(int j = margin; j < W - margin; j++){
            for(int m = -margin; m <= margin; m++){
                for(int n = -margin; n <= margin; n++){
                    Temp[(m+margin)*size+(n+margin)] = Img[(i+m)*W+(j+n)];
                }
            }
            Out[i*W+j]=Sorting(Temp, 9);
//            Out[i*W+j]=MaxPooling(Temp, 9);
//            Out[i*W+j]=MinPooling(Temp, 9);
        }
    }
}

/*          Labelling         */
 
//라벨링 함수 시작
int push(short* stackx, short* stacky, int arr_size, short vx, short vy, int* top)
{
    if (*top >= arr_size) return(-1);
    (*top)++;
    stackx[*top] = vx;
    stacky[*top] = vy;
    return(1);
}

int pop(short* stackx, short* stacky, short* vx, short* vy, int* top)
{
    if (*top == 0) return(-1);
    *vx = stackx[*top];
    *vy = stacky[*top];
    (*top)--;
    return(1);
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
    //CPP
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

    //CPP
//    delete[] coloring;
//    delete[] stackx;
//    delete[] stacky;
}

// 라벨링 후 가장 넓은 영역에 대해서만 뽑아내는 코드 포함
//라벨링 코드 끝

//이진 파일 경계 검출
void BinaryImgEdgeDetection(BYTE* Bin, BYTE* Out, int W, int H){
    for(int i = 0; i < H; i++){
        for(int j =0; j < W; j++){
            if(Bin[i*W + j] == 0){//그림 화소라면,
                if(!(Bin[(i-1) * W + j] == 0 &&
                     Bin[(i+1) * W + j] == 0 &&
                     Bin[i * W + (j-1)] ==0 &&
                     Bin[(i-1) * W + (j+1)] == 0)){
                    Out[i * W + j] = 255;
                }
            }
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
    Binarization(Image, Temp, hinfo.biWidth, hinfo.biHeight, 50);
    
    //중간 영상처리
    InverseImage(Temp, Output, hinfo.biWidth, hinfo.biHeight);
    //BrightnessAdj(Image, Output, hinfo.biWidth, hinfo.biHeight, -70);
    //ContrastAdj(Image,Output, hinfo.biWidth, hinfo.biHeight, 1.5);
    
    m_BlobColoring(Output, hinfo.biHeight, hinfo.biWidth);
    
//    for(int i = 0; i < imgSize; i++) Output[i]=Image[i];//배경을 input이미지로
//    for(int i = 0; i < imgSize; i++) Output[i] = 255;//배경을 하얗게
    //경계 검출
    BinaryImgEdgeDetection(Temp, Output, hinfo.biWidth, hinfo.biHeight);
   
    
    fp=fopen("output_bin_pupil2_test.bmp", "wb");
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

