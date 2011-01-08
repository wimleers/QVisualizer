#include "HeatMapVisualization.h"

HeatMapVisualization::HeatMapVisualization(QSize resolution) : QWidget() {

    mainLayout = new QVBoxLayout();

    lastEvents = NULL;
    height = resolution.height();
    width = resolution.width();

    heatMap = new int*[height];
    for (int i = 0; i < height; ++i)
        heatMap[i] = new int[width];

    mouseRoute = new QVector<QPoint>();

    clearHeatMap();

    marge = 40;
    mouseRouteInterval = 1;

    leftClick = true;
    rightClick = true;
    mouseMoveRoute = true;
    click = true;
    doubleClick = true;
    mouseMove = true;

    screenWidth = 500;
    screenHeight = 300;

    image = new QImage(width, height, QImage::Format_RGB32);

    heatMapLabel = new QLabel();
    /*heatMapLabel->setMinimumWidth(screenWidth);
    heatMapLabel->setMaximumWidth(screenWidth);
    heatMapLabel->setMinimumHeight(screenHeight);
    heatMapLabel->setMaximumHeight(screenHeight);*/
    heatMapLabel->setPixmap(QPixmap::fromImage(*image));
    QPushButton *showImageButton = new QPushButton("Afbeelding in oorspronkelijke grootte");
    connect(showImageButton, SIGNAL(clicked()), this, SLOT(showImage()));

    mainLayout->addWidget(heatMapLabel, 0, Qt::AlignCenter);
    mainLayout->addWidget(showImageButton,0,Qt::AlignCenter);

    QHBoxLayout *checkButtonLayout = new QHBoxLayout();
    QGroupBox *box1 = new QGroupBox();
    QVBoxLayout *box1Layout = new QVBoxLayout();
    leftClickCheckBox = new QCheckBox("&Linker muisklik", this);
    rightClickCheckBox = new QCheckBox("&Rechter muisklik", this);
    mouseMoveRouteCheckBox = new QCheckBox("&Traject muisbewegingen");
    QGroupBox *box2 = new QGroupBox();
    QVBoxLayout *box2Layout = new QVBoxLayout();
    clickCheckBox = new QCheckBox("&Eén muisklik", this);
    doubleClickCheckBox = new QCheckBox("&Dubbel muisklik", this);
    mouseMoveCheckBox = new QCheckBox("&Muisbewegingen", this);

    leftClickCheckBox->setCheckState(Qt::Checked);
    rightClickCheckBox->setCheckState(Qt::Checked);
    mouseMoveRouteCheckBox->setCheckState(Qt::Checked);
    clickCheckBox->setCheckState(Qt::Checked);
    doubleClickCheckBox->setCheckState(Qt::Checked);
    mouseMoveCheckBox->setCheckState(Qt::Checked);

    connect(leftClickCheckBox, SIGNAL(stateChanged(int)), this, SLOT(updateParameters(int)));
    connect(rightClickCheckBox, SIGNAL(stateChanged(int)), this, SLOT(updateParameters(int)));
    connect(mouseMoveRouteCheckBox, SIGNAL(stateChanged(int)), this, SLOT(updateParameters(int)));
    connect(clickCheckBox, SIGNAL(stateChanged(int)), this, SLOT(updateParameters(int)));
    connect(doubleClickCheckBox, SIGNAL(stateChanged(int)), this, SLOT(updateParameters(int)));
    connect(mouseMoveCheckBox, SIGNAL(stateChanged(int)), this, SLOT(updateParameters(int)));

    box1Layout->addWidget(leftClickCheckBox);
    box1Layout->addWidget(rightClickCheckBox);
    box1Layout->addWidget(mouseMoveRouteCheckBox);
    box2Layout->addWidget(clickCheckBox);
    box2Layout->addWidget(doubleClickCheckBox);
    box2Layout->addWidget(mouseMoveCheckBox);

    box1->setLayout(box1Layout);
    box2->setLayout(box2Layout);

    checkButtonLayout->addWidget(box1);
    checkButtonLayout->addWidget(box2);

    QLabel *margeLabel = new QLabel("Marge rond elke klik:");
    margeSpinBox = new QSpinBox();
    margeSpinBox->setRange(1, 100);
    margeSpinBox->setValue(marge);
    connect(margeSpinBox, SIGNAL(valueChanged(int)), this, SLOT(updateMarge(int)));

    QLabel *mouseRouteIntervalLabel = new QLabel("Nauwkeurigheid van muisbewegingen");
    mouseRouteIntervalSpinBox = new QSpinBox();
    mouseRouteIntervalSpinBox->setRange(1, 100);
    mouseRouteIntervalSpinBox->setValue(mouseRouteInterval);
    connect(mouseRouteIntervalSpinBox, SIGNAL(valueChanged(int)), this, SLOT(updateMouseRouteInterval(int)));

    QGroupBox *box3 = new QGroupBox();
    QVBoxLayout *box3Layout = new QVBoxLayout();

    box3Layout->addWidget(margeLabel);
    box3Layout->addWidget(margeSpinBox);
    box3Layout->addWidget(mouseRouteIntervalLabel);
    box3Layout->addWidget(mouseRouteIntervalSpinBox);

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
    int counter = 0, mouseRouteX = 0, mouseRouteY = 0;
    for (int i = 0; i < events->count(); ++i) {
        if ((click && events->at(i)->getEventType().compare("MouseButtonPress") == 0) || (doubleClick && events->at(i)->getEventType().compare("MouseButtonDblClick") == 0) || ((mouseMove || mouseMoveRoute) && events->at(i)->getEventType().compare("MouseMove") == 0)) {
            // enkel muiskliks (enkel en dubbel)
            //qDebug() << "Type: " << events->at(i)->getEventType();

            QString details = events->at(i)->getDetails();
            details.remove("\"");//remove "

            QStringList args = details.split(";");
            if (args.count() >= 2) {
                int x = args.at(0).toInt();
                int y = args.at(1).toInt();

                //qDebug() << details << x  << y;
                if (x >= 0 && y >= 0 && x < height && y < width)  {
                    if (mouseMove && events->at(i)->getEventType().compare("MouseMove") == 0)
                        clickOnHeatMap(x,y);
                    if ((click || doubleClick) && args.count() == 3 && events->at(i)->getEventType().compare("MouseMove") != 0) {
                        // controleren: links en rechts filteren indien nodig (checkBoxes)
                        if (leftClick && args.at(2).indexOf("L") != -1)// args.at(2) == ""L"	" "
                            clickOnHeatMap(x,y);
                        else if (rightClick && args.at(2).indexOf("R") != -1)
                            clickOnHeatMap(x,y);
                    }
                    if (mouseMoveRoute && events->at(i)->getEventType().compare("MouseMove") == 0) { //muisbewegingen tekenen => gemiddeld punt berekenen per mouseRouteInterval bewegingen
                        if (counter < mouseRouteInterval) {
                            mouseRouteX += x;
                            mouseRouteY += y;
                            counter++;
                        }
                        if(counter == mouseRouteInterval) {
                            mouseRoute->append(QPoint(mouseRouteY/mouseRouteInterval, mouseRouteX/mouseRouteInterval));
                            counter = 0;
                            mouseRouteX = 0;
                            mouseRouteY = 0;
                        }
                    }
                }
            }
        }
    }

    renderVisualization();
}

void HeatMapVisualization::clickOnHeatMap(int x, int y) {
    // waar geklikt is, een hoge score toekennen, de marge errond score alsmaar verkleinen
    for (int k = -marge; k <= marge && x + k < height; ++k)
        for (int l = -marge; l <= marge && y+l < width; ++l)
            if(x + k > 0 && y + l > 0) {
                heatMap[x + k][y + l] += (marge + 1) - max(abs(k),abs(l));
            }

    if (maxClicks < heatMap[x][y])//hoogst voorkomend aantal kliks bijhouden
        maxClicks = heatMap[x][y];
}

void HeatMapVisualization::renderVisualization() {
    QColor color;
    int h, v, s;

    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            if (heatMap[i][j] == 0) {// er is niet geklikt
                h = 255;
                v = 0;
                s = 255;
            }
            else {// geklikt
                s = 255;
                float value = float(heatMap[i][j])/float(maxClicks);// verhouding: is op deze plaats veel geklikt

                v = value * 255.0;

                if (value > 0.333){//matig tot veel geklikt => de kleuren geel tot rood hiervoor gebruiken
                    float inversDiff = 3/2, min = 1/3;
                    int color = 60;//==colorRange
                    //0 rood
                    //60 geel
                    h = color - (inversDiff * (value - min)) * color;

                    if (heatMap[i][j] > maxClicks) {
                        h = 0;
                        v = 255;//in deze omgeving is heel veel geklikt
                        s = 230;// => extra benadrukken
                    }
                }
                else {// weinig geklikt (marge rond klik) => blauw tinten
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

    QPainter painter;
    painter.begin(image);
    for(int i = 0; i + 1 < mouseRoute->size(); ++i){
        QLineF line(mouseRoute->at(i), mouseRoute->at(i+1));

        painter.drawLine(line);
    }
    painter.end();

    QImage scaledImage = image->scaled(QSize(screenWidth,screenHeight),Qt::KeepAspectRatio);
    heatMapLabel->setPixmap(QPixmap::fromImage(scaledImage));

}

int HeatMapVisualization::max(int a, int b) {
    return (a>b)?a:b;
}

void HeatMapVisualization::clearHeatMap() {
    maxClicks = 0;

    for (int i = 0; i < height; ++i)
        for (int j = 0; j < width; ++j)
            heatMap[i][j] = 0;

    mouseRoute->clear();
}

void HeatMapVisualization::updateParameters(int state) {
    leftClick = leftClickCheckBox->isChecked();
    rightClick = rightClickCheckBox->isChecked();
    mouseMoveRoute = mouseMoveRouteCheckBox->isChecked();
    click = clickCheckBox->isChecked();
    doubleClick = doubleClickCheckBox->isChecked();
    mouseMove = mouseMoveCheckBox->isChecked();

    update();
}

void HeatMapVisualization::updateMarge(int marge) {
    this->marge = marge;

    update();
}

void HeatMapVisualization::updateMouseRouteInterval(int interval) {
    mouseRouteInterval = interval;

    update();
}

void HeatMapVisualization::showImage() {
    dialog = new QDialog();
    QVBoxLayout *layout = new QVBoxLayout();

    QLabel *imageLabel = new QLabel();
    imageLabel->setPixmap(QPixmap::fromImage(*image));
    QPushButton *okButton = new QPushButton("Ok");
    connect(okButton, SIGNAL(clicked()), this, SLOT(closeDialog()));

    layout->addWidget(imageLabel, 0, Qt::AlignCenter);
    layout->addWidget(okButton, 0, Qt::AlignCenter);

    dialog->setLayout(layout);

    dialog->show();
}

void HeatMapVisualization::closeDialog() {
    dialog->close();
    delete dialog;
}
