#include "svgpreviewview.h"

#include <QWheelEvent>

namespace geometrize
{

namespace dialog
{

class SvgPreviewView::SvgPreviewViewImpl
{
public:
    SvgPreviewViewImpl(SvgPreviewView* pQ) : q{pQ}
    {
        q->setMouseTracking(true);
    }

    SvgPreviewViewImpl operator=(const SvgPreviewViewImpl&) = delete;
    SvgPreviewViewImpl(const SvgPreviewViewImpl&) = delete;
    ~SvgPreviewViewImpl() = default;

private:
    SvgPreviewView* q;
};

SvgPreviewView::SvgPreviewView(QWidget* parent) : QGraphicsView(parent), d{std::make_unique<SvgPreviewView::SvgPreviewViewImpl>(this)}
{
}

SvgPreviewView::~SvgPreviewView()
{
}

void SvgPreviewView::wheelEvent(QWheelEvent* e)
{
    if (Qt::ControlModifier != (e->modifiers() & Qt::ControlModifier)) {
        QGraphicsView::wheelEvent(e);
        return;
    }

    if (e->angleDelta().x() == 0) {
        const QPoint pos{e->pos()};
        const QPointF posf{mapToScene(pos)};

        const double angle{static_cast<double>(e->angleDelta().y())};
        double by{0.0};
        if(angle > 0) {
            by = 1 + (angle / 360 * 0.1);
        } else if (angle < 0) {
            by = 1 - (-angle / 360 * 0.1);
        } else {
            by = 1;
        }

        scale(by, by);

        const int w{viewport()->width()};
        const int h{viewport()->height()};
        const double wf{mapToScene(QPoint(w - 1, 0)).x() -mapToScene(QPoint(0,0)).x()};
        const double hf{mapToScene(QPoint(0, h - 1)).y() - mapToScene(QPoint(0,0)).y()};
        const double lf{posf.x() - pos.x() * wf / w};
        const double tf{posf.y() - pos.y() * hf / h};

        ensureVisible(lf, tf, wf, hf, 0, 0);
        const QPointF newPos{mapToScene(pos)};
        ensureVisible(QRectF(QPointF(lf, tf) - newPos + posf, QSizeF(wf, hf)), 0, 0);

        e->accept();
    }
}

void SvgPreviewView::mouseMoveEvent(QMouseEvent* event)
{
    QGraphicsView::mouseMoveEvent(event);
}

}

}