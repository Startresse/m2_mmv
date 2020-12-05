#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_toggle_wireframe_stateChanged(int arg1);

    void on_select_mesh_activated(const QString &arg1);

    void on_select_color_activated(const QString &arg1);

    void on_toggle_voronoi_toggled(bool checked);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
