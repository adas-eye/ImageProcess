#ifndef IMGPROCESSOR_H
#define IMGPROCESSOR_H

#include <QMainWindow>
#include <QAction>
#include <QComboBox>
#include <QColor>
#include <QColorDialog>
#include <QMenu>
#include <QMenuBar>
#include <QMatrix>
#include <QMessageBox>
#include <QFont>
#include <QFile>
#include <QFileDialog>
#include <QFontComboBox>
#include <QFontDatabase>
#include <QImage>
#include <QLabel>
#include <QPrinter>
#include <QPrintDialog>
#include <QPainter>
#include <QScrollArea>
#include <QRect>
#include <QSize>
#include <QSpinBox>
#include <QTextDocument>
#include <QToolButton>
#include <QTextCharFormat>
#include <QToolBar>
#include <QTextStream>
#include <QTextCursor>
#include <QTextList>
#include "ImageProcess/ImageProcess.h"


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void createActions(); //创建动作
    void createMenus();   //创建菜单
    void createToolBars();//创建工具栏


protected:
    void paintEvent(QPaintEvent *event);

protected slots:
    void openFile();
    void newFile();
    void saveFile();
    void saveAsFile();
    void undo();

    void peperSaltNoise();
    void showHistgram();
    void meanFilter();
    void medianFilter();

    void edgeRobert();
    void edgeSobel();

    void horzMirror();
    void vertMirror();
    void imageZoomIn();
    void imageZoomOut();
    void imageRotate();

    void about();

private:
    QMenu *fileMenu;   //menu
    QMenu *imageEnhanceMenu;
    QMenu *subimageEnhanceMenu;
    QMenu *edgeDetectMenu;
    QMenu *geometryTransforMenu;
    QMenu *helpMenu;

    QAction *openFileAction; //file
    QAction *newFileAction;
    QAction *savefileAction;
    QAction *undoAction;
    QAction *exitAction;

    QAction *peperSaltNoiseAction;//imageEnhance
    QAction *histgramAction;
    QMenu   *filter;
    QAction *meanFilterAction;
    QAction *medianFilterAction;

    QAction *edgeRobertAction; //edgeDetect
    QAction *edgeSobelAction;

    //geometryTransfor
    QAction *horzMirrorAction;
    QAction *vertMirrorAction;
    QAction *zoomInAction;
    QAction *zoomOutAction;
    QAction *rotateAction;

    QAction *aboutAction;  //help

    QToolBar *fileTool;   //toolBar
    QToolBar *geometryTransfor;
    QToolBar *undoTool;

    QScrollArea *scrollArea; //滚动区域

    QImage       curImage;
    QImage       tmpImage;
    QString      imageFileName;
    QLabel       *imageLabel;
    ImageProcess imageProcess;
    bool         flagLoadImage;//标记是否加载图像
    bool         flagModified; //
};

#endif // IMGPROCESSOR_H
