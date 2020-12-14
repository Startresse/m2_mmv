#include "heightfield.h"


/* CONSTRUCTORS */

HeightField::HeightField(const cv::Mat& im, const Box2& b, double low, double high)
    : SF2(Grid2(b, im.rows, im.cols)), low(low), high(high)
{
    for (int i = 0; i < nx; ++i) {
        for (int j = 0; j < ny; ++j) {
            at(i, j) = (high - low)*im.at<uchar>(i, j)/img_max_value + low;
            assert(at(i, j) >= low && at(i, j) <= high);
        }
    }
}

HeightField::HeightField(const HeightField& hf) : SF2(static_cast<SF2>(hf))
{
    low = hf.low;
    high = hf.high;
}


/* PROPERTIES */

double HeightField::height(int i, int j) const
{
    return at(i, j);
}

double HeightField::slope(int i, int j) const
{
    return Gradient(i, j).length();
}


double HeightField::average_slope(int i, int j) const
{
    float avg_slp = 0.0;
    int count = 0;
    for (int ii = -1; ii <= 1; ++ii) {
        for (int jj = -1; jj <= 1; ++jj) {
            if (!inside(i + ii, j + jj) || (ii == 0 && jj == 0))
                continue;

            count++;
            avg_slp += abs(slope(i + ii, j + jj));
        }
    }

    avg_slp /= count;
    return avg_slp;
}


QVector3D HeightField::vertex(int i, int j) const
{
    QVector2D xy = Grid2::vertex(i, j);
    return QVector3D(xy.x(), xy.y(), height(i, j));
}

QVector3D HeightField::normal(int i, int j) const
{
    QVector2D grad = Gradient(i, j);
    return QVector3D(-grad.x(), -grad.y(), 1.0).normalized();
}


/* MAPS */

SF2 HeightField::slope_map() const
{
#ifdef DISPLAY_TIME
    auto cpu_start = std::chrono::high_resolution_clock::now();
#endif

    SF2 sm(Grid2(*this));

    for (int i = 0; i < nx; ++i) {
        for (int j = 0; j < ny; ++j) {
            sm.at(i, j) = slope(i, j);
        }
    }


#ifdef DISPLAY_TIME
    auto cpu_stop = std::chrono::high_resolution_clock::now();
    int cpu_time = std::chrono::duration_cast<std::chrono::milliseconds>(cpu_stop - cpu_start).count();
    std::cout << "slope :" << int(cpu_time % 1000) << "\n";
#endif

    return sm;
}

SF2 HeightField::avg_slope_map() const
{
#ifdef DISPLAY_TIME
    auto cpu_start = std::chrono::high_resolution_clock::now();
#endif

    SF2 sm(Grid2(*this));

    for (int i = 0; i < nx; ++i) {
        for (int j = 0; j < ny; ++j) {
            sm.at(i, j) = average_slope(i, j);
        }
    }

#ifdef DISPLAY_TIME
    auto cpu_stop = std::chrono::high_resolution_clock::now();
    int cpu_time = std::chrono::duration_cast<std::chrono::milliseconds>(cpu_stop - cpu_start).count();
    std::cout << "avg slope :" << int(cpu_time % 1000) << "\n";
#endif

    return sm;
}

SF2 HeightField::laplacian_map() const
{
#ifdef DISPLAY_TIME
    auto cpu_start = std::chrono::high_resolution_clock::now();
#endif

    SF2 lm(Grid2(*this));

    for (int i = 0; i < nx; ++i) {
        for (int j = 0; j < ny; ++j) {
            lm.at(i, j) = laplacian(i, j);
        }
    }

#ifdef DISPLAY_TIME
    auto cpu_stop = std::chrono::high_resolution_clock::now();
    int cpu_time = std::chrono::duration_cast<std::chrono::milliseconds>(cpu_stop - cpu_start).count();
    std::cout << "laplacian :" << int(cpu_time % 1000) << "\n";
#endif

    return lm;
}

SF2 HeightField::stream_power() const
{
#ifdef DISPLAY_TIME
    auto cpu_start = std::chrono::high_resolution_clock::now();
#endif

    SF2 stream = stream_area();
    SF2 slope = slope_map();
    SF2 res(Grid2(*this));
    for (int i = 0; i < nx; ++i) {
        for (int j = 0; j < ny; ++j) {
            res.at(i, j) = sqrt(stream.at(i, j)) * slope.at(i, j);
        }
    }

#ifdef DISPLAY_TIME
    auto cpu_stop = std::chrono::high_resolution_clock::now();
    int cpu_time = std::chrono::duration_cast<std::chrono::milliseconds>(cpu_stop - cpu_start).count();
    std::cout << "stream power :" << int(cpu_time % 1000) << "\n";
#endif

    return res;
}

SF2 HeightField::wetness_index() const
{
#ifdef DISPLAY_TIME
    auto cpu_start = std::chrono::high_resolution_clock::now();
#endif

    SF2 stream = stream_area();
    SF2 slope = slope_map();
    SF2 res(Grid2(*this));
    for (int i = 0; i < nx; ++i) {
        for (int j = 0; j < ny; ++j) {
            res.at(i, j) = log(stream.at(i, j)) / (slope.at(i, j) + 0.000001);
        }
    }
    int mean = std::accumulate(field.begin(), field.end(), 0.0) / field.size();
    res.clamp(0.0, mean/10.0f);

#ifdef DISPLAY_TIME
    auto cpu_stop = std::chrono::high_resolution_clock::now();
    int cpu_time = std::chrono::duration_cast<std::chrono::milliseconds>(cpu_stop - cpu_start).count();
    std::cout << "wetness :" << int(cpu_time % 1000) << "\n";
#endif

    return res;
}

/* STREAM AREA */

const double eps = 0.00001;

std::vector<ScalarPoint2> HeightField::get_scalar_points() const
{
    std::vector<ScalarPoint2> points;
    points.reserve(nx*ny);
    for (int i = 0; i < nx; ++i) {
        for (int j = 0; j < ny; ++j) {
            points.push_back(ScalarPoint2(i, j, height(i, j)));
        }
    }
    assert(int(points.size()) == nx*ny);
    return points;

}

int HeightField::check_flow_slope(const QPoint& p, FlowTiles* ft) const
{
    static const double inv_sqrt_2 = 1.0/sqrt(2.0);
    int n = 0;
    double s[8];

    int i = p.x();
    int j = p.y();
    double zp = height(i, j);

    double slopesum = 0.0;
    for (int ii = -1; ii <= 1; ++ii) {
        for (int jj = -1; jj <= 1; ++jj) {
            int k = i + ii;
            int l = j + jj;
            if (!inside(k, l) || (ii == 0 && jj == 0))
                continue;

            bool diag = ii && jj;
            double step = height(k, l) - zp;
            if (step < -eps) {
                ft->q[n] = QPoint(k, l);
                assert(ft->q[n].x() != i || ft->q[n].y() != j);

                s[n] = -step;
                if(diag)
                    s[n] *= inv_sqrt_2;

                slopesum += s[n];

                ++n;
            }

        }
    }
    assert (n < 9);

    for (int i = 0; i < n; ++i) {
        ft->sn[i] = s[i] / slopesum;
        assert(ft->sn[i] <= 1.0);
    }

    return n;
}

SF2 HeightField::stream_area() const
{
#ifdef DISPLAY_TIME
    auto cpu_start = std::chrono::high_resolution_clock::now();
#endif

    std::vector<ScalarPoint2> points = get_scalar_points();
    std::sort(points.begin(), points.end());
    for (uint i = 0; i < points.size() - 1; ++i) {
        assert(points[i].z() <= points[i + 1].z());
    }

    SF2 sa = SF2(*this, 1.0);

    for (int i = points.size() - 1; i >= 0; --i)
    {
        const QPoint& p = points[i].point();
        const double sp = sa.at(p);

        FlowTiles ft;
        int n = check_flow_slope(p, &ft);

        if (n == 0)
            continue;

        double checksum = 0.0;
        for (int j = 0; j < n; ++j) {
            checksum += ft.sn[j];
            sa.at(ft.q[j]) += sp*ft.sn[j];
        }
        assert(abs(checksum - 1.0) < eps);
    }


#ifdef DISPLAY_TIME
    auto cpu_stop = std::chrono::high_resolution_clock::now();
    int cpu_time = std::chrono::duration_cast<std::chrono::milliseconds>(cpu_stop - cpu_start).count();
    std::cout << "stream area :" << int(cpu_time % 1000) << "\n";
#endif


    return sa;

}


/* EXPORT */

QImage HeightField::render(double contrast) const
{
    QImage image(nx, ny, QImage::Format_ARGB32);
    const QVector3D lightdir = QVector3D(2.0, 1.0, 3.0).normalized();

    for (int i = 0; i < nx; ++i) {
        for (int j = 0; j < ny; ++j) {
            QVector3D n = normal(i, j);
            double d = QVector3D::dotProduct(n, lightdir);
            d = (1.0 + d) / 2.0;

            assert(d >= 0.0 && d <= 1.0);
            d = pow(d, contrast);

            d *= 255;
            image.setPixel(i, j, qRgb(d, d, d));
        }
    }

    return image;
}




/* SHORTEST PATH */


double length(const QPoint& a, const QPoint& b)
{
    double x = a.x() - b.x();
    double y = a.y() - b.y();
    return sqrt(x*x + y*y);
}

double HeightField::weight(const QPoint& a, const QPoint& b)
{
//    if (a.x() == a.y() && b.x() == b.y()) return 500;
    QVector2D aa = static_cast<Grid2>(*this).vertex(a.x(), a.y());
    QVector2D bb = static_cast<Grid2>(*this).vertex(b.x(), b.y());
    double l = (aa - bb).length();
    return l;
}

std::vector<QPoint> neig = {
    QPoint( 0,  1),
    QPoint( 0, -1),
    QPoint( 1,  0),
    QPoint(-1,  0),
    QPoint(-1, -1),
    QPoint(-1,  1),
    QPoint( 1, -1),
    QPoint( 1,  1),
};
void HeightField::build_adjacency_list()
{
    adjacency_list.clear();
    adjacency_list.resize(nx*ny);
    for (int i = 0; i < nx; ++i) {
        for (int j = 0; j < ny; ++j) {
            for (const QPoint& n : neig) {
                int ii = i + n.x();
                int jj = j + n.y();
                adjacency_list[index(i, j)].push_back(neighbor(index(ii, jj), weight(QPoint(i, j), QPoint(ii, jj))));
            }
        }
    }
    list_built = true;
}

void DijkstraComputePaths(vertex_t source,
                          const adjacency_list_t &adjacency_list,
                          std::vector<weight_t> &min_distance,
                          std::vector<vertex_t> &previous)
{
    int n = adjacency_list.size();
    min_distance.clear();
    min_distance.resize(n, max_weight);
    min_distance[source] = 0;
    previous.clear();
    previous.resize(n, -1);
    std::set<std::pair<weight_t, vertex_t>> vertex_queue;
    vertex_queue.insert(std::make_pair(min_distance[source], source));

    while (!vertex_queue.empty())
    {
        weight_t dist = vertex_queue.begin()->first;
        vertex_t u = vertex_queue.begin()->second;
        vertex_queue.erase(vertex_queue.begin());

        // Visit each edge exiting u
        const std::vector<neighbor> &neighbors = adjacency_list[u];
        for (std::vector<neighbor>::const_iterator neighbor_iter = neighbors.begin();
             neighbor_iter != neighbors.end();
             neighbor_iter++)
        {
            vertex_t v = neighbor_iter->target;
            weight_t weight = neighbor_iter->weight;
            weight_t distance_through_u = dist + weight;
            if (distance_through_u < min_distance[v])
            {
                vertex_queue.erase(std::make_pair(min_distance[v], v));

                min_distance[v] = distance_through_u;
                previous[v] = u;
                vertex_queue.insert(std::make_pair(min_distance[v], v));
            }
        }
    }
}

std::list<vertex_t> DijkstraGetShortestPathTo(
    vertex_t vertex, const std::vector<vertex_t> &previous)
{
    std::list<vertex_t> path;
    for (; vertex != -1; vertex = previous[vertex])
        path.push_front(vertex);
    return path;
}

std::ostream& operator<<(std::ostream& os, const QPoint& p) {
    os << "(" << p.x() << ", " << p.y() << ")";
    return os;
}


std::list<vertex_t> HeightField::shortest_path(const QPoint& a, const QPoint& b)
{
    if (!list_built)
        build_adjacency_list();

    std::vector<weight_t> min_distance;
    std::vector<vertex_t> previous;

    int source = index(a.x(), a.y());
    int dest   = index(b.x(), b.y());

    DijkstraComputePaths(source, adjacency_list, min_distance, previous);
    std::cout << "Distance from " << a << " to " << b << " : " << min_distance[dest] << std::endl;
    std::list<vertex_t> path = DijkstraGetShortestPathTo(dest, previous);
    std::cout << "Path : ";
//    std::copy(path.begin(), path.end(), std::ostream_iterator<vertex_t>(std::cout, " "));
    for (vertex_t i : path) {
        std::cout << invIndex(i) << " ";
    }
    std::cout << std::endl;
    return path;
}



