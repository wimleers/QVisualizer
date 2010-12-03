#ifndef BARTREEVISUALIZATION_H
#define BARTREEVISUALIZATION_H

#include <QWebView>
#include <QWebFrame>
#include <QResource>
#include <QDebug>

class BarTreeVisualization : public QWebView {
    Q_OBJECT
public:
    explicit BarTreeVisualization(QWidget *parent = 0);

signals:

public slots:

};

#endif // BARTREEVISUALIZATION_H
