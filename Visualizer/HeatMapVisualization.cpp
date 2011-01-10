#include "HeatMapVisualization.h"

HeatMapVisualization::HeatMapVisualization(Database *database) : QWidget() {

    lastEvents = NULL;
    height = database->getResolution().height();
    width = database->getResolution().width();

    heatMap = new int*[height];
    for (int i = 0; i < height; ++i)
        heatMap[i] = new int[width];

    clickHeatMap = new int*[height];
    for (int i = 0; i < height; ++i)
        clickHeatMap[i] = new int[width];

    mouseRoute = new QVector<QPoint>();

    clearHeatMap();

    marge = 20;
    mouseRouteInterval = 1;
    clickDeviation = 10;

    showLeftClicks = showRightClicks = showSingleClicks = showDoubleClicks = showMouseMoveRoute = true;
    showMouseMove = false;

    screenWidth = (QApplication::desktop()->width() >= 1920) ? 1050: 470;
    screenWidth = (screenWidth > width) ? width : screenWidth;
    screenHeight = (QApplication::desktop()->width() > 800) ? 500 : 300;
    screenHeight = (screenHeight > height) ? height : screenHeight;

    heatMapImage = new QImage(width, height, QImage::Format_ARGB32_Premultiplied);

    scene = new HeatMapVisQGraphicsScene();
    view = new QGraphicsView(scene);
    view->setRenderHints(QPainter::Antialiasing);
    scene->setSceneRect(0, 0, screenWidth, screenHeight);
    connect(scene, SIGNAL(clicked(QPointF)), SLOT(pixelSelected(QPointF)));

    mainLayout = new QVBoxLayout();
    mainLayout->addWidget(view, 0, Qt::AlignCenter);

    createCheckBoxes();

    determineAvailableBackgroundImages(database->getMaxEventTime());
    renderVisualization();

    setLayout(mainLayout);
}

void HeatMapVisualization::update(QVector<Event*> *events = NULL) {
    clearHeatMap();

    if(events != NULL)
        lastEvents = events;
    else if(lastEvents != NULL)
        events = lastEvents;
    else
        return;

    lastEventTime = events->isEmpty() ? 0 : events->last()->getTime();

    scene->removeItem(scene->numClicksLabel);
    scene->numClicksLabel = NULL;

    int counter = 0, mouseRouteX = 0, mouseRouteY = 0;
    for(int i = 0; i < events->count(); ++i) {
        if((showSingleClicks && events->at(i)->getEventType().compare("MouseButtonPress") == 0) ||
           (showDoubleClicks && events->at(i)->getEventType().compare("MouseButtonDblClick") == 0) ||
           ((showMouseMove || showMouseMoveRoute) && events->at(i)->getEventType().compare("MouseMove") == 0)) {
            // enkel muiskliks (enkel en dubbel)

            QString details = events->at(i)->getDetails();
            details.remove("\"");

            QStringList args = details.split(";");
            if (args.count() >= 2) {
                int x = args.at(0).toInt();
                int y = args.at(1).toInt();

                //qDebug() << details << x  << y;
                if (x >= 0 && y >= 0 && x < height && y < width)  {
                    if (showMouseMove && events->at(i)->getEventType().compare("MouseMove") == 0)
                        clickOnHeatMap(x,y);
                    if ((showSingleClicks || showDoubleClicks) && args.count() == 3 && events->at(i)->getEventType().compare("MouseMove") != 0) {
                        // controleren: links en rechts filteren indien nodig (checkBoxes)
                        if (showLeftClicks && args.at(2).indexOf("L") != -1)// args.at(2) == ""L"	" "
                            clickOnHeatMap(x,y);
                        else if (showRightClicks && args.at(2).indexOf("R") != -1)
                            clickOnHeatMap(x,y);
                        clickHeatMap[x][y]++;
                    }
                    if (showMouseMoveRoute && events->at(i)->getEventType().compare("MouseMove") == 0) { //muisbewegingen tekenen => gemiddeld punt berekenen per mouseRouteInterval bewegingen
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
    int h, v, s, a;

    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            if (heatMap[i][j] == 0) {// er is niet geklikt
                h = 255;
                v = 0;
                s = 255;
                a = 0;
                color.setHsv(h,v,s,a);
                heatMapImage->setPixel(j, i, color.rgba());
            }
            else {// geklikt
                s = 255;
                float value = float(heatMap[i][j])/float(maxClicks);// verhouding: is op deze plaats veel geklikt

                v = value * 255.0;

                a = v * 1/30;

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
                color.setHsv(h,v,s,a);
                heatMapImage->setPixel(j, i, color.rgba());
            }
        }
    }

    //muisbewigingen tekenen
    QPainter painter;
    painter.begin(heatMapImage);
    for(int i = 0; i + 1 < mouseRoute->size(); ++i){
        QLineF line(mouseRoute->at(i), mouseRoute->at(i+1));

        painter.drawLine(line);
    }
    painter.end();

    scene->setBackgroundImage(determineBackgroundImage(lastEventTime));
    scene->setForegroundImage(heatMapImage);
    scene->invalidate();
}

void HeatMapVisualization::highlightEventLocation(int msec) {
    update();
    for (int i = 0; i < lastEvents->size(); ++i)
        if (lastEvents->at(i)->getTime() == msec) {
            QString details = lastEvents->at(i)->getDetails();
            details.remove("\"");

            QStringList args = details.split(";");
            if (args.count() >= 2) {
                QPoint p = QPoint(args.at(1).toInt(), args.at(0).toInt());

                QPainter painter;
                painter.begin(heatMapImage);
                painter.setBrush(QBrush(QColor(255,0,255)));
                painter.drawEllipse(p, 5, 5);

                painter.end();
            }
        }
    scene->setBackgroundImage(&(determineBackgroundImage(lastEventTime)->scaled(QSize(screenWidth,screenHeight), Qt::KeepAspectRatio)));
    scene->setForegroundImage(heatMapImage);
    scene->invalidate();
}

void HeatMapVisualization::pixelSelected(QPointF p) {
    int numClicks = 0;
    for (int i = -clickDeviation; i <= clickDeviation && p.y() + i < height; ++i)
        for (int j = -clickDeviation; j <= clickDeviation && p.x() + j < width; ++j)
            if(p.x() + i > 0 && p.y() + j > 0)
                numClicks += clickHeatMap[int(p.y()) + i][int(p.x()) + j];

    if(scene->numClicksLabel == NULL)
        scene->numClicksLabel = scene->addText("");

    scene->numClicksLabel->setHtml("<html><body bgcolor=\"white\"><p style=\"color: #000000;\">" + QString::number(numClicks) + "</p></body></html>");
    scene->numClicksLabel->setPos(p);

    scene->invalidate();
}

int HeatMapVisualization::max(int a, int b) {
    return (a>b)?a:b;
}

void HeatMapVisualization::clearHeatMap() {
    maxClicks = 0;

    for (int i = 0; i < height; ++i)
        for (int j = 0; j < width; ++j)
            heatMap[i][j] = 0;

    for (int i = 0; i < height; ++i)
        for (int j = 0; j < width; ++j)
            clickHeatMap[i][j] = 0;

    mouseRoute->clear();
}

void HeatMapVisualization::updateParameters(int state) {
    Q_UNUSED(state);

    showLeftClicks = showLeftClicksCheckBox->isChecked();
    showRightClicks = showRightClicksCheckBox->isChecked();
    showSingleClicks = showSingleClicksCheckBox->isChecked();
    showDoubleClicks = showDoubleClicksCheckBox->isChecked();
    showMouseMove = showMouseMoveCheckBox->isChecked();
    showMouseMoveRoute = showMouseMoveRouteCheckBox->isChecked();

    showLeftClicksCheckBox->setEnabled(true);
    showRightClicksCheckBox->setEnabled(true);
    showSingleClicksCheckBox->setEnabled(true);
    showDoubleClicksCheckBox->setEnabled(true);
    showMouseMoveCheckBox->setEnabled(true);
    showMouseMoveRouteCheckBox->setEnabled(true);

    if(!showSingleClicks && !showDoubleClicks) {
        showLeftClicksCheckBox->setEnabled(false);
        showRightClicksCheckBox->setEnabled(false);
    }

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

void HeatMapVisualization::createCheckBoxes() {
    QGroupBox *box1 = new QGroupBox();
    QVBoxLayout *vbox1 = new QVBoxLayout();
    showLeftClicksCheckBox = new QCheckBox("Show left-clicks");
    showRightClicksCheckBox = new QCheckBox("Show right-clicks");
    vbox1->addWidget(showLeftClicksCheckBox);
    vbox1->addWidget(showRightClicksCheckBox);
    box1->setLayout(vbox1);

    QGroupBox *box2 = new QGroupBox();
    QVBoxLayout *vbox2 = new QVBoxLayout();
    showSingleClicksCheckBox = new QCheckBox("Show single-clicks");
    showDoubleClicksCheckBox = new QCheckBox("Show double-clicks");
    vbox2->addWidget(showSingleClicksCheckBox);
    vbox2->addWidget(showDoubleClicksCheckBox);
    box2->setLayout(vbox2);

    QGroupBox *box3 = new QGroupBox();
    QVBoxLayout *vbox3 = new QVBoxLayout();
    showMouseMoveRouteCheckBox = new QCheckBox("Show mouse route");
    showMouseMoveCheckBox = new QCheckBox("Show mouse moves");
    vbox3->addWidget(showMouseMoveRouteCheckBox);
    vbox3->addWidget(showMouseMoveCheckBox);
    box3->setLayout(vbox3);

    showLeftClicksCheckBox->setChecked(showLeftClicks);
    showRightClicksCheckBox->setChecked(showRightClicks);
    showSingleClicksCheckBox->setChecked(showSingleClicks);
    showDoubleClicksCheckBox->setChecked(showDoubleClicks);
    showMouseMoveCheckBox->setChecked(showMouseMove);
    showMouseMoveRouteCheckBox->setChecked(showMouseMoveRoute);

    connect(showLeftClicksCheckBox, SIGNAL(stateChanged(int)), SLOT(updateParameters(int)));
    connect(showRightClicksCheckBox, SIGNAL(stateChanged(int)), SLOT(updateParameters(int)));
    connect(showSingleClicksCheckBox, SIGNAL(stateChanged(int)), SLOT(updateParameters(int)));
    connect(showDoubleClicksCheckBox, SIGNAL(stateChanged(int)), SLOT(updateParameters(int)));
    connect(showMouseMoveCheckBox, SIGNAL(stateChanged(int)), SLOT(updateParameters(int)));
    connect(showMouseMoveRouteCheckBox, SIGNAL(stateChanged(int)), SLOT(updateParameters(int)));

    QLabel *margeLabel = new QLabel("Click radius: ");
    margeSpinBox = new QSpinBox();
    margeSpinBox->setRange(1, 100);
    margeSpinBox->setValue(marge);
    connect(margeSpinBox, SIGNAL(valueChanged(int)), SLOT(updateMarge(int)));

    QLabel *mouseRouteIntervalLabel = new QLabel("Mouse move accuracy: ");
    mouseRouteIntervalSpinBox = new QSpinBox();
    mouseRouteIntervalSpinBox->setRange(1, 100);
    mouseRouteIntervalSpinBox->setValue(mouseRouteInterval);
    connect(mouseRouteIntervalSpinBox, SIGNAL(valueChanged(int)), SLOT(updateMouseRouteInterval(int)));

    QGroupBox *box4 = new QGroupBox();
    QVBoxLayout *vbox4 = new QVBoxLayout();
    QHBoxLayout *hbox4_1 = new QHBoxLayout();
    hbox4_1->addWidget(margeLabel);
    hbox4_1->addWidget(margeSpinBox);
    QHBoxLayout *hbox4_2 = new QHBoxLayout();
    hbox4_2->addWidget(mouseRouteIntervalLabel);
    hbox4_2->addWidget(mouseRouteIntervalSpinBox);
    vbox4->addLayout(hbox4_1);
    vbox4->addLayout(hbox4_2);
    box4->setLayout(vbox4);

    QHBoxLayout *inputLayout = new QHBoxLayout();
    inputLayout->addWidget(box1);
    inputLayout->addWidget(box2);
    inputLayout->addWidget(box3);
    inputLayout->addWidget(box4);

    mainLayout->addLayout(inputLayout);
}

void HeatMapVisualization::determineAvailableBackgroundImages(int msecs) {
    availableBackgroundImages = new QHash<int, bool>();
    QString filename;
    foreach(filename, QDir("./screenshots").entryList())
        if(filename.contains(".png"))
            availableBackgroundImages->insert((filename.split('.')[0]).toInt(), true);
}

QImage* HeatMapVisualization::determineBackgroundImage(int msecs) {
    while(msecs > 0) {
        if(availableBackgroundImages->contains(msecs))
            return new QImage("./screenshots/" + QString::number(msecs) + ".png");
        --msecs;
    }
    return new QImage();
}
