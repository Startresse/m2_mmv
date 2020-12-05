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

void MainWindow::on_toggle_wireframe_stateChanged(int arg1)
{
//   ui->widget->get_world().get_mesh().set_wireframe(arg1);
}

void MainWindow::on_select_mesh_activated(const QString &arg1)
{
//    ui->widget->get_world().get_mesh().load(arg1);
}

void MainWindow::on_select_color_activated(const QString &arg1)
{
//   ui->widget->get_world().get_mesh().set_color_mode(arg1);
}

void MainWindow::on_toggle_voronoi_toggled(bool checked)
{
//    ui->widget->get_world().get_mesh().set_voronoi(checked);
}
