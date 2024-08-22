#ifndef QCHARTW_HPP
#define QCHARTW_HPP

#include <QWidget>
#include <QtCharts>
#include <NeuralNetwork/NeuralNetwork.hpp>

class QChartWidget : public QChartView {
    Q_OBJECT

    public:
        QChartWidget(QWidget* Parent, const QPoint& Position= QPoint(0,0), const QSize& Size= QSize(0,0));
        ~QChartWidget();
        void showLossResults(NeuralNetwork*);
        void resetChart(void);

    private slots:
        void slot_resetChart(void);

    private:
        QChart* m_chartWidget;
};

#endif