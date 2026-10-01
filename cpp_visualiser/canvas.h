#ifndef CANVAS_H
#define CANVAS_H

#include <QWidget>
#include <vector>
#include <Route.h>
#include <optional>

class Canvas : public QWidget
{
    Q_OBJECT

public:
    Canvas(QWidget *parent = nullptr);
    void setGrid(const GRID & grid);
    void setPath(const std::vector<Point> & path);
    void setTargets(const std::vector<Point> & targets);
    void setStart(Point start_pos);
    ~Canvas();

protected:
    void paintEvent(QPaintEvent * event) override;

private:
    std::optional<GRID> grid_;
    std::vector<Point> path_;
    std::vector<Point> targets_;
    Point start_pos_{0, 0};
    bool get_grid_ = false;

    double cell_size_ = 2.0;

};
#endif // CANVAS_H
