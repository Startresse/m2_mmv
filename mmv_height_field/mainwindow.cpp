#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_comboBox_activated(int index)
{
    ui->widget->get_world().get_mesh().update_render_type(index);
    if (index == ROAD)
    {
        findChild<GLDisplayWidget*>("widget")->activate_mouse_record();
    }
}

void MainWindow::on_doubleSpinBox_valueChanged(double arg1)
{
    ui->widget->get_world().get_mesh().update_render_power(arg1);
}

void MainWindow::on_comboBox_2_activated(int index)
{
   ui->widget->get_world().get_mesh().update_file(index);
}

void MainWindow::on_spinBox_valueChanged(int arg1)
{
   ui->widget->get_world().get_mesh().update_blur_strength(arg1);
}

void MainWindow::on_comboBox_3_activated(int index)
{
    ui->widget->get_world().get_mesh().update_blur_type(index);
}

void MainWindow::on_doubleSpinBox_2_valueChanged(double arg1)
{
   ui->widget->get_world().get_mesh().update_blend(arg1);
}
