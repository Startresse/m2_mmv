#include "mesh.h"

void Mesh::load()
{
    cv::Mat image = cv::imread(image_name.toStdString(), cv::IMREAD_GRAYSCALE);
    hf = HeightField(image, Box2(length_x, length_y), low, high);

#ifdef DISPLAY_TIME
    auto cpu_start = std::chrono::high_resolution_clock::now();
    std::cout << "New mesh " << image.cols << "x" << image.rows << std::endl;
#endif


    // REINIT
    shading_done = false;



    blur();
    render();
    set_up();

    hf.shortest_path(QPoint(0, 0), QPoint(1, 1));
    exit(1);


#ifdef DISPLAY_TIME
    auto cpu_stop = std::chrono::high_resolution_clock::now();
    int cpu_time = std::chrono::duration_cast<std::chrono::milliseconds>(cpu_stop - cpu_start).count();
    std::cout << "new mesh :" << int(cpu_time % 1000) << "\n";
#endif
}

constexpr double eps_bottom = -0.001;
void Mesh::set_up()
{
    faces.clear();

    float length_x = hf.size_x()  / 4;
    float length_y = hf.highest() / 2;
    float length_z = hf.size_y()  / 4;

    if (closed) {
        // BOTTOM
        faces.emplace_back(QVector3D(-2, eps_bottom, -2),
                           QVector3D(-2, eps_bottom,  2),
                           QVector3D( 2, eps_bottom,  2));

        faces.emplace_back(QVector3D(-2, eps_bottom, -2),
                           QVector3D( 2, eps_bottom, -2),
                           QVector3D( 2, eps_bottom,  2));

        // FRONT & BACK
        for (int j : {0, hf.size_y() - 1}) {
            for (int i = 0; i < hf.size_x() - 1; ++i) {
                faces.emplace_back( PointIJ(QVector3D((i    )/length_x - 2, hf.height(i    , j) / length_y, -j/length_z + 2), i, j),
                                            QVector3D((i    )/length_x - 2, eps_bottom                    , -j/length_z + 2)       ,
                                            QVector3D((i + 1)/length_x - 2, eps_bottom                    , -j/length_z + 2)        );

                faces.emplace_back( PointIJ(QVector3D((i    )/length_x - 2, hf.height(i    , j) / length_y, -j/length_z + 2), i    , j),
                                    PointIJ(QVector3D((i + 1)/length_x - 2, hf.height(i + 1, j) / length_y, -j/length_z + 2), i + 1, j),
                                            QVector3D((i + 1)/length_x - 2, eps_bottom                    , -j/length_z + 2)        );
            }
        }

        // SIDES
        for (int j : {0, hf.size_x() - 1}) {
            for (int i = 0; i < hf.size_y() - 1; ++i) {
                faces.emplace_back( PointIJ(QVector3D(j/length_x - 2, hf.height(j, i    ) / length_y, -(i    )/length_z + 2), j, i),
                                            QVector3D(j/length_x - 2, eps_bottom                    , -(i    )/length_z + 2),
                                            QVector3D(j/length_x - 2, eps_bottom                    , -(i + 1)/length_z + 2));

                faces.emplace_back( PointIJ(QVector3D(j/length_x - 2, hf.height(j, i    ) / length_y, -(i    )/length_z + 2), j, i    ),
                                    PointIJ(QVector3D(j/length_x - 2, hf.height(j, i + 1) / length_y, -(i + 1)/length_z + 2), j, i + 1),
                                            QVector3D(j/length_x - 2, eps_bottom                    , -(i + 1)/length_z + 2)           );
            }
        }
    }

    // HF
    for (int i = 0; i < hf.size_x() - 1; ++i) {
        for (int j = 0; j < hf.size_y() - 1; ++j) {
            faces.emplace_back( PointIJ(QVector3D((i    )/length_x - 2, hf.height(i    , j    ) / length_y, -(j    )/length_z + 2), i    , j    ),
                                PointIJ(QVector3D((i    )/length_x - 2, hf.height(i    , j + 1) / length_y, -(j + 1)/length_z + 2), i    , j + 1),
                                PointIJ(QVector3D((i + 1)/length_x - 2, hf.height(i + 1, j + 1) / length_y, -(j + 1)/length_z + 2), i + 1, j + 1));

            faces.emplace_back( PointIJ(QVector3D((i    )/length_x - 2, hf.height(i    , j    ) / length_y, -(j    )/length_z + 2), i    , j    ),
                                PointIJ(QVector3D((i + 1)/length_x - 2, hf.height(i + 1, j    ) / length_y, -(j    )/length_z + 2), i + 1, j    ),
                                PointIJ(QVector3D((i + 1)/length_x - 2, hf.height(i + 1, j + 1) / length_y, -(j + 1)/length_z + 2), i + 1, j + 1));
        }
    }

}


void Mesh::blur()
{
    if (blur_no_median)
        hf.blur(blur_strength);
    else
        hf.median(blur_strength);
}

void Mesh::update_blur_type(bool b)
{
    blur_no_median = b;
    load();
}

void Mesh::update_blur_strength(int b)
{
    blur_strength = b;
    load();
}

void Mesh::update_render_type(int type)
{
    render_t = render_type(type);
    render();
}

void Mesh::update_render_power(double pow)
{
    render_power = pow;
    render();
}

void Mesh::render()
{
    if (!shading_done) {
        shading_done = true;
        shading = hf.render(2.0);
    }

    SF2 render_tmp;
    switch (render_t) {
    case RENDER:
        render_img = hf.render(render_power);
        return;
    case STREAM_AREA:
        render_tmp = hf.stream_area();
        break;
    case LAPLACIAN:
        render_tmp = hf.laplacian_map();
        break;
    case SLOPE:
        render_tmp = hf.slope_map();
        break;
    case AVG_SLOPE:
        render_tmp = hf.avg_slope_map();
        break;
    case STREAM_POWER:
        render_tmp = hf.stream_power();
        break;
    case WETNESS:
        render_tmp = hf.wetness_index();
        break;
    default:
        std::cout << "Wrong enum" << std::endl;
        break;
    }

    render_tex = render_tmp.save(render_power);
    render_img = render_tex;
    update_tex_blend();
}

void Mesh::update_blend(double t)
{
    blend_fact = t;
    update_tex_blend();
}

void Mesh::update_tex_blend()
{
    for (int i = 0; i < render_tex.width(); ++i) {
        for (int j = 0; j < render_tex.height(); ++j) {
            QRgb color = render_tex.pixel(i, j);
            QRgb shade = shading.pixel(i, j);

            double red = qRed(color) + qRed(shade) * blend_fact ;
            double green = qGreen(color) + qGreen(shade) * blend_fact;
            double blue = qBlue(color) + qBlue(shade) * blend_fact ;

            red = std::min(img_max_value, red);
            green = std::min(img_max_value, green);
            blue = std::min(img_max_value, blue);

            render_img.setPixel(i, j, qRgb(red, green, blue));
        }
    }
}

void Mesh::update_file(int file_id)
{
    switch(images(file_id)) {
    case SPYKES:
        image_name = "images/heightmap3.jpeg";
        length_x = 2000;
        length_y = 2000;
        low = 0;
        high = 1200;
        break;
    case CHAM:
        image_name = "images/cham.png";
        // UNKNOWN
        length_x = 2000;
        length_y = 2000;
        low = 0;
        high = 1200;
        break;
    case MB:
        image_name = "images/mb_height.png";
        //UNKNOWN
        length_x = 2000;
        length_y = 2000;
        low = 0;
        high = 1200;
        break;
    case VESUVE:
        image_name = "images/vesuve.png";
        length_x = 15000;
        length_y = 15000;
        low = 0;
        high = 1200;
        break;
    case VOLC:
        image_name = "images/volc.png";
        // UNKNOWN
        length_x = 2000;
        length_y = 2000;
        low = 0;
        high = 1200;
        break;
    }

    load();
}


inline
void glPoint(const QVector3D& v)
{
    glVertex3d(v.x(), v.y(), v.z());
}

inline
void Mesh::glTriangle(const Triangles& t)
{
    for (int i = 0; i < 3; ++i) {
        const PointIJ& v = t.vertices[i];
        if (hf.inside(v.i, v.j)) {
            QRgb color = render_img.pixel(v.i, v.j);
            double red = qRed(color) / img_max_value;
            double green = qGreen(color) / img_max_value;
            double blue = qBlue(color) / img_max_value;

//            QRgb shade = shading.pixel(v.i, v.j);
//            float t = 0.1;
//            red = (red *(1 - t)+ t*qRed(shade)/img_max_value);
//            blue = (blue * (1 - t)+ t*qBlue(shade)/img_max_value);
//            green = (green * (1 - t)+ t*qGreen(shade)/img_max_value);

            glColor3d(red, green, blue);
        } else {
            glColor3d(0, 0, 0);
        }
        glPoint(v);
    }
}

void Mesh::draw() {
    srand(0);
    glColor3d(.8, .8, .8);

    glBegin(GL_TRIANGLES);
    for (const Triangles& t : faces)
        glTriangle(t);
    glEnd();
}
