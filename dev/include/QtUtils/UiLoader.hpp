#ifndef UILOADER
#define UILOADER

#include <QUiLoader>
#include <QWidget>
#include <CustomWidget/QAppWidgetSFML.hpp>
#include <CustomWidget/QListLayerWidget.hpp>
#include <CustomWidget/QChartWidget.hpp>

class UiLoader : public QUiLoader {
    Q_OBJECT

    public:
        UiLoader(QObject * parent = nullptr) : QUiLoader(parent) {}

        QWidget * createWidget(const QString & className, QWidget * parent = 0,
                            const QString & name = QString()) {
            if (className == "QAppWidgetSFML") {
                QAppWidgetSFML* w = new QAppWidgetSFML(parent);
                w->setObjectName(name);
                return w;
            } else if (className == "QListLayerWidget") {
                QListLayerWidget* w = new QListLayerWidget(parent);
                w->setObjectName(name);
                return w;
            } else if (className == "QChartWidget") {
                QChartWidget* w = new QChartWidget(parent);
                w->setObjectName(name);
                return w;
            }
            return QUiLoader::createWidget(className, parent, name);
        }
};

#endif