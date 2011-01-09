#include "BarTreeVisualization.h"

BarTreeVisualization::BarTreeVisualization(QWidget *parent) : QWebView(parent) {
    this->mainFrame = this->page()->mainFrame();
    connect(this, SIGNAL(loadFinished(bool)), SLOT(loadHasFinished(bool)));

    // See explanation at BarTreeVisualization::timerEvent().
    startTimer(BARTREEVISUALIZATION_DATA_REFRESH_INTERVAL);

    // Load the visualization in the web view.
    this->load(QUrl("qrc:/BarTree/BarTree.html"));
}

void BarTreeVisualization::loadHasFinished(bool ok) {
    this->pageHasLoaded = ok;
}

void BarTreeVisualization::eventSequenceChanged(QVector<Event *> * events) {
    this->mutex.lock();
    this->nextEventSequence = events;
    this->mutex.unlock();
}

void BarTreeVisualization::highlightEventLocation(int msecs, const QString & eventType) {
    qDebug() << eventType;
}

/**
 * Periodically (BARTREEVISUALIZATION_DATA_REFRESH_INTERVAL intervals) poll
 * to see if there's a new event sequence to draw. We do this using polling
 * to prevent continuous redrawing. The number of redrawings per second is
 * now effectively limited to 1/BARTREEVISUALIZATION_DATA_REFRESH_INTERVAL.
 */
void BarTreeVisualization::timerEvent(QTimerEvent *) {
    // If the page hasn't been loaded yet, then definitely don't send an
    // event sequence to the web view yet!
    if (!this->pageHasLoaded)
        return;

    // Make the next event sequence the current, if any, and send it to the
    // web view.
    if (this->nextEventSequence != NULL) {
        this->mutex.lock();
        this->currentEventSequence = this->nextEventSequence;
        this->nextEventSequence = NULL;
        this->mutex.unlock();
        this->sendEventSequenceToWebView();
    }
}

/**
 * Convert the current event sequence into the data format the JavaScript
 * code expects, but then as QVariants. QxtJSON::stringify() will then
 * encode this data as JSON, which the JavaScript code can use directly.
 */
void BarTreeVisualization::sendEventSequenceToWebView() {
    QVariantMap data;
    QMap<QString, int> inputTypeFrequency;
    QMap<QString, QMap<QString, int> > eventTypeFrequency;
    QMap<QString, QMap<QString, QMap<QString, int> > > modifierFrequency;
    QString inputType, eventType, modifier, details;
    QStringList detailsList;
    Event * event;

    // Count frequencies.
    foreach (event, *this->currentEventSequence) {
        inputType = event->getInputType();
        eventType = event->getEventType();
        details = event->getDetails();

        // Ignore MouseButtonRelease events.
        if (eventType.compare(QString("MouseButtonRelease")) == 0)
            continue;

        // Ignore MouseMove events.
        if (eventType.compare(QString("MouseMove")) == 0)
            continue;

        if (eventType.contains("Mouse"))
            inputType = QString("Mouse");
        else
            inputType = QString("Keyboard");

        // Clean up event type for optimized display.
        eventType.replace("Key", "");
        eventType.replace("Mouse", "");
        // Keyboard.
        eventType.replace("Release", "Press");
        // Mouse.
        eventType.replace("ButtonPress", "Click");
        eventType.replace("ButtonDblClick", "Double");

        // Extract nice & clean "modifier" from event details.
        modifier = (inputType == "Mouse") ? "none" : "something";
        detailsList = details.split(';');
        if (inputType.compare("Mouse") == 0) {
            modifier = detailsList[2];
            if (modifier.length() == 0)
                modifier = "none";
        }
        else {
            // Clean the labels, so humans can actually understand them!
            int key = detailsList[0].toInt();
            // ASCII labels.
            if (key >= 48 && key <= 122) {
                detailsList[1] = (char) key;
            }
            // Arrows.
            else if (key >= 16777234) {
                switch (key) {
                case 16777234:
                    detailsList[1] = QString("arrow left");
                    break;
                case 16777235:
                    detailsList[1] = QString("arrow up");
                    break;
                case 16777236:
                    detailsList[1] = QString("arrow right");
                    break;
                case 16777237:
                    detailsList[1] = QString("arrow down");
                    break;
                default:
                    break;
                }
            }

            detailsList[2].replace(":", "");
            bool modifierKeyPressed = (detailsList[2].length() > 0);
            bool regularKeyPressed = (detailsList[1].length() > 0);
            if (modifierKeyPressed && regularKeyPressed) {
                modifier = detailsList[2] + "+" + detailsList[1];
            }
            else if (modifierKeyPressed && !regularKeyPressed) {
                modifier = detailsList[2];
            }
            else if (!modifierKeyPressed && regularKeyPressed) {
                modifier = detailsList[1];
            }
            else {
                modifier = "other";
            }
        }

        // Init inputTypeFrequency (1st level frequency counts).
        if (!inputTypeFrequency.contains(inputType)) {
            inputTypeFrequency.insert(inputType, 0);
        }

        // Init eventTypeFrequency (2nd level frequency counts).
        if (!eventTypeFrequency.contains(inputType)) {
            QMap<QString, int> map;
            eventTypeFrequency.insert(inputType, map);
        }
        if (!eventTypeFrequency[inputType].contains(eventType)) {
            eventTypeFrequency[inputType].insert(eventType, 0);
        }

        // Init modifierFrequency (3rd level frequency counts).
        if (!modifierFrequency.contains(inputType)) {
            QMap<QString, QMap<QString, int> > map;
            modifierFrequency.insert(inputType, map);
        }
        if (!modifierFrequency[inputType].contains(eventType)) {
            QMap<QString, int> map;
            modifierFrequency[inputType].insert(eventType, map);
        }
        if (!modifierFrequency[inputType][eventType].contains(modifier)) {
            modifierFrequency[inputType][eventType].insert(modifier, 0);
        }

        // Incrementalization.
        inputTypeFrequency.insert(inputType, inputTypeFrequency.value(inputType) + 1);
        eventTypeFrequency[inputType].insert(eventType, eventTypeFrequency[inputType][eventType] + 1);
        modifierFrequency[inputType][eventType].insert(modifier, modifierFrequency[inputType][eventType][modifier] + 1);
    }

    // Build QVariantMap that represents the data in the format expected by the
    // JavaScript code.
    QString keyL1, keyL2, keyL3;
    QVariantMap level1, level2, level3, tmp;
    double totalL1, totalL2, totalL3;

    totalL1 = BarTreeVisualization::calcTotalFreq(inputTypeFrequency);
    level1.clear();
    foreach (keyL1, inputTypeFrequency.keys()) {

        totalL2 = BarTreeVisualization::calcTotalFreq(eventTypeFrequency[keyL1]);
        level2.clear();
        foreach (keyL2, eventTypeFrequency[keyL1].keys()) {

            totalL3 = BarTreeVisualization::calcTotalFreq(modifierFrequency[keyL1][keyL2]);
            level3.clear();
            foreach(keyL3, modifierFrequency[keyL1][keyL2].keys()) {
                tmp.clear();
                tmp.insert("count",     modifierFrequency[keyL1][keyL2][keyL3]);
                tmp.insert("frequency", modifierFrequency[keyL1][keyL2][keyL3] / totalL3);
                level3.insert(keyL3, tmp);
            }

            tmp.clear();
            tmp.insert("count",     eventTypeFrequency[keyL1][keyL2]);
            tmp.insert("frequency", eventTypeFrequency[keyL1][keyL2] / totalL2);
            tmp.insert("children", level3);
            level2.insert(keyL2, tmp);
        }

        tmp.clear();
        tmp.insert("count",     inputTypeFrequency[keyL1]);
        tmp.insert("frequency", inputTypeFrequency[keyL1] / totalL1);
        tmp.insert("children", level2);

        // Assign color depending on input type.
        tmp.insert("color", (keyL1 == "Mouse") ? BARTREEVISUALIZATION_COLOR_MOUSE : BARTREEVISUALIZATION_COLOR_KEYBOARD);

        level1.insert(keyL1, tmp);
    }
    data = level1;

    QString json = QxtJSON::stringify(data);
    qDebug() << "Generated JSON:" << json;
    this->mainFrame->evaluateJavaScript(QString("barTreeView.loadData(%1);").arg(json));
}

/**
 * Helper method to calculate the total frequency of a level.
 */
double BarTreeVisualization::calcTotalFreq(const QMap<QString, int> & frequencies) {
    static QString key;
    static double totalFrequency;

    totalFrequency = 0;
    foreach(key, frequencies.keys()) {
        totalFrequency += frequencies[key];
    }

    return totalFrequency;
}
