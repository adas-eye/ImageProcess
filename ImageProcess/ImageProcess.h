
#include <QImage>

#define Point(x,y) pImageDataTemp0[(x)+(y)*biWidth]
#define Point1(x,y) pImageDataTemp1[(x)+(y)*biWidth]

typedef quint8  BYTE; //BYTE表示8位无符号整数，一个字节
typedef quint16 WORD; //WORD表示16位无符号整数，两个字节
typedef quint32 DWORD; //DWORD表示32位无符号整数，四个个字节
typedef qint32  LONG; //LONG表示32位整数，四个字节

class ImageProcess{

public:
	ImageProcess();
    ImageProcess(QImage);
	~ImageProcess();
	
    void peperSaltNoise(BYTE *);  //椒盐噪声
    void meanFilter(BYTE *);      //均值滤波
    void medianFilter(BYTE *);    //中值滤波

    void edgeDetectRoberts(BYTE *); //边缘检测 Roberts算子
    void edgeDetectSobel(BYTE *);   //边缘检测 Sobel算子
	
    void horzMirror(BYTE *);        //水平镜像
    void vertMirror(BYTE *);        //垂直镜像
    //void imageZoomIn(BYTE *);     //图像放大
    //void imageZoomOut(BYTE *);    //图像缩小
    void imageRotate(BYTE *);	    //图像旋转

    void bitmapToGray(BYTE *); //将位图转换为灰度图
    void bmp8BitTo24Bit(QImage *);
    void getBitmapData(QImage);
	
private:
    BYTE  biFormat;   //位图文件类型
    LONG  biWidth;    //位图的宽度
    LONG  biHeight;   //位图的高度
    DWORD biDataSize; //位图中数据大小,以byte为单位

};
