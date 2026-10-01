#include "canvas.h"
#include <QPainter>
#include <algorithm>


Canvas::Canvas(QWidget *parent)
    : QWidget(parent)
{
    setMinimumSize(2000, 1000);
    setWindowTitle("Theta * visualiser");
}

void Canvas::setGrid(const GRID &grid){
    grid_ = grid;
    get_grid_ = true;

    cell_size_ = 2.0;
    //double Sx = double(width())  / grid_->x_size;
    //double Sy = double(height()) / grid_->y_size;
    //cell_size_ = std::min(Sx, Sy);

    update();
}

void Canvas::setPath(const std::vector<Point> &path){
    path_ = path;
    update();
}

void Canvas::setTargets(const std::vector<Point> &targets){
    targets_ = targets;
    update();
}

void Canvas::setStart(Point start_pos){
    start_pos_ = start_pos;
    update();
}

void Canvas::paintEvent(QPaintEvent *event){
    QPainter p(this);
        p.fillRect(rect(), Qt::white);

        if (!get_grid_) return;
        p.translate(0, grid_->y_size * cell_size_);
        p.scale(1, -1);
        p.setPen(Qt::NoPen);
        p.setBrush(QColor(100, 100, 100));
        for (int x = 0; x < grid_->x_size; ++x) {
            for (int y = 0; y < grid_->y_size; ++y) {
                if (grid_->field[grid_->index(x, y)] == 1) {
                    p.drawRect(QRectF(x * cell_size_, y * cell_size_, cell_size_, cell_size_));
                }
            }
        }

        p.setPen(QPen(QColor(100, 100, 100), 0.5));
        for (int x = 0; x <= grid_->x_size; ++x) {
            p.drawLine(QPointF(x * cell_size_, 0),
                       QPointF(x * cell_size_, grid_->y_size * cell_size_));
        }
        for (int y = 0; y <= grid_->y_size; ++y) {
            p.drawLine(QPointF(0, y * cell_size_),
                       QPointF(grid_->x_size * cell_size_, y * cell_size_));
        }

        if (path_.size() >= 2) {
            p.setPen(QPen(Qt::blue, 2));
            for (size_t i = 0; i + 1 < path_.size(); ++i) {
                QPointF a((path_[i].x     + 0.5) * cell_size_, (path_[i].y     + 0.5) * cell_size_);
                QPointF b((path_[i + 1].x + 0.5) * cell_size_, (path_[i + 1].y + 0.5) * cell_size_);
                p.drawLine(a, b);
            }

            p.setPen(Qt::NoPen);
            p.setBrush(Qt::blue);
            for (const auto & pt : path_) {
                p.drawEllipse(QPointF((pt.x + 0.5) * cell_size_, (pt.y + 0.5) * cell_size_), cell_size_ * 0.3, cell_size_ * 0.3);
            }
        }

        p.setPen(Qt::NoPen);
        p.setBrush(Qt::green);
        p.drawEllipse(QPointF((start_pos_.x + 0.5) * cell_size_, (start_pos_.y + 0.5) * cell_size_), cell_size_ * 0.6, cell_size_ * 0.6);

        for (size_t i = 0; i < targets_.size(); ++i) {
            const Point & t = targets_[i];

            int shade = std::max(80, 255 - static_cast<int>(i) * 40);
            p.setBrush(QColor(255, shade, shade));

            p.drawEllipse(QPointF((t.x + 0.5) * cell_size_, (t.y + 0.5) * cell_size_), cell_size_ * 0.6, cell_size_ * 0.6);

            p.setPen(Qt::white);
            QFont f = p.font();
            f.setPointSizeF(std::max(6.0, cell_size_ * 0.8));
            f.setBold(true);
            p.setFont(f);
            p.drawText(QRectF((t.x + 0.5) * cell_size_ - cell_size_ * 0.6, (t.y + 0.5) * cell_size_ - cell_size_ * 0.6, cell_size_ * 1.2, cell_size_ * 1.2), Qt::AlignCenter, QString::number(i + 1));
            p.setPen(Qt::NoPen);
        }
    }


Canvas::~Canvas()
{
}

