#ifndef PLOTTERWID_H
#define PLOTTERWID_H
#include "plotter.h"
#include <QPainter>
#include <QPaintEvent>
#include <QPaintDevice>
#include <QContextMenuEvent>
#include <QMenu>

namespace _VectorScan
{
    // Виджет, для рисования графика
    class PlotterWid : public IPlotterWid
    {
        Q_OBJECT
    public:
        explicit PlotterWid(QWidget *parent = nullptr);

        // IPlotterWid interface
    public slots:
        virtual void show(const QByteArray &, QColor color) override;

        virtual void redraw(const QByteArray &, QColor color) override;

        virtual void removePlot(QColor color) override;

        virtual void setParam(const Param &param) override;

        virtual void getParam_sl() override;
    signals:

    public slots:
        virtual void paintEvent(QPaintEvent* event) override;

    private:
        Param param; // Парам парам :D Параметры

        QByteArrayList dataList; // Данные для графика. Хранить их надо для пересчета координат при ресайзе
        QList<Plot>  plotList; // Графики

        // Показывает, отображается ли на экране виджет
        // Нужен, чтобы не отображать новое окно при рисовании нового графика
        bool isShow = false;

        bool isPercent = true;  // Отображение процентов по оси Y

        QMenu* cMenu;

        QAction* actHideAll, *actShowAll;

        void drawGrid(QPainter& painter);

        // Данные в plot
        void drawGraph(QPainter& painter);

        // Пересчитывает данные для графика в точки и сохраняет в plot
        // i - индекс графика.
        void recalc(QByteArray const& data, int i);

        // Пересчитывает все графики
        void recalcAll();

    private slots:
        void triggeredContext(QAction* act);

        // QWidget interface
    protected:
        virtual void resizeEvent(QResizeEvent *event) override;

        virtual void closeEvent(QCloseEvent *event) override;

        virtual void changeEvent(QEvent * event) override;

        virtual QImage plotImage() override;

        virtual void contextMenuEvent(QContextMenuEvent *event) override;

    };


}
#endif // PLOTTERWID_H
