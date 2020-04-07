#ifndef CALCLINE_H
#define CALCLINE_H

#include <QObject>
#include "plotter.h"
#include "options.h"

namespace _VectorScan
{

    class CalcLine : public IPlotter
    {
        Q_OBJECT
    public:
        explicit CalcLine(QObject *parent = nullptr);

        virtual ~CalcLine() override;


    signals:

        // Отображает виджет
        void show(QByteArray const&, QColor);

        // Перерисовывает график с новыми данными
        void redraw(QByteArray const&, QColor);
        void getParamPlot_sg();
    private:

        QByteArray PostProcessing(QByteArray &&data);

        // Вовращает массив интерсивностей на векторе
        QByteArray BresenhamAlg();

    private:
        QPointer<IPlotterWid> plot;

        ByteImage img;

        Vector vector;

        ParamCalc param;

        QPointer<Options> windOpt;

        // IPlotter interface
    public slots:
        virtual void vectorPainted(const Vector &vect) override;

        virtual void vectorChanged(const Vector &vect) override;

        virtual void openedByteImage(const ByteImage &image,
                                     QString const& filename) override;

        virtual void vectorRemove(QColor color) override;

        virtual QImage plotImage() override;

        virtual void openWindowOption() override;

    private slots:

        virtual void getParamPlot_sl(Param const& param);

        void setParam(ParamCalc const& param);
    };
}

#endif // CALCLINE_H
