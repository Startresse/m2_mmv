#include "heightfield.h"

HeightField::HeightField()
{

}

double HeightField::Height(int i, int j) const
{
    return at(i, j);
}

double HeightField::Slope(int i, int j) const
{
    QVector2D g = Gradient(i, j);
    return g.length();
}


//double HeightField::AverageSlope(int i, int j) const
//{
//    // moyenne des 8 slope directionnelles (en abs)
//}


QVector3D HeightField::p_3d(int i, int j) const
{
    QVector3D ret;
    QVector2D xy = Grid2::vertex(i, j);
    ret.setX(xy.x());
    ret.setY(xy.y());
    ret.setZ(Height(i, j));
    return ret;
}

QVector3D HeightField::n_3d(int i, int j) const
{
    QVector2D grad = Gradient(i, j);
    QVector3D g = QVector3D(-grad.x(), -grad.y(), 1.0);
    return g.normalized();
}


QImage HeightField::Export() const
{
    QImage image(nx, ny, QImage::Format_ARGB32);
    // Relief shading : Vec3 LightDir

//    QVector3D lightdir;
//    for (int i = 0; i < nx; ++i) {
//        for (int j = 0; j < ny; ++j) {
//            // Phong
//            double val = 0;
//            QVector3D n = n_3d(i, j);
//            double d ;//= dot(n, lightdir);
//            d = (1.0 + d) / 2.0;
//            d *= d; 	//joli shading
//            image.setPixel(i, j, val);
//        }
//    }

    return image;
}


SF2 HeightField::StreamArea() const
{
    SF2 sa = SF2(static_cast<Grid2>(*this));

    struct PointCoord {
        QPoint coord;
        double height;
    };

    std::vector<PointCoord> points;
    for (int i = 0; i < 10; ++i) {
        PointCoord pc;
        pc.coord = QPoint(i, 0);
        pc.height = 2*(double(rand())/RAND_MAX - 0.5);
        points.push_back(pc);
    }

    struct {
        bool operator()(PointCoord a, PointCoord b) {
            return a.height > b.height;
        }
    } z_more;

    std::sort(points.begin(), points.end(), z_more); // re-test this

//    for (const PointCoord& p : points)
//    {

//    }


    return sa;

}





