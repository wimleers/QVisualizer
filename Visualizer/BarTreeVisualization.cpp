#include "BarTreeVisualization.h"

BarTreeVisualization::BarTreeVisualization(QWidget *parent) : QWebView(parent) {
    this->load(QUrl("qrc:/BarTree/BarTree.html"));
    this->show();

    //this->page()->mainFrame()->evaluateJavaScript("alert('boohoo');");
}
