#include <CustomWidget/QChartWidget.hpp>
#include <QtUtils/UiLoader.hpp>

QChartWidget::QChartWidget(QWidget* Parent, const QPoint& Position, const QSize& Size) : QChartView(Parent) {
    move(Position);
    resize(Size);

    this->setMouseTracking(true);

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
    auto temp_axis_h = static_cast<const QValueAxis*>(m_chartWidget->axes(Qt::Horizontal).back());
    auto temp_axis_v = static_cast<const QValueAxis*>(m_chartWidget->axes(Qt::Vertical).back());
}

void QChartWidget::resetChart(void) { 
    m_chartWidget->removeAllSeries();
    m_chartWidget->axes(Qt::Horizontal).back()->setRange(0,1);
    m_chartWidget->axes(Qt::Vertical).back()->setRange(0,1);
    m_chartWidget->axes(Qt::Horizontal).back()->setTitleText("Epochs");
    m_chartWidget->axes(Qt::Vertical).back()->setTitleText("Loss");
}

void QChartWidget::slot_resetChart(void) { resetChart(); }

void QChartWidget::wheelEvent(QWheelEvent *e)
{
    QPoint numDegrees = e->angleDelta();
    if(numDegrees.y() > 0) {
        m_chartWidget->zoomIn();
    }
    else if(numDegrees.y() < 0) {
        m_chartWidget->zoomOut();
    }
}

void QChartWidget::mouseMoveEvent(QMouseEvent *ev) {
    if(m_dragDrop) {
        QPoint delta_mouse_pos = QCursor::pos() - m_lastMousePos;
        m_lastMousePos = QCursor::pos();
        auto temp_axis_h = static_cast<const QValueAxis*>(m_chartWidget->axes(Qt::Horizontal).back());
        auto temp_axis_v = static_cast<const QValueAxis*>(m_chartWidget->axes(Qt::Vertical).back());

        if(delta_mouse_pos.x()!=0) {
            qreal alpha = delta_mouse_pos.x()/m_chartWidget->geometry().width();
            qreal beta = (temp_axis_h->max() - temp_axis_h->min())*alpha;
            m_chartWidget->axes(Qt::Horizontal).back()->setRange(temp_axis_h->min()-beta, temp_axis_h->max()-beta);
        }

        if(delta_mouse_pos.y()!=0) {
            qreal alpha = delta_mouse_pos.y()/m_chartWidget->geometry().height();
            qreal beta = (temp_axis_v->max() - temp_axis_v->min())*alpha;
            m_chartWidget->axes(Qt::Vertical).back()->setRange(temp_axis_v->min()+beta, temp_axis_v->max()+beta);
        }
    }
}

void QChartWidget::mousePressEvent(QMouseEvent *ev) {
    m_dragDrop = true;
    m_lastMousePos = QCursor::pos();
    this->setCursor(QCursor(Qt::ClosedHandCursor));
}

void QChartWidget::mouseReleaseEvent(QMouseEvent *ev) { 
    m_dragDrop = false;
    this->setCursor(QCursor(Qt::OpenHandCursor));
}

void QChartWidget::keyPressEvent(QKeyEvent *event) {}