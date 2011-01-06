#include "HeatMapVisualization.h"

HeatMapVisualization::HeatMapVisualization(QSize resolution) : QWidget() {

    mainLayout = new QVBoxLayout();

    lastEvents = NULL;
    height = resolution.height();
    width = resolution.width();
    qDebug() << "height: " << height << " width: " << width;

    heatMap = new int*[height];
    for (int i = 0; i < height; ++i)
        heatMap[i] = new int[width];

    clearHeatMap();

    marge = 25;
    leftClick = true;
    rightClick = true;
    click = true;
    doubleClick = true;

    screenWidth = 500;
    screenHeight = 300;

    image = new QImage(width, height, QImage::Format_RGB32);

    heatMapLabel = new QLabel();
    /*heatMapLabel->setMinimumWidth(screenWidth);
    heatMapLabel->setMaximumWidth(screenWidth);
    heatMapLabel->setMinimumHeight(screenHeight);
    heatMapLabel->setMaximumHeight(screenHeight);*/
    heatMapLabel->setPixmap(QPixmap::fromImage(*image));
    mainLayout->addWidget(heatMapLabel, 0, Qt::AlignCenter);

    QHBoxLayout *checkButtonLayout = new QHBoxLayout();
    QGroupBox *box1 = new QGroupBox();
    QVBoxLayout *box1Layout = new QVBoxLayout();
    leftClickCheckBox = new QCheckBox("&Linker muisklik", this);
    rightClickCheckBox = new QCheckBox("&Rechter muisklik", this);
    QGroupBox *box2 = new QGroupBox();
    QVBoxLayout *box2Layout = new QVBoxLayout();
    clickCheckBox = new QCheckBox("&Eén muisklik", this);
    doubleClickCheckBox = new QCheckBox("&Dubbel muisklik", this);

    leftClickCheckBox->setCheckState(Qt::Checked);
    rightClickCheckBox->setCheckState(Qt::Checked);
    clickCheckBox->setCheckState(Qt::Checked);
    doubleClickCheckBox->setCheckState(Qt::Checked);

    connect(leftClickCheckBox, SIGNAL(stateChanged(int)), this, SLOT(updateParameters(int)));
    connect(rightClickCheckBox, SIGNAL(stateChanged(int)), this, SLOT(updateParameters(int)));
    connect(clickCheckBox, SIGNAL(stateChanged(int)), this, SLOT(updateParameters(int)));
    connect(doubleClickCheckBox, SIGNAL(stateChanged(int)), this, SLOT(updateParameters(int)));

    box1Layout->addWidget(leftClickCheckBox);
    box1Layout->addWidget(rightClickCheckBox);
    box2Layout->addWidget(clickCheckBox);
    box2Layout->addWidget(doubleClickCheckBox);

    box1->setLayout(box1Layout);
    box2->setLayout(box2Layout);

    checkButtonLayout->addWidget(box1);
    checkButtonLayout->addWidget(box2);

    QLabel *margeLabel = new QLabel("Marge rond elke klik:");
    margeSpinBox = new QSpinBox();
    margeSpinBox->setRange(1, 100);
    margeSpinBox->setValue(marge);
    connect(margeSpinBox, SIGNAL(valueChanged(int)), this, SLOT(updateMarge(int)));

    QGroupBox *box3 = new QGroupBox();
    QVBoxLayout *box3Layout = new QVBoxLayout();

    box3Layout->addWidget(margeLabel);
    box3Layout->addWidget(margeSpinBox);

    box3->setLayout(box3Layout);
    checkButtonLayout->addWidget(box3);

    mainLayout->addLayout(checkButtonLayout);

    renderVisualization();

    this->setLayout(mainLayout);
    this->show();
}

void HeatMapVisualization::update(QVector<Event*> *events = NULL) {
    //qDebug() << "HeatMapVisualization::update()";
    clearHeatMap();
    if (events != NULL)
        lastEvents = events;
    else if (lastEvents != NULL)
        events = lastEvents;
    else
        return;
    for (int i = 0; i < events->count(); ++i) {
        if ((click && events->at(i)->getEventType().compare("MouseButtonPress") == 0) || (doubleClick && events->at(i)->getEventType().compare("MouseButtonDblClick") == 0)) {
            //qDebug() << "Type: " << events->at(i)->getEventType();

            QString details = events->at(i)->getDetails();
            details.remove("\"");//remove "

            QStringList args = details.split(";");
            if (args.count() >= 2) {
                int x = args.at(0).toInt();
                int y = args.at(1).toInt();

                //qDebug() << details << x  << y;
                if (x >= 0 && y >= 0 && x < height && y < width)  {

                    if (args.count() == 3) {
                        if (leftClick && args.at(2).indexOf("L") != -1)// args.at(2) == ""L"	" "
                            clickOnHeatMap(x,y);
                        else if (rightClick && args.at(2).indexOf("R") != -1)
                            clickOnHeatMap(x,y);
                    }
                }
            }
        }
    }
    //printHeatMap();

    renderVisualization();
}

void HeatMapVisualization::clickOnHeatMap(int x, int y) {

    for (int k = -marge; k <= marge && x + k < height; ++k)
        for (int l = -marge; l <= marge && y+l < width; ++l)
            if(x + k > 0 && y + l > 0) {
                heatMap[x + k][y + l] += (marge + 1) - max(abs(k),abs(l));
            }



    if (maxClicks < heatMap[x][y])
        maxClicks = heatMap[x][y];
}

void HeatMapVisualization::renderVisualization() {
    QColor color;
    int h, v, s;

    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            if (heatMap[i][j] == 0) {
                h = 255;
                v = 0;
                s = 255;
            }
            else {
                s = 255;
                float value = float(heatMap[i][j])/float(maxClicks);

                v = value * 255.0;

                if (value > 0.333){
                    float inversDiff = 3/2, min = 1/3;
                    int color = 60;//==colorRange
                    //0 rood
                    //60 geel
                    h = color - (inversDiff * (value - min)) * color;

                    if (heatMap[i][j] > maxClicks) {
                        h = 0;
                        v = 255;//in deze omgeving is heel veel geklikt
                        s = 230;
                    }
                }
                else {
                    float inversDiff = 3/1, min = 0;
                    int color = 240;
                    int colorRange = 180;
                    //60 geel
                    //240 blauw
                    h = color - (inversDiff * (value - min)) * colorRange;
                }

                //qDebug() << "color " << h << v << s;
            }
            color.setHsv(h,v,s);
            color.setAlpha(127);
            image->setPixel(j, i, color.rgb());
        }
    }
    QImage scaledImage = image->scaled(QSize(screenWidth,screenHeight),Qt::KeepAspectRatio);
    heatMapLabel->setPixmap(QPixmap::fromImage(scaledImage));
}

void HeatMapVisualization::printHeatMap() {
    qDebug() << "heatMap:";
    for (int i = 0; i < height; ++i)
        for (int j = 0; j < width; ++j)
            if (heatMap[i][j] != 0)
                qDebug() << i << ", " << j << ": " << heatMap[i][j];
}

int HeatMapVisualization::max(int a, int b) {
    return (a>b)?a:b;
}

void HeatMapVisualization::clearHeatMap() {
    maxClicks = 0;

    for (int i = 0; i < height; ++i)
        for (int j = 0; j < width; ++j)
            heatMap[i][j] = 0;
}

void HeatMapVisualization::updateParameters(int state) {
    if (leftClickCheckBox->checkState() == Qt::Checked)
        leftClick = true;
    else
        leftClick = false;

    if (rightClickCheckBox->checkState() == Qt::Checked)
        rightClick = true;
    else
        rightClick = false;

    if (clickCheckBox->checkState() == Qt::Checked)
        click = true;
    else
        click = false;

    if (doubleClickCheckBox->checkState() == Qt::Checked)
        doubleClick = true;
    else
        doubleClick = false;

    update(NULL);
}

void HeatMapVisualization::updateMarge(int marge) {
    this->marge = marge;

    update(NULL);
}
