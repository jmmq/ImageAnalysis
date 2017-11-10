#include "application.h"
#include <QtWidgets>
#include <QMenuBar>
#include <QPixmap>
#include<QFileDialog>
#include<imagewidget.h>
#include<imagefunctions.h>
#include"histogramwidget.h"
Application::Application(int argc, char *argv[]):QApplication(argc,argv){
}
int Application::exec(){
    design();
    return QApplication::exec();
}
void Application::design(){
    designMainWindow();
    designMenu();
    designMdiArea();
    designFileDialogs();
    display();
}
void Application::designMainWindow(){
    mainWindow=new QMainWindow();
    mainWindow->setWindowTitle(QApplication::translate("toplevel", "Análisis de imágenes"));
    mainWindow->setAttribute(Qt::WA_DeleteOnClose);
}
void Application::designMenu(){
    QMenu *fileMenu=new QMenu("Archivo");
    fileMenu->addAction("Abrir imagen",this,&Application::openImage,QKeySequence(Qt::CTRL+Qt::Key_O));
    fileMenu->addAction("Guardar imagen",this,&Application::saveImage,QKeySequence(Qt::CTRL+Qt::Key_S));
    mainWindow->menuBar()->addMenu(fileMenu);
    QMenu *toolsMenu=new QMenu("Herramientas");
    toolsMenu->addAction("A escala de grises",this,&Application::toGrayScale,QKeySequence(Qt::CTRL+Qt::Key_G));
    toolsMenu->addAction("Binarizar",this,&Application::binarize,QKeySequence(Qt::CTRL+Qt::Key_B));
    toolsMenu->addAction("Binarizar canal",this,&Application::binarizeChannel,QKeySequence(Qt::CTRL+Qt::Key_I));
    toolsMenu->addAction("Abrir canal",this,&Application::openChannel);
    QMenu *arithmeticMenu=toolsMenu->addMenu("Aritmética");
    arithmeticMenu->addAction("+",this,&Application::add);
    arithmeticMenu->addAction("-",this,&Application::subtract);
    arithmeticMenu->addAction("*",this,&Application::multiply);
    QMenu *logicMenu=toolsMenu->addMenu("Lógica");
    logicMenu->addAction("NOT",this,&Application::logicNot);
    logicMenu->addAction("AND",this,&Application::logicAnd);
    logicMenu->addAction("OR",this,&Application::logicOR);
    logicMenu->addAction("XOR",this,&Application::logicXor);
    toolsMenu->addAction("Histograma",this,&Application::histogram,QKeySequence(Qt::CTRL+Qt::Key_H));
    QMenu *filtersMenu=toolsMenu->addMenu("Filtros");
    QMenu *lowPassFiltersMenu=filtersMenu->addMenu("Pasa bajas");
    lowPassFiltersMenu->addAction("Promedio estándar",this,&Application::meanFilter);
    lowPassFiltersMenu->addAction("Promedio estándar pesado",this,&Application::heavyMeanFilter);
    lowPassFiltersMenu->addAction("Gausiano",this,&Application::gaussianFilter);

    QMenu *highPassFiltersMenu=filtersMenu->addMenu("Pasa altas");
    highPassFiltersMenu->addAction("Sobel",this,&Application::sobel);
    highPassFiltersMenu->addAction("Prewitt",this,&Application::prewitt);
    highPassFiltersMenu->addAction("Roberts",this,&Application::roberts);
    highPassFiltersMenu->addAction("Pixel difference",this,&Application::pixelDifference);

    QMenu *noLinealFiltersMenu=filtersMenu->addMenu("No lineales");
    noLinealFiltersMenu->addAction("Moda",this,&Application::mode);
    noLinealFiltersMenu->addAction("Mediana",this,&Application::median);

    //QAction arithmeticAction=new QAction("Operaciones aritméticas");
    //toolsMenu->addAction();
    mainWindow->menuBar()->addMenu(toolsMenu);
}
void Application::designMdiArea(){
    mdiArea=new QMdiArea();
    mdiArea->setBackground(QBrush(QColor(255,255,255)));
    mdiArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    mdiArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    mainWindow->setCentralWidget(mdiArea);
    mdiSubWindowCounter=0;
}
void Application::display(){
    mainWindow->showMaximized();
}
QString *Application::getSaveImageFileName()
{
    QString* selectedImageFormat=getSaveImageFormat();
    if(selectedImageFormat==nullptr){
        return nullptr;
    }
    if (saveImageDialog->exec() != QDialog::Accepted)
        return nullptr;
    QString *fileName = new QString(saveImageDialog->selectedFiles().first());
    QFileInfo fileInfo(*fileName);
    if(fileInfo.suffix().compare(*selectedImageFormat)!=0){
        fileName->append(".").append(*selectedImageFormat);
    }
    return fileName;
}
QString *Application::getSaveImageFormat()
{
    QStringList supportedImageFormatsStringList;
    QList<QByteArray> supportedImageFormatsList=QImageWriter::supportedImageFormats();
    for(int i=0;i<supportedImageFormatsList.size();i++){
        supportedImageFormatsStringList.append(supportedImageFormatsList.at(i));
    }
    bool ok;
    QString selectedImageFormat=QInputDialog::getItem(mainWindow,"Seleccione un formato",
                                         "Formato:", supportedImageFormatsStringList, 0, false, &ok);
    if (!ok||selectedImageFormat.isEmpty()){
       return nullptr;
    }
    return new QString(selectedImageFormat);
}
void Application::createSubWindow(const QString &imageName, QImage *image)
{
    QMdiSubWindow *mdiSubWindow=new QMdiSubWindow();
    QScrollArea *scrollArea = new QScrollArea();
    ImageWidget *imageWidget=new ImageWidget(image);
    scrollArea->setWidget(imageWidget);
    mdiSubWindow->setWidget(scrollArea);
    mdiSubWindowCounter++;
    QString title("Ventana ");
    title.append(QString::number(mdiSubWindowCounter));
    title.append(" "+imageName);
    mdiSubWindow->setWindowTitle(title);
    mdiSubWindow->setWindowFilePath(imageName);
    mdiSubWindow->setAttribute(Qt::WA_DeleteOnClose);
    QPixmap pixmap(1,1);
    pixmap.fill(QColor(0,0,0,0));
    mdiSubWindow->setWindowIcon(QIcon(pixmap));
    mdiArea->addSubWindow(mdiSubWindow);
    mdiSubWindow->show();
}
QStringList *Application::getSubWindowsTitles(QList<QMdiSubWindow *>* subWindowsList)
{

    QStringList *subWindowsTitles=new QStringList;
    for(int i=0;i<subWindowsList->size();i++){
        *subWindowsTitles<<subWindowsList->at(i)->windowTitle();
    }
    return subWindowsTitles;
}
QMdiSubWindow *Application::getSubWindowFromTitle(QList<QMdiSubWindow *> *subWindowsList, const QString &selectedWindowTitle)
{
    int selectedWidowIndex;
    for(selectedWidowIndex=0;selectedWidowIndex<subWindowsList->size();selectedWidowIndex++){
        if(subWindowsList->at(selectedWidowIndex)->windowTitle()==selectedWindowTitle){
            return subWindowsList->at(selectedWidowIndex);
        }
    }
    return nullptr;
}
QImage *Application::imageFromWindow(QMdiSubWindow *subWindow)
{
    QScrollArea *scrollArea=qobject_cast<QScrollArea *>(subWindow->widget());
    ImageWidget *imageWidget=qobject_cast<ImageWidget *>(scrollArea->widget());
    return imageWidget->getImage();
}

ImageWidget *Application::imageWidgetFromWindow(QMdiSubWindow *subWindow)
{
    QScrollArea *scrollArea=qobject_cast<QScrollArea *>(subWindow->widget());
    if(scrollArea==0){
        return nullptr;
    }
    ImageWidget *imageWidget=qobject_cast<ImageWidget *>(scrollArea->widget());
    if(imageWidget==0){
        return nullptr;
    }
    return imageWidget;
}
QMdiSubWindow *Application::selectWindow(const QString &imageSymbol)
{
    QList<QMdiSubWindow *> subWindowsList=mdiArea->subWindowList();
    if(subWindowsList.empty()){
        noImageSelectedWarning();
        return nullptr;
    }
    QStringList *subWindowsTitles;
    if((subWindowsTitles=getSubWindowsTitles(&subWindowsList))==nullptr){
        return nullptr;
    }
    bool ok;
    QString selectedWindowTitle = QInputDialog::getItem(mainWindow,"Seleccione la imagen "+imageSymbol,
                                         "Imagen "+imageSymbol+":", *subWindowsTitles, 0, false, &ok);
    if (!ok||selectedWindowTitle.isEmpty()){
       return nullptr;
    }
    QMdiSubWindow* selectedSubWindow;
    if((selectedSubWindow=getSubWindowFromTitle(&subWindowsList,selectedWindowTitle))==nullptr){
        return nullptr;
    }
    return selectedSubWindow;
}
QMdiSubWindow *Application::selectWindow()
{
    return selectWindow("");
}
Application::WindowTarget Application::selectWindowTarget()
{
    bool ok;
    QStringList options;
    options<<"Ventana original"<<"Nueva Ventana";
    QString target = QInputDialog::getItem(mainWindow, tr("Seleccionar ventana"),
                                         "Mostrar resultado en:", options, 0, false, &ok);
    if (!ok||options.isEmpty()){
       return None;
    }
    if(target.compare(options.at(0))==0){
        return Original;
    }else{
        return New;
    }
}
void Application::updateWindow(QMdiSubWindow *subWindow)
{
    QScrollArea *scrollArea=qobject_cast<QScrollArea *>(subWindow->widget());
    ImageWidget *imageWidget=qobject_cast<ImageWidget *>(scrollArea->widget());
    scrollArea->update();
    subWindow->update();
}
void Application::noImageSelectedWarning()
{
    QMessageBox msgBox;
    msgBox.setWindowTitle("Advertencia");
    msgBox.setText("Ninguna imagen ha sido seleccionada");
    msgBox.exec();
}
void Application::designFileDialogs()
{
    designSaveImageDialog();
    designOpenImageDialog();
}
void Application::designSaveImageDialog()
{
    saveImageDialog=new QFileDialog(
                mainWindow,
                "Guardar imagen",
                QStandardPaths::standardLocations(QStandardPaths::PicturesLocation).first()
    );
    saveImageDialog->setFileMode(QFileDialog::AnyFile);
    saveImageDialog->setAcceptMode(QFileDialog::AcceptSave);
}
void Application::designOpenImageDialog()
{
    openImageDialog=new QFileDialog(
                mainWindow,
                "Abrir imagen",
                QStandardPaths::standardLocations(QStandardPaths::PicturesLocation).first()
    );

    QStringList nameFileters;
    QString imagesNameFilter("Imágenes (");
    QList<QByteArray> supportedImageFormatsList=QImageReader::supportedImageFormats();
    for(int i=0;i<supportedImageFormatsList.size();i++){
        if(i<supportedImageFormatsList.size()-1){
            imagesNameFilter.append("*"+QString(supportedImageFormatsList.at(i))+" ");
        }else{
            imagesNameFilter.append("*"+QString(supportedImageFormatsList.at(i)));
        }
    }
    imagesNameFilter.append(")");
    nameFileters.append(imagesNameFilter);
    openImageDialog->setNameFilters(nameFileters);
    /*QStringList mimeTypes;
    foreach (const QByteArray &bf, QImageReader::supportedMimeTypes()){
        mimeTypes.append(QString(bf));
    }
    openImageDialog->setMimeTypeFilters(mimeTypes);*/
    openImageDialog->setAcceptMode(QFileDialog::AcceptOpen);
}
void Application::openImage(){
    if(openImageDialog->exec()==QFileDialog::Rejected){
        return;
    }
    QString openedImageFileName=openImageDialog->selectedFiles().at(0);
    if(openedImageFileName.isNull()||openedImageFileName.isEmpty()){
        return;
    }
    QImage *image=new QImage(openedImageFileName);
    QTextStream info(stdout);
    info<<"Opened image original format code: "<<QString::number(image->format())<<"\n";
    QMessageBox mb;
    if(image->format()!=QImage::Format_RGB32){
        image=new QImage(image->convertToFormat(QImage::Format_RGB32));
        if(
            image->isNull()||
            (image->format()!=QImage::Format_RGB32))
        {
            mb.setText("No se pudo convertir imagen con formato inválido código: "+QString::number(image->format()));
            mb.exec();
            return;
        }
    }
    createSubWindow(QFileInfo(openedImageFileName).fileName(),image);
    info<<"Opened image final format code: "<<QString::number(image->format())<<"\n";
    info.flush();
}
void Application::saveImage(){
    QMessageBox msgBox;
    QMdiSubWindow* selectedWindow;
    selectedWindow=mdiArea->activeSubWindow();
    if(selectedWindow==0){
        noImageSelectedWarning();
        return;
    }
    QImage *selectedImage=imageFromWindow(selectedWindow);
    if(selectedImage==nullptr||selectedImage->isNull()){
        return;
    }
    QString *savedImageFileName=getSaveImageFileName();
    if(savedImageFileName==nullptr){
        return;
    }
    if(savedImageFileName->isNull()||savedImageFileName->isEmpty()){
        return;
    }
    if(selectedImage->save(*savedImageFileName)){
        msgBox.setText("Imagen guardada exitosamente "+*savedImageFileName);
    }else{
        QMessageBox msgBox;
        msgBox.setText("No se pudo guardar la imagen "+*savedImageFileName);
    }
    msgBox.exec();

}
void Application::toGrayScale()
{
    QMdiSubWindow* selectedWindow;
    selectedWindow=mdiArea->activeSubWindow();
    if(selectedWindow==0){
        noImageSelectedWarning();
        return;
    }
    ImageWidget *imageWidget=imageWidgetFromWindow(selectedWindow);
    if(imageWidget==nullptr){
        return;
    }
    WindowTarget windowTarget=selectWindowTarget();
    if(windowTarget==None){
        return;
    }
    QImage *newImage=ImageFunctions::toGrayScale(imageWidget->getImage(),true);
    if(windowTarget==New){
        createSubWindow(selectedWindow->windowFilePath(),newImage);
    }else{
        imageWidget->updateImage(newImage);
        updateWindow(selectedWindow);
    }
}
void Application::binarize()
{
    QMdiSubWindow* selectedWindow;
    selectedWindow=mdiArea->activeSubWindow();
    if(selectedWindow==0){
        noImageSelectedWarning();
        return;
    }
    ImageWidget *imageWidget=imageWidgetFromWindow(selectedWindow);
    if(imageWidget==nullptr){
        return;
    }
    bool ok;
    int threshold = QInputDialog::getInt(mainWindow, tr("Binarizar"),
                                       tr("Umbral:"), 128, 0, 255, 1, &ok);
    if(!ok){
        return;
    }
    bool inverse;
    QStringList inverseStringList;
    inverseStringList<<"Estandár"<<"Inversa";
    QString inverseString = QInputDialog::getItem(mainWindow, tr("Binarizar"),
                                         "Tipo de binarización:", inverseStringList, 0, false, &ok);
    if(!ok||inverseString.isEmpty()){
       return;
    }
    if(inverseString.compare(inverseStringList.at(0))==0){
        inverse=false;
    }else{
        inverse=true;
    }
    WindowTarget windowTarget=selectWindowTarget();
    if(windowTarget==None){
        return;
    }
    QImage *newImage=ImageFunctions::binarize(imageWidget->getImage(),true,threshold,inverse);
    if(windowTarget==New){
        createSubWindow(selectedWindow->windowFilePath(),newImage);
    }else{
        imageWidget->updateImage(newImage);
        updateWindow(selectedWindow);
    }
}
void Application::binarizeChannel()
{
    QMdiSubWindow* selectedWindow;
    selectedWindow=mdiArea->activeSubWindow();
    if(selectedWindow==0){
        noImageSelectedWarning();
        return;
    }
    ImageWidget *imageWidget=imageWidgetFromWindow(selectedWindow);
    if(imageWidget==nullptr){
        return;
    }
    bool ok;
    QStringList channelsList;
    channelsList<<"Rojo"<<"Verde"<<"Azul";
    QString channelString = QInputDialog::getItem(mainWindow, tr("Binarizar"),
                                         "Canal:", channelsList, 0, false, &ok);
    if(!ok||channelString.isEmpty()){
       return;
    }
    ImageFunctions::Channel channel;
    if(channelString.compare(channelsList.at(0))==0){
        channel=ImageFunctions::Red;
    }
    if(channelString.compare(channelsList.at(1))==0){
        channel=ImageFunctions::Green;
    }
    if(channelString.compare(channelsList.at(2))==0){
        channel=ImageFunctions::Blue;
    }
    int threshold = QInputDialog::getInt(mainWindow, tr("Binarizar"),
                                       tr("Umbral:"), 128, 0, 255, 1, &ok);
    if(!ok){
        return;
    }
    bool inverse;
    QStringList inverseStringList;
    inverseStringList<<"Estandár"<<"Inversa";
    QString inverseString = QInputDialog::getItem(mainWindow, tr("Binarizar"),
                                         "Tipo de binarización:", inverseStringList, 0, false, &ok);
    if(!ok||inverseString.isEmpty()){
       return;
    }
    if(inverseString.compare(inverseStringList.at(0))==0){
        inverse=false;
    }else{
        inverse=true;
    }
    WindowTarget windowTarget=selectWindowTarget();
    if(windowTarget==None){
        return;
    }
    QImage *newImage=ImageFunctions::binarizeChannel(imageWidget->getImage(),true,threshold,inverse,channel);
    if(windowTarget==New){
        createSubWindow(selectedWindow->windowFilePath(),newImage);
    }else{
        imageWidget->updateImage(newImage);
        updateWindow(selectedWindow);
    }
}
void Application::openChannel()
{
    QMdiSubWindow* selectedWindow;
    selectedWindow=mdiArea->activeSubWindow();
    if(selectedWindow==0){
        noImageSelectedWarning();
        return;
    }
    ImageWidget *imageWidget=imageWidgetFromWindow(selectedWindow);
    if(imageWidget==nullptr){
        return;
    }
    bool ok;
    QStringList channelsList;
    channelsList<<"Rojo"<<"Verde"<<"Azul";
    QString channelString = QInputDialog::getItem(mainWindow,"Selecciona un canal",
                                         "Canal:", channelsList, 0, false, &ok);
    if(!ok||channelString.isEmpty()){
       return;
    }
    ImageFunctions::Channel channel;
    if(channelString.compare(channelsList.at(0))==0){
        channel=ImageFunctions::Red;
    }
    if(channelString.compare(channelsList.at(1))==0){
        channel=ImageFunctions::Green;
    }
    if(channelString.compare(channelsList.at(2))==0){
        channel=ImageFunctions::Blue;
    }
    WindowTarget windowTarget=selectWindowTarget();
    if(windowTarget==None){
        return;
    }
    QImage *newImage=ImageFunctions::openChannel(imageWidget->getImage(),true,channel);
    if(windowTarget==New){
        createSubWindow(selectedWindow->windowFilePath(),newImage);
    }else{
        imageWidget->updateImage(newImage);
        updateWindow(selectedWindow);
    }
}
void Application::add()
{
    QMdiSubWindow* selectedWindow;
    selectedWindow=mdiArea->activeSubWindow();
    if(selectedWindow==0){
        noImageSelectedWarning();
        return;
    }
    ImageWidget *imageWidget=imageWidgetFromWindow(selectedWindow);
    if(imageWidget==nullptr){
        return;
    }
    QMdiSubWindow* selectedWindowB;
    if((selectedWindowB=selectWindow("B"))==nullptr){
        return;
    }
    ImageWidget *imageWidgetB=imageWidgetFromWindow(selectedWindow);
    if(imageWidgetB==nullptr){
        return;
    }
    WindowTarget windowTarget=selectWindowTarget();
    if(windowTarget==None){
        return;
    }
    QImage *newImage=ImageFunctions::addition(imageWidget->getImage(),imageWidgetB->getImage(),true);
    if(windowTarget==New){
        createSubWindow(selectedWindow->windowFilePath()+" + "+selectedWindowB->windowFilePath(),newImage);
    }else{
        imageWidget->updateImage(newImage);
        updateWindow(selectedWindow);
    }
}
void Application::subtract()
{
    QMdiSubWindow* selectedWindow;
    selectedWindow=mdiArea->activeSubWindow();
    if(selectedWindow==0){
        noImageSelectedWarning();
        return;
    }
    ImageWidget *imageWidget=imageWidgetFromWindow(selectedWindow);
    if(imageWidget==nullptr){
        return;
    }
    QMdiSubWindow* selectedWindowB;
    if((selectedWindowB=selectWindow("B"))==nullptr){
        return;
    }
    ImageWidget *imageWidgetB=imageWidgetFromWindow(selectedWindow);
    if(imageWidgetB==nullptr){
        return;
    }
    WindowTarget windowTarget=selectWindowTarget();
    if(windowTarget==None){
        return;
    }
    QImage *newImage=ImageFunctions::subtraction(imageWidget->getImage(),imageWidgetB->getImage(),true);
    if(windowTarget==New){
        createSubWindow(selectedWindow->windowFilePath()+" - "+selectedWindowB->windowFilePath(),newImage);
    }else{
        imageWidget->updateImage(newImage);
        updateWindow(selectedWindow);
    }
}
void Application::multiply()
{
    QMdiSubWindow* selectedWindow;
    selectedWindow=mdiArea->activeSubWindow();
    if(selectedWindow==0){
        noImageSelectedWarning();
        return;
    }
    ImageWidget *imageWidget=imageWidgetFromWindow(selectedWindow);
    if(imageWidget==nullptr){
        return;
    }
    QMdiSubWindow* selectedWindowB;
    if((selectedWindowB=selectWindow("B"))==nullptr){
        return;
    }
    ImageWidget *imageWidgetB=imageWidgetFromWindow(selectedWindow);
    if(imageWidgetB==nullptr){
        return;
    }
    WindowTarget windowTarget=selectWindowTarget();
    if(windowTarget==None){
        return;
    }
    QImage *newImage=ImageFunctions::multiplication(imageWidget->getImage(),imageWidgetB->getImage(),true);
    if(windowTarget==New){
        createSubWindow(selectedWindow->windowFilePath()+" * "+selectedWindowB->windowFilePath(),newImage);
    }else{
        imageWidget->updateImage(newImage);
        updateWindow(selectedWindow);
    }
}

void Application::logicNot()
{
    QMdiSubWindow* selectedWindow;
    selectedWindow=mdiArea->activeSubWindow();
    if(selectedWindow==0){
        noImageSelectedWarning();
        return;
    }
    ImageWidget *imageWidget=imageWidgetFromWindow(selectedWindow);
    if(imageWidget==nullptr){
        return;
    }
    WindowTarget windowTarget=selectWindowTarget();
    if(windowTarget==None){
        return;
    }
    QImage *newImage=ImageFunctions::logicNot(imageWidget->getImage(),true);
    if(windowTarget==New){
        createSubWindow(selectedWindow->windowFilePath(),newImage);
    }else{
        imageWidget->updateImage(newImage);
        updateWindow(selectedWindow);
    }
}
void Application::logicAnd()
{
    QMdiSubWindow* selectedWindow;
    selectedWindow=mdiArea->activeSubWindow();
    if(selectedWindow==0){
        noImageSelectedWarning();
        return;
    }
    ImageWidget *imageWidget=imageWidgetFromWindow(selectedWindow);
    if(imageWidget==nullptr){
        return;
    }
    QMdiSubWindow* selectedWindowB;
    if((selectedWindowB=selectWindow("B"))==nullptr){
        return;
    }
    ImageWidget *imageWidgetB=imageWidgetFromWindow(selectedWindow);
    if(imageWidgetB==nullptr){
        return;
    }
    WindowTarget windowTarget=selectWindowTarget();
    if(windowTarget==None){
        return;
    }
    QImage *newImage=ImageFunctions::logicAND(imageWidget->getImage(),imageWidgetB->getImage(),true);
    if(windowTarget==New){
        createSubWindow(selectedWindow->windowFilePath()+" & "+selectedWindowB->windowFilePath(),newImage);
    }else{
        imageWidget->updateImage(newImage);
        updateWindow(selectedWindow);
    }
}
void Application::logicOR()
{
    QMdiSubWindow* selectedWindow;
    selectedWindow=mdiArea->activeSubWindow();
    if(selectedWindow==0){
        noImageSelectedWarning();
        return;
    }
    ImageWidget *imageWidget=imageWidgetFromWindow(selectedWindow);
    if(imageWidget==nullptr){
        return;
    }
    QMdiSubWindow* selectedWindowB;
    if((selectedWindowB=selectWindow("B"))==nullptr){
        return;
    }
    ImageWidget *imageWidgetB=imageWidgetFromWindow(selectedWindow);
    if(imageWidgetB==nullptr){
        return;
    }
    WindowTarget windowTarget=selectWindowTarget();
    if(windowTarget==None){
        return;
    }
    QImage *newImage=ImageFunctions::logicOR(imageWidget->getImage(),imageWidgetB->getImage(),true);
    if(windowTarget==New){
        createSubWindow(selectedWindow->windowFilePath()+" | "+selectedWindowB->windowFilePath(),newImage);
    }else{
        imageWidget->updateImage(newImage);
        updateWindow(selectedWindow);
    }
}
void Application::logicXor()
{
    QMdiSubWindow* selectedWindow;
    selectedWindow=mdiArea->activeSubWindow();
    if(selectedWindow==0){
        noImageSelectedWarning();
        return;
    }
    ImageWidget *imageWidget=imageWidgetFromWindow(selectedWindow);
    if(imageWidget==nullptr){
        return;
    }
    QMdiSubWindow* selectedWindowB;
    if((selectedWindowB=selectWindow("B"))==nullptr){
        return;
    }
    ImageWidget *imageWidgetB=imageWidgetFromWindow(selectedWindow);
    if(imageWidgetB==nullptr){
        return;
    }
    WindowTarget windowTarget=selectWindowTarget();
    if(windowTarget==None){
        return;
    }
    QImage *newImage=ImageFunctions::logicXOR(imageWidget->getImage(),imageWidgetB->getImage(),true);
    if(windowTarget==New){
        createSubWindow(selectedWindow->windowFilePath()+" ^ "+selectedWindowB->windowFilePath(),newImage);
    }else{
        imageWidget->updateImage(newImage);
        updateWindow(selectedWindow);
    }
}
void Application::histogram(){
    QMdiSubWindow* selectedWindow;
    selectedWindow=mdiArea->activeSubWindow();
    if(selectedWindow==0){
        noImageSelectedWarning();
        return;
    }
    ImageWidget *imageWidget=imageWidgetFromWindow(selectedWindow);
    if(imageWidget==nullptr){
        return;
    }
    HistogramWidget *histogramWidget=new HistogramWidget(selectedWindow->windowTitle(),imageWidget->getImage());
    histogramWidget->show();
}
void Application::meanFilter()
{
    QMdiSubWindow* selectedWindow;
    selectedWindow=mdiArea->activeSubWindow();
    if(selectedWindow==0){
        noImageSelectedWarning();
        return;
    }
    ImageWidget *imageWidget=imageWidgetFromWindow(selectedWindow);
    if(imageWidget==nullptr){
        return;
    }
    WindowTarget windowTarget=selectWindowTarget();
    if(windowTarget==None){
        return;
    }
    QImage *newImage=ImageFunctions::meanFilter(imageWidget->getImage(),true);
    if(windowTarget==New){
        createSubWindow(selectedWindow->windowFilePath(),newImage);
    }else{
        imageWidget->updateImage(newImage);
        updateWindow(selectedWindow);
    }
}

void Application::gaussianFilter()
{
    QMdiSubWindow* selectedWindow;
    selectedWindow=mdiArea->activeSubWindow();
    if(selectedWindow==0){
        noImageSelectedWarning();
        return;
    }
    ImageWidget *imageWidget=imageWidgetFromWindow(selectedWindow);
    if(imageWidget==nullptr){
        return;
    }
    WindowTarget windowTarget=selectWindowTarget();
    if(windowTarget==None){
        return;
    }
    QImage *newImage=ImageFunctions::gaussianFilter(imageWidget->getImage(),true);
    if(windowTarget==New){
        createSubWindow(selectedWindow->windowFilePath(),newImage);
    }else{
        imageWidget->updateImage(newImage);
        updateWindow(selectedWindow);
    }
}

void Application::heavyMeanFilter()
{
    QMdiSubWindow* selectedWindow;
    selectedWindow=mdiArea->activeSubWindow();
    if(selectedWindow==0){
        noImageSelectedWarning();
        return;
    }
    ImageWidget *imageWidget=imageWidgetFromWindow(selectedWindow);
    if(imageWidget==nullptr){
        return;
    }
    bool ok;
    int N=QInputDialog::getInt(mainWindow, tr("Peso N"),
                                       tr("N:"), 2, 0, 255, 1, &ok);
    if(!ok){
        return;
    }
    WindowTarget windowTarget=selectWindowTarget();
    if(windowTarget==None){
        return;
    }
    QImage *newImage=ImageFunctions::heavyMeanFilter(imageWidget->getImage(),N,true);
    if(windowTarget==New){
        createSubWindow(selectedWindow->windowFilePath(),newImage);
    }else{
        imageWidget->updateImage(newImage);
        updateWindow(selectedWindow);
    }
}

void Application::sobel()
{
    QMdiSubWindow* selectedWindow;
    selectedWindow=mdiArea->activeSubWindow();
    if(selectedWindow==0){
        noImageSelectedWarning();
        return;
    }
    ImageWidget *imageWidget=imageWidgetFromWindow(selectedWindow);
    if(imageWidget==nullptr){
        return;
    }
    WindowTarget windowTarget=selectWindowTarget();
    if(windowTarget==None){
        return;
    }
    QImage *newImage=ImageFunctions::sobel(imageWidget->getImage(),true);
    if(windowTarget==New){
        createSubWindow(selectedWindow->windowFilePath(),newImage);
    }else{
        imageWidget->updateImage(newImage);
        updateWindow(selectedWindow);
    }
}

void Application::prewitt()
{
    QMdiSubWindow* selectedWindow;
    selectedWindow=mdiArea->activeSubWindow();
    if(selectedWindow==0){
        noImageSelectedWarning();
        return;
    }
    ImageWidget *imageWidget=imageWidgetFromWindow(selectedWindow);
    if(imageWidget==nullptr){
        return;
    }
    WindowTarget windowTarget=selectWindowTarget();
    if(windowTarget==None){
        return;
    }
    QImage *newImage=ImageFunctions::prewitt(imageWidget->getImage(),true);
    if(windowTarget==New){
        createSubWindow(selectedWindow->windowFilePath(),newImage);
    }else{
        imageWidget->updateImage(newImage);
        updateWindow(selectedWindow);
    }
}

void Application::roberts()
{
    QMdiSubWindow* selectedWindow;
    selectedWindow=mdiArea->activeSubWindow();
    if(selectedWindow==0){
        noImageSelectedWarning();
        return;
    }
    ImageWidget *imageWidget=imageWidgetFromWindow(selectedWindow);
    if(imageWidget==nullptr){
        return;
    }
    WindowTarget windowTarget=selectWindowTarget();
    if(windowTarget==None){
        return;
    }
    QImage *newImage=ImageFunctions::roberts(imageWidget->getImage(),true);
    if(windowTarget==New){
        createSubWindow(selectedWindow->windowFilePath(),newImage);
    }else{
        imageWidget->updateImage(newImage);
        updateWindow(selectedWindow);
    }
}

void Application::pixelDifference()
{
    QMdiSubWindow* selectedWindow;
    selectedWindow=mdiArea->activeSubWindow();
    if(selectedWindow==0){
        noImageSelectedWarning();
        return;
    }
    ImageWidget *imageWidget=imageWidgetFromWindow(selectedWindow);
    if(imageWidget==nullptr){
        return;
    }
    WindowTarget windowTarget=selectWindowTarget();
    if(windowTarget==None){
        return;
    }
    QImage *newImage=ImageFunctions::pixelDifference(imageWidget->getImage(),true);
    if(windowTarget==New){
        createSubWindow(selectedWindow->windowFilePath(),newImage);
    }else{
        imageWidget->updateImage(newImage);
        updateWindow(selectedWindow);
    }
}

void Application::mode()
{
    QMdiSubWindow* selectedWindow;
    selectedWindow=mdiArea->activeSubWindow();
    if(selectedWindow==0){
        noImageSelectedWarning();
        return;
    }
    ImageWidget *imageWidget=imageWidgetFromWindow(selectedWindow);
    if(imageWidget==nullptr){
        return;
    }
    WindowTarget windowTarget=selectWindowTarget();
    if(windowTarget==None){
        return;
    }
    QImage *newImage=ImageFunctions::mode(imageWidget->getImage(),true);
    if(windowTarget==New){
        createSubWindow(selectedWindow->windowFilePath(),newImage);
    }else{
        imageWidget->updateImage(newImage);
        updateWindow(selectedWindow);
    }
}

void Application::median()
{
    QMdiSubWindow* selectedWindow;
    selectedWindow=mdiArea->activeSubWindow();
    if(selectedWindow==0){
        noImageSelectedWarning();
        return;
    }
    ImageWidget *imageWidget=imageWidgetFromWindow(selectedWindow);
    if(imageWidget==nullptr){
        return;
    }
    WindowTarget windowTarget=selectWindowTarget();
    if(windowTarget==None){
        return;
    }
    QImage *newImage=ImageFunctions::median(imageWidget->getImage(),true);
    if(windowTarget==New){
        createSubWindow(selectedWindow->windowFilePath(),newImage);
    }else{
        imageWidget->updateImage(newImage);
        updateWindow(selectedWindow);
    }
}
