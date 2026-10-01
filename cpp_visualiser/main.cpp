#include <QApplication>

#include "canvas.h"       // ← виджет
#include "scenario.h"     // ← данные

int main(int argc, char ** argv)
{
    QApplication app(argc, argv);

    Scenario s = run_scenario();

    if (s.path.empty()) {
        qWarning("Path not found — not showing visualization");
        return 1;
    }

    Canvas canvas;
    canvas.setGrid(s.grid);
    canvas.setStart(s.start);
    canvas.setTargets(s.targets);
    canvas.setPath(s.path);
    canvas.resize(1000, 700);
    canvas.show();

    return app.exec();
}
