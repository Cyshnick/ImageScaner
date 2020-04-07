#include "calcline.h"
#include "math.h"

using namespace _VectorScan;

// --------------------------------- CaclLine ------------------------------------
CalcLine::CalcLine(QObject *parent) : IPlotter(parent)
{
    plot = FactoryPlotterWid::create(nullptr);

    windOpt = new Options(nullptr);

    connect(this, SIGNAL(show(QByteArray const&, QColor)),
            plot, SLOT(show(QByteArray const&, QColor)) );

    connect(this, SIGNAL(redraw(QByteArray const&, QColor)),
            plot, SLOT(redraw(QByteArray const&, QColor)) );

    connect(plot, SIGNAL(showWidget(QWidget*)),
            this, SIGNAL(showWidget(QWidget*)) );

    connect(plot, SIGNAL(closed()),
            this, SIGNAL(closed()) );

    connect(windOpt, SIGNAL(setCalcParam(ParamCalc const&)),
            this, SLOT(setParam(ParamCalc const&)) );

    connect(windOpt, SIGNAL(setPlotParam(Param const&)),
            plot, SLOT(setParam(Param const&)) );

    connect(this, SIGNAL(getParamPlot_sg()),
            plot, SLOT(getParam_sl()) );

    connect(plot, SIGNAL(getParam_sg(Param const&)),
            this, SLOT(getParamPlot_sl(Param const&)) );

    param.step = 1;

    windOpt->setBeginCalcParam(param);
    emit getParamPlot_sg();
}

// --------------------------------- ~CalcLine ------------------------------------
CalcLine::~CalcLine()
{
    if(plot)
        delete plot;
    if(!windOpt)
        delete windOpt;
}

// --------------------------------- PostProcessing ------------------------------------
QByteArray CalcLine::PostProcessing(QByteArray &&data)
{
    return std::move(data);
}

// --------------------------------- BresenhemAlg ------------------------------------
QByteArray CalcLine::BresenhamAlg()
{
    QByteArray res;
    int st = param.step;
    int x0 = static_cast<int>(vector.begin.x());
    int y0 = static_cast<int>(vector.begin.y());
    int x1 = static_cast<int>(vector.end.x());
    int y1 = static_cast<int>(vector.end.y());

    int X = x1 - x0;
    int Y = y1 - y0;

    if((X == 0) && (Y == 0))
        return QByteArray();

    int x = 0, y = 0;
    float tg = static_cast<float>(Y) / X;
    float f = 0.f;
    int dx = (X > 0) ? 1 : -1;
    int dy = (Y > 0) ? 1 : -1;
    if(abs(Y) < abs(X))
    {
        do
        {
            x += dx;
            f = x*tg;
            if(fabs(f - y) > 0.5f)
            {
                y += dy;
            }
            if(st == param.step)
            {
                res.push_back(img.data.at((x + x0)*img.size.height() + (y + y0)));
                st = 0;
            }
            st++;
        }while((x != X) || (y != Y));
    }
    else
    {
        do
        {
            y+=dy;
            f = y/tg;
            if(fabs(f - x) > 0.5f)
            {
                x+=dx;
            }
            if(st == param.step)
            {
                res.push_back(img.data.at((x + x0)*img.size.height() + (y + y0)));
                st = 0;
            }
            st++;
        }while((x != X) || (y != Y));
    }
    return res;
}

// --------------------------------- vectorPainted ------------------------------------
void CalcLine::vectorPainted(const Vector &vect)
{
    vector = vect;
    emit show(BresenhamAlg(), vector.color);
}

// --------------------------------- vectorChanged ------------------------------------
void CalcLine::vectorChanged(const Vector &vect)
{
    vector = vect;
    emit redraw(PostProcessing(BresenhamAlg()), vector.color);
}

// --------------------------------- openedByteImage ------------------------------------
void CalcLine::openedByteImage(const ByteImage &image, QString const& filename)
{
    Q_UNUSED(image)
    plot->setWindowTitle("График "+filename);
    windOpt->setWindowTitle("Параметры " + filename);
    img = image;
}

// --------------------------------- vectorRemove ------------------------------------
void CalcLine::vectorRemove(QColor color)
{
    plot->removePlot(color);
}

// --------------------------------- plotImage ------------------------------------
QImage CalcLine::plotImage()
{
    return plot->plotImage();
}

// --------------------------------- openWindowOption ------------------------------------
void CalcLine::openWindowOption()
{
    emit showWidget(windOpt);
}

// --------------------------------- getParamPlot_sl ------------------------------------
void CalcLine::getParamPlot_sl(const Param &param)
{
    windOpt->setBeginPlotParam(param);
}

// --------------------------------- setParam ------------------------------------
void CalcLine::setParam(const ParamCalc &param)
{
    this->param = param;
    vectorChanged(this->vector);
}

// --------------------------- FabricPlotter --------------------------------------------
IPlotter* FactoryPlotter::create(QObject *parent)
{
    return new CalcLine(parent);
}
