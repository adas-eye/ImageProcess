
#include <QDebug> //测试所需的头文件
#include "MainWindow.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{  
    setWindowTitle(tr("Image Processor"));
    flagLoadImage = false;

    imageLabel = new QLabel;
    imageLabel->setBackgroundRole(QPalette::NoRole);  /*QPalette::Base--基本的颜色(白色						//  ) QPalette::NoRole--继承父类框的背景风格 */
    imageLabel->setSizePolicy(QSizePolicy::Ignored,QSizePolicy::Ignored);
    imageLabel->setScaledContents(true); //选择自动适应框的变化，就是无论将对话框，缩小				// ，放大都不影响像素，作用在看图片的时候，图片的像素会跟着相框调整
	
	scrollArea = new QScrollArea; //滚动区域
	scrollArea ->setBackgroundRole(QPalette::Dark);
	setCentralWidget(scrollArea);
	this->setCentralWidget(scrollArea);
    scrollArea->setWidget(imageLabel);
	
    createActions();
    createMenus();
    createToolBars();

}

MainWindow::~MainWindow()
{
}

void MainWindow::createActions()
{
    //"打开"动作
    openFileAction =new QAction(QIcon(":/res/open.png"),tr("打开"),this);
    openFileAction->setShortcut(tr("Ctrl+O"));
    openFileAction->setStatusTip(tr("打开"));
    connect(openFileAction,SIGNAL(triggered()),this,SLOT(openFile()));

    //"新建"动作
    newFileAction =new QAction(QIcon(":/res/new.png"),tr("新建"),this);
    newFileAction->setShortcut(tr("Ctrl+N"));
    newFileAction->setStatusTip(tr("新建"));
    connect(newFileAction,SIGNAL(triggered()),this,SLOT(newFile()));

    //"保存"动作
    savefileAction =new QAction(QIcon(":/res/save.png"),tr("保存"),this);
    savefileAction->setShortcut(tr("Ctrl+S"));
    savefileAction->setStatusTip(tr("保存"));
    connect(savefileAction,SIGNAL(triggered()),this,SLOT(saveFile()));

    //"取消"动作
    undoAction =new QAction(QIcon(":/res/undo.png"),tr("撤销"),this);
    undoAction->setShortcut(tr("Ctrl+Z"));
    undoAction->setStatusTip(tr("撤销"));
    connect(undoAction, SIGNAL(triggered()), this, SLOT(undo()));

    //"退出"动作
    exitAction =new QAction(tr("退出"),this);
    exitAction->setShortcut(tr("Ctrl+Q"));
    exitAction->setStatusTip(tr("退出"));
    connect(exitAction,SIGNAL(triggered()),this,SLOT(close()));

    //"椒盐噪声"动作
    peperSaltNoiseAction =new QAction(tr("椒盐噪声"),this);
    connect(peperSaltNoiseAction,SIGNAL(triggered()),this,SLOT(peperSaltNoise()));

    //"直方图"动作
    histgramAction =new QAction(tr("直方图"),this);
    connect(histgramAction,SIGNAL(triggered()),this,SLOT(showhistgram()));

    //"均值滤波"操作
    meanFilterAction =new QAction(tr("均值滤波"),this);
    connect(meanFilterAction,SIGNAL(triggered()),this,SLOT(meanFilter()));

    //"中值滤波"操作
    medianFilterAction =new QAction(tr("中值滤波"),this);
    connect(medianFilterAction,SIGNAL(triggered()),this,SLOT(medianFilter()));

    //"边缘检测 Roberts算子"操作
    edgeRobertAction =new QAction(tr("Robert 算子"),this);
    connect(edgeRobertAction,SIGNAL(triggered()),this,SLOT(edgeRobert()));

    //"边缘检测 Sobel算子"操作
    edgeSobelAction =new QAction(tr("Sobel 算子"),this);
    connect(edgeSobelAction,SIGNAL(triggered()),this,SLOT(edgeSobel()));

    //"图像放大"操作
    zoomInAction =new QAction(QIcon(":/res/zoomin.png"), tr("图像放大"),this);
    connect(zoomInAction,SIGNAL(triggered()),this,SLOT(imageZoomIn()));

    //"水平镜像"操作
    horzMirrorAction =new QAction(tr("水平镜像"),this);
    connect(horzMirrorAction,SIGNAL(triggered()),this,SLOT(horzMirror()));

    //"垂直镜像"操作
    vertMirrorAction =new QAction(tr("垂直镜像"),this);
    connect(vertMirrorAction,SIGNAL(triggered()),this,SLOT(vertMirror()));

    //"图像缩小"操作
    zoomOutAction =new QAction(QIcon(":/res/zoomout.png"), tr("图像缩小"),this);
    connect(zoomOutAction,SIGNAL(triggered()),this,SLOT(imageZoomOut()));

    //"图像旋转"操作
    rotateAction =new QAction(QIcon(":/res/rotate90.png"), tr("图像旋转"),this);
    connect(rotateAction,SIGNAL(triggered()),this,SLOT(imageRotate()));

    //"关于"操作
    aboutAction =new QAction(tr("关于"),this);
    connect(aboutAction,SIGNAL(triggered()),this,SLOT(about()));

}

void MainWindow::createMenus()
{
    //"文件"菜单中的 Action
    fileMenu = menuBar()->addMenu(tr("文件"));
    fileMenu->addAction(newFileAction);
    fileMenu->addAction(openFileAction);
    fileMenu->addAction(savefileAction);
    fileMenu->addSeparator();
    fileMenu->addAction(undoAction);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAction);

    //"图像增强"菜单中的Action
    imageEnhanceMenu = menuBar()->addMenu(tr("图像增强"));
    imageEnhanceMenu->addAction(peperSaltNoiseAction);
    imageEnhanceMenu->addAction(histgramAction);
    subimageEnhanceMenu = imageEnhanceMenu->addMenu(tr("滤波"));
    subimageEnhanceMenu->addAction(meanFilterAction);
    subimageEnhanceMenu->addAction(medianFilterAction);


    //"边缘检测"菜单中的Action
    edgeDetectMenu = menuBar()->addMenu(tr("边缘检测"));
    edgeDetectMenu->addAction(edgeRobertAction);
    edgeDetectMenu->addAction(edgeSobelAction);

    //"几何变换"菜单中的Action
    geometryTransforMenu = menuBar()->addMenu(tr("几何变换"));
    geometryTransforMenu->addAction(zoomInAction);
    geometryTransforMenu->addAction(zoomOutAction);
    geometryTransforMenu->addSeparator();
    geometryTransforMenu->addAction(horzMirrorAction);
    geometryTransforMenu->addAction(vertMirrorAction);
    geometryTransforMenu->addSeparator();
    geometryTransforMenu->addAction(rotateAction);

    //"帮助"菜单中的Action
    helpMenu = menuBar()->addMenu(tr("帮助"));
    helpMenu->addAction(aboutAction);
}

void MainWindow::createToolBars()
{
    //
    fileTool = addToolBar("File");
    fileTool->setMovable(true);
    fileTool->addAction(newFileAction);
    fileTool->addAction(openFileAction);
    fileTool->addAction(savefileAction);

    //
    undoTool = addToolBar("Undo");
    undoTool->setMovable(true);
    undoTool->addAction(undoAction);

    //
    geometryTransfor = addToolBar("GeometryTransfor");
    geometryTransfor->setMovable(true);
    geometryTransfor->addAction(zoomInAction);
    geometryTransfor->addAction(zoomOutAction);
    geometryTransfor->addSeparator();
    geometryTransfor->addAction(rotateAction);

}

void MainWindow::paintEvent(QPaintEvent *event)
{
    imageLabel->setPixmap(QPixmap::fromImage(curImage));
    imageLabel->adjustSize();
}

void MainWindow::newFile()
{
    MainWindow *newImgProcessor =new MainWindow;
    newImgProcessor->show();
}

void MainWindow::openFile()
{
    QImage loadImage;
    QString loadFileName = QFileDialog::getOpenFileName(this, tr("Open Image"),\
                                                     ".", tr("Image Files(*.bmp)"));

    if( loadFileName.isEmpty() )
    {
        QMessageBox::warning(NULL, tr("警告"), \
                             tr("没有选择任何bmp格式文件!"));
        return;
    }

    if (!loadImage.load(loadFileName))
    {
        QMessageBox::critical(NULL, tr("打开文件错误"), \
                             tr("打开文件bmp格式文件,失败!"));
        return;
    }

    /*如果图像本身是32、24位的，程序中图像是32位的，如果图像本身是8位、1位的，程序中对应为8位、1位*/

    if( loadImage.format() != QImage::Format_Indexed8 && loadImage.format() != QImage::Format_RGB32 )
    {
        QMessageBox::critical(NULL, tr("打开文件错误"), \
                         tr("只支持8 24 32Bit BMP文件格式!"));

            return;
    }

    //将bmp 8 32bit 转换为24bit  老版本的qt没有QImage::Format_RGB888
    loadImage = loadImage.convertToFormat(QImage::Format_RGB888);


    curImage = loadImage;
    tmpImage = loadImage;
    imageFileName = loadFileName;
    imageProcess.getBitmapData( curImage );
    flagLoadImage = true;
    update();
}

void MainWindow::saveFile()
{
    if( !curImage.save(imageFileName) )
    {
        QMessageBox::critical(NULL, tr("错误"), \
                             tr("保存bmp格式文件,失败!"));

        return;
    }

}

void MainWindow::saveAsFile()
{
    QString saveFileName = QFileDialog::getSaveFileName(this,
                               tr("Save Image"), ".",
                               tr("Image files (*.bmp)"));

    //displayArea->saveImage(saveFileName);
    QImage saveImage = curImage;
    if( !saveImage.save(saveFileName) )
    {
        QMessageBox::critical(NULL, tr("错误"), \
                             tr("保存bmp格式文件,失败!"));

        return;
    }
}

void MainWindow::undo()
{
    curImage = tmpImage;
    update();
}

void MainWindow::peperSaltNoise()
{
    if(flagLoadImage == false)
    {
        QMessageBox::critical(NULL, tr("错误"), \
                             tr("没有加载bmp格式的文件!"));
        return;
    }

    imageProcess.peperSaltNoise( curImage.bits() );
    update();
}

void MainWindow::showHistgram()
{
}

void MainWindow::meanFilter()
{
    if(flagLoadImage == false)
    {
        QMessageBox::critical(NULL, tr("错误"), \
                             tr("没有加载bmp格式的文件!"));
        return;
    }

    imageProcess.meanFilter( curImage.bits() );
    update();
}

void MainWindow::medianFilter()
{
    if(flagLoadImage == false)
    {
        QMessageBox::critical(NULL, tr("错误"), \
                             tr("没有加载bmp格式的文件!"));
        return;
    }

    imageProcess.medianFilter( curImage.bits() );
    update();
}

void MainWindow::edgeRobert()
{
    if(flagLoadImage == false)
    {
        QMessageBox::critical(NULL, tr("错误"), \
                             tr("没有加载bmp格式的文件!"));
        return;
    }

    imageProcess.edgeDetectRoberts( curImage.bits() );
    update();
}

void MainWindow::edgeSobel()
{
    if(flagLoadImage == false)
    {
        QMessageBox::critical(NULL, tr("错误"), \
                             tr("没有加载bmp格式的文件!"));
        return;
    }

    imageProcess.edgeDetectSobel( curImage.bits() );
    update();
}

void MainWindow::horzMirror()
{
    if(flagLoadImage == false)
    {
        QMessageBox::critical(NULL, tr("错误"), \
                             tr("没有加载bmp格式的文件!"));
        return;
    }

    imageProcess.horzMirror( curImage.bits() );
    update();
}

void MainWindow::vertMirror()
{
    if(flagLoadImage == false)
    {
        QMessageBox::critical(NULL, tr("错误"), \
                             tr("没有加载bmp格式的文件!"));
        return;
    }

    imageProcess.vertMirror( curImage.bits() );
    update();
}

void MainWindow::imageZoomIn()
{
}

void MainWindow::imageZoomOut()
{

}

void MainWindow::imageRotate()
{
    if(flagLoadImage == false)
    {
        QMessageBox::critical(NULL, tr("错误"), \
                             tr("没有加载bmp格式的文件!"));
        return;
    }

    imageProcess.imageRotate( curImage.bits() );
    update();
}

void MainWindow::about()
{
    QMessageBox message(QMessageBox::NoIcon, "ImageProcess", tr("<p><b>数字图像处理 V0.10</b></p> <p>作者:董亮 WTU CS0921</p>"));
    message.setIconPixmap(QPixmap(":/res/qt.ico"));
    message.exec();

}













