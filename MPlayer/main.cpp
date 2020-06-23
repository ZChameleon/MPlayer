#include <QApplication>
#include "MPlayer.h"


int main(int argc, char* argv[]) {

    QApplication a{argc, argv};

    QFont f;
    f.setFamily("Microsoft YaHei");
    f.setPointSize(10);
    QApplication::setFont(f);

    MPlayer w;
    w.show();

    return QApplication::exec();
}
