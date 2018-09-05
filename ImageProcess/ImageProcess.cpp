#include "ImageProcess.h"
#include <QMessageBox>
#include <QTime>
#include <QDebug>

ImageProcess::ImageProcess()
{}

ImageProcess::ImageProcess(QImage image)
{
    biFormat   = image.format();
    biWidth    = image.width();
    biHeight   = image.height();
    biDataSize = image.byteCount();
}

ImageProcess::~ImageProcess()
{}

void ImageProcess::getBitmapData(QImage image)
{
    biFormat   = image.format();
    biWidth    = image.width();
    biHeight   = image.height();
    biDataSize = image.byteCount();
}

void ImageProcess::peperSaltNoise(BYTE *pImageData)
{
    int i,j;        //循环变量,i:外层 j:内层
    int pos;        //在图像的数据中的位置
    int byteWidth;  //宽度有多少个字节, 3*宽度

    if (pImageData == 0) //没有图像数据
        return;

    byteWidth = biWidth*3; //在32位计算机中,数据4个字节对齐
    if (byteWidth%4)
        byteWidth += 4-(byteWidth%4);

   qsrand(QTime(0,0,0).msecsTo(QTime::currentTime())); //随机数的种子

    for(i=0; i<biHeight; i++)
    {
        for(j=0; j<biWidth; j++)
        {
            if (rand() > 31500) //如果当前随机产生一个数大于31500,那么就将该位置的RGB的值全都设为255
            {
                pos               = (j*3) + (i*byteWidth);
                pImageData[pos]   = 255;
                pImageData[pos+1] = 255;
                pImageData[pos+2] = 255;
            }
        }
    }
}


void ImageProcess::meanFilter(BYTE *pImageData)
{
    int i,  j;
    int i1, j1;
    int pos;
    int sumR, sumG, sumB;
    int byteWidth;

    BYTE *pImageDataTemp=(BYTE *)malloc(sizeof(BYTE)*biDataSize);

    byteWidth = biWidth*3;
    if (byteWidth%4)
        byteWidth += 4-(byteWidth%4);

    for(i=1; i<biHeight-1; i++)
	{
        for(j=1; j<biWidth-1; j++)
		{
            pos = j*3 + i*byteWidth;
            sumR = 0;
            sumG = 0;
            sumB = 0;

            for(i1=-1; i1<=1; i1++)
            {
                for(j1=-1; j1<=1; j1++)
                {
                    sumB += pImageData[(i+i1)*byteWidth+(j+j1)*3];
                    sumG += pImageData[(i+i1)*byteWidth+(j+j1)*3+1];
                    sumR += pImageData[(i+i1)*byteWidth+(j+j1)*3+2];
                }
            }

            pImageDataTemp[pos+2] = sumR/9;
            pImageDataTemp[pos+1] = sumG/9;
            pImageDataTemp[pos]   = sumB/9;
		}
	}

    memcpy(pImageData, pImageDataTemp, biDataSize);
    free(pImageDataTemp);
}

void ImageProcess::medianFilter(BYTE *pImageData)
{
    int i,  j;
    int i1, j1;
    int byteWidth;
    BYTE p[9],tmp;
    BYTE *pImageDataTemp=(BYTE *)malloc(sizeof(BYTE)*biDataSize);

    byteWidth = biWidth*3;
    if (byteWidth%4)
        byteWidth += 4-(byteWidth%4);

    for(i=1; i<biHeight-1; i++)
	{
        for(j=3; j<biWidth*3-3; j++)
		{

            p[0] = pImageData[j-3+(i-1)*byteWidth];
            p[1] = pImageData[j+(i-1)*byteWidth];
            p[2] = pImageData[j+3+(i-1)*byteWidth];
            p[3] = pImageData[j-3+i*byteWidth];
            p[4] = pImageData[j+i*byteWidth];
            p[5] = pImageData[j+3+i*byteWidth];
            p[6] = pImageData[j-3+(i+1)*byteWidth];
            p[7] = pImageData[j+(i+1)*byteWidth];
            p[8] = pImageData[j+3+(i+1)*byteWidth];

            //将数组p进行选择排序,仅仅对于前5个
            for(i1=0; i1<5; i1++)
			{
                for(j1=i1+1; j1<9; j1++)
				{
                    if (p[i1] > p[j1])
					{
                        tmp   = p[i1];
                        p[i1] = p[j1];
                        p[j1] = tmp;
					}
				}
			}

            pImageDataTemp[j+i*byteWidth] = p[4];
		}
	}

    memcpy(pImageData, pImageDataTemp, biDataSize);
    free(pImageDataTemp);
}

void ImageProcess::edgeDetectRoberts(BYTE *pImageData)
{
    int x,y;
	int d;
    BYTE *pImageDataTemp0=(BYTE *)malloc(sizeof(BYTE)*biDataSize);
    BYTE *pImageDataTemp1=(BYTE *)malloc(sizeof(BYTE)*biDataSize);

    memset(pImageDataTemp1,0,biDataSize);
    memcpy(pImageDataTemp0, pImageData, biDataSize);
    int byteWidth = biWidth*3;
    if (byteWidth%4)
        byteWidth += 4-(byteWidth%4);

    bitmapToGray(pImageDataTemp0);

    for(y=0;y<biHeight-1;y++)
	{
        for(x=0;x<biWidth-1;x++)
		{
            d=(abs(Point(x,y)-Point(x+1,y+1))+abs(Point(x+1,y)-Point(x,y+1)));
            if (d>100)
                d=255;
            Point1(x,y)=(BYTE)d;
		}
	}

    int p1;
    int p;
    for(y=0;y<biHeight;y++)
    {
        for(x=0;x<biWidth;x++)
        {
            p=x*3+y*byteWidth;
            p1=x+y*biWidth;
            pImageData[p]=pImageDataTemp1[p1];
            pImageData[p+1]=pImageDataTemp1[p1];
            pImageData[p+2]=pImageDataTemp1[p1];
        }
    }

}

void ImageProcess::edgeDetectSobel(BYTE *pImageData)
{
    int x,y;

    BYTE *pImageDataTemp0=(BYTE *)malloc(sizeof(BYTE)*biDataSize);
    BYTE *pImageDataTemp1=(BYTE *)malloc(sizeof(BYTE)*biDataSize);
    int d,max;
    int Gx,Gy;
    static int s[2][9]={
        {-1,0,1,-2,0,2,-1,0,1},
        {-1,-2,-1,0,0,0,1,2,1}
    };

    memset(pImageDataTemp1,0,biDataSize);
    memcpy(pImageDataTemp0, pImageData, biDataSize);

    int byteWidth = biWidth*3;
    if (byteWidth%4)
        byteWidth += 4-(byteWidth%4);


    bitmapToGray(pImageDataTemp0);

    int x1;
    int y1,i;
    for(y=1;y<biHeight-1;y++)
    {
        for(x=1;x<biWidth-1;x++)
        {

            Gx =(Point(x+1, y-1)+2*Point(x+1,y)+Point(x+1,y+1))-(Point(x-1,y-1)+2*Point(x-1,y)+Point(x-1,y+1));
            Gy =(Point(x-1, y+1)+2*Point(x,y+1)+Point(x+1,y+1))-(Point(x-1,y-1)+2*Point(x,y-1)+Point(x+11,y-1));

            d = abs(Gx)+abs(Gy);
            if (d>255)
                d=255;
            Point1(x,y)=(BYTE)d;
        }
    }


    int p1;
    int p;
    for(y=0;y<biHeight;y++)
    {
        for(x=0;x<biWidth;x++)
        {
            p=x*3+y*byteWidth;
            p1=x+y*biWidth;
            pImageData[p]=pImageDataTemp1[p1];
            pImageData[p+1]=pImageDataTemp1[p1];
            pImageData[p+2]=pImageDataTemp1[p1];
        }
    }
}

void ImageProcess::horzMirror(BYTE *pImageData)
{
    int x,y;
    BYTE *pImageDataTemp=(BYTE *)malloc(sizeof(BYTE)*biDataSize);

    int byteWidth = biWidth*3;
    if (byteWidth%4)
        byteWidth += 4-(byteWidth%4);

    for(y=0;y<biHeight;y++)
    {
        for(x=0;x<biWidth;x++)
        {
            pImageDataTemp[x*3+y*byteWidth]=pImageData[(biWidth-1-x)*3+y*byteWidth];
            pImageDataTemp[x*3+1+y*byteWidth]=pImageData[(biWidth-1-x)*3+1+y*byteWidth];
            pImageDataTemp[x*3+2+y*byteWidth]=pImageData[(biWidth-1-x)*3+2+y*byteWidth];
        }
    }

    memcpy(pImageData, pImageDataTemp, biDataSize);
    free(pImageDataTemp);
}

void ImageProcess::vertMirror(BYTE *pImageData)
{
    int x,y;
    BYTE *pImageDataTemp=(BYTE *)malloc(sizeof(BYTE)*biDataSize);

    int byteWidth = biWidth*3;
    if (byteWidth%4)
        byteWidth += 4-(byteWidth%4);

    for(y=0;y<biHeight;y++)
    {
        for(x=0;x<byteWidth;x++)
        {
            pImageDataTemp[x+y*byteWidth]=pImageData[x+(biHeight-y-1)*byteWidth];
        }
    }

    memcpy(pImageData, pImageDataTemp, biDataSize);
    free(pImageDataTemp);
}

void ImageProcess::imageRotate(BYTE *pImageData)
{

}

void ImageProcess::bitmapToGray(BYTE *ImageData)
{
    int x,y,p;
    int byteWidth;
    BYTE *pImageDataTemp=(BYTE *)malloc(sizeof(BYTE)*biDataSize);

    byteWidth = biWidth*3;
    if (byteWidth%4)
        byteWidth += 4-(byteWidth%4);

    for(y=0;y<biHeight;y++)
    {
        for(x=0;x<biWidth;x++)
        {
            p=x*3+y*byteWidth;
            pImageDataTemp[x+y*biWidth]=(BYTE)(0.299*(float)ImageData[p+2]+ \
                                               0.587*(float)ImageData[p+1]+ \
                                               0.114*(float)ImageData[p]+0.1);
        }
    }

    memcpy(ImageData, pImageDataTemp, biDataSize);
    free(pImageDataTemp);
}

void ImageProcess::bmp8BitTo24Bit(QImage *image)
{
    QImage destImage24(biWidth, biHeight, QImage::Format_RGB888);

    if(image->format() != QImage::Format_RGB888)
    {
        if(image->format() != QImage::Format_Indexed8)
        {
            QMessageBox::critical(NULL, QObject::tr("错误"),QObject::tr("加载bmp格式文件,失败!"));
            return ;
        }

    //将8Bit BMP转换为 24Bit BMP

    }//end of if


}

