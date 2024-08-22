#include <CustomWidget/QChartWidget.hpp>
#include <QtUtils/UiLoader.hpp>

QChartWidget::QChartWidget(QWidget* Parent, const QPoint& Position, const QSize& Size) : QChartView(Parent) {
    move(Position);
    resize(Size);

    m_chartWidget = new QChart();
    m_chartWidget->setTitle("Training results");
    m_chartWidget->createDefaultAxes();

    QtCharts::QValueAxis *axisX = new QtCharts::QValueAxis();
    axisX->setRange(0, 1);
    QtCharts::QValueAxis *axisY = new QtCharts::QValueAxis();
    axisY->setRange(0, 1);
    m_chartWidget->addAxis(axisX, Qt::AlignBottom);
    m_chartWidget->addAxis(axisY, Qt::AlignLeft);
    m_chartWidget->axes(Qt::Horizontal).back()->setTitleText("Epochs");
    m_chartWidget->axes(Qt::Vertical).back()->setTitleText("Loss");

    setChart(m_chartWidget);
    setRenderHint(QPainter::Antialiasing);
}

QChartWidget::~QChartWidget() { delete m_chartWidget; }

void QChartWidget::showLossResults(NeuralNetwork* nn) {
    resetChart();
    auto epochEvo = nn->getEpochResult();
    auto lossEvo = nn->getLossResult();
    auto accuEvo = nn->getAccuResult();

    for(int i=0; i<lossEvo.row(); i++) {
        QLineSeries* lineSeries = new QLineSeries();
        lineSeries->setName(("loss output "+ std::to_string(i)).c_str());
        for(int j=0; j<lossEvo.col(); j++) lineSeries->append(QPointF(epochEvo.at(j), lossEvo.getCoeff(i,j)));
        m_chartWidget->addSeries(lineSeries);
    }

    QLineSeries* lineSeries = new QLineSeries();
    lineSeries->setName("accuracy");
    for(int i=0; i<accuEvo.size(); i++) lineSeries->append(QPointF(epochEvo.at(i), accuEvo.at(i)));
    m_chartWidget->addSeries(lineSeries);
    m_chartWidget->createDefaultAxes();
    m_chartWidget->axes(Qt::Horizontal).back()->setTitleText("Epochs");
    m_chartWidget->axes(Qt::Vertical).back()->setTitleText("Loss");
}

void QChartWidget::resetChart(void) { 
    m_chartWidget->removeAllSeries();
    m_chartWidget->axes(Qt::Horizontal).back()->setRange(0,1);
    m_chartWidget->axes(Qt::Vertical).back()->setRange(0,1);
    m_chartWidget->axes(Qt::Horizontal).back()->setTitleText("Epochs");
    m_chartWidget->axes(Qt::Vertical).back()->setTitleText("Loss");
}

void QChartWidget::slot_resetChart(void) { resetChart(); }