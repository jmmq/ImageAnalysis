
#ifndef APPICATION_H
#define APPICATION_H
#include <QObject>
#include <QMainWindow>
#include <QMdiArea>
#include <QFileDialog>
#include<QApplication>
#include"imagewidget.h"
class Application : public QApplication{
Q_OBJECT
public:
    Application(int argc, char *argv[]);
    int exec();
    //QMap<mdi> *mdiSubWindowExtensionMap;
private:
    enum WindowTarget{None,Original,New};
    QMainWindow *mainWindow;
    QMdiArea *mdiArea;
    QFileDialog *saveImageDialog;
    QFileDialog *openImageDialog;
    int mdiSubWindowCounter;
    void designFileDialogs();
    void designSaveImageDialog();
    void designOpenImageDialog();
    void design();
    void designMainWindow();
    void designMenu();
    void designMdiArea();
    void display();
    QString *getSaveImageFileName();
    QString *getSaveImageFormat();
    void createSubWindow(const QString &imageName, QImage *image);
    QStringList* getSubWindowsTitles(QList<QMdiSubWindow *>* subWindowsList);
    QMdiSubWindow* getSubWindowFromTitle(QList<QMdiSubWindow *>* subWindowsList,const QString &selectedWindowTitle);
    QImage *imageFromWindow(QMdiSubWindow *subWindow);
    ImageWidget *imageWidgetFromWindow(QMdiSubWindow *subWindow);
    QMdiSubWindow *selectWindow(const QString &imageSymbol);
    QMdiSubWindow *selectWindow();
    WindowTarget selectWindowTarget();
    void updateWindow(QMdiSubWindow *subWindow);
    void noImageSelectedWarning();
public slots:
    void openImage();
    void saveImage();
    void toGrayScale();
    void binarize();
    void binarizeChannel();
    void openChannel();
    void add();
    void subtract();
    void multiply();
    void logicNot();
    void logicAnd();
    void logicOR();
    void logicXor();
    void histogram();
    void meanFilter();
    void gaussianFilter();
    void heavyMeanFilter();
    void sobel();
    void prewitt();
    void roberts();
    void pixelDifference();
    void mode();
    void median();
    void max();
    void min();
    void undo();
    void redo();
    void expansion();
    void compression();
    void displacement();
};
#endif // APPICATION_H
