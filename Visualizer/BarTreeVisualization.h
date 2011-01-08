#ifndef BARTREEVISUALIZATION_H
#define BARTREEVISUALIZATION_H

#include <QWebView>
#include <QWebFrame>
#include <QResource>
#include <QxtJSON>
#include <QVariant>
#include <QVariantList>
#include <QVariantMap>
#include <QMap>
#include <QMutex>
#include <QTimerEvent>
#include <QDebug>
#include "Event.h"

#define BARTREEVISUALIZATION_DATA_REFRESH_INTERVAL 100
#define BARTREEVISUALIZATION_COLOR_KEYBOARD "#428496"
#define BARTREEVISUALIZATION_COLOR_MOUSE "#505DF2"

class BarTreeVisualization : public QWebView {
    Q_OBJECT
public:
    explicit BarTreeVisualization(QWidget *parent = 0);

signals:

public slots:
    void loadHasFinished(bool ok);
    void eventSequenceChanged(QVector<Event *> * events);
    void highlightEventLocation(int msecs, const QString & eventType);

protected:
    void timerEvent(QTimerEvent *);
    void sendEventSequenceToWebView();
    static double calcTotalFreq(const QMap<QString, int> & frequencies);

    QWebFrame * mainFrame;
    QVector<Event *> * currentEventSequence;
    QVector<Event *> * nextEventSequence;
    QMutex mutex;
    bool pageHasLoaded;
};

#endif // BARTREEVISUALIZATION_H
