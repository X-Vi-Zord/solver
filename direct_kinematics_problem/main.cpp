#include <QApplication>
#include <SetDataWidget.h>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    SetDataWidget widget(nullptr);
    widget.show();

    return app.exec();
}
