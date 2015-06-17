#ifndef WIDGET_H
#define WIDGET_H
//-----------------------------------------QT class incldues----------------------------------
# include <QWidget>
# include <QMessageBox>
# include <QDebug>
# include <QFileDialog>
# include <QTimer>
# include <QFileDialog>
# include <QFile>
# include <QTextStream>
# include <QGraphicsDropShadowEffect>
# include <qmath.h>
# include <exception>
//---------------------------------------------------------------------------------------------
# include <sqlite3.h>       //DB include

# include <malloc.h>
# include "bass.h"
# include "qcustomplot.h"

# define BUFSTEP    200000
# define SPECWIDTH  368     // display width (should be multiple of 4)
# define SPECHEIGHT 127     // height (changing requires palette adjustments too)

#ifdef QT_DEBUG
    #define QDEBUG(q) qDebug()<<q
#else
    #define QDEBUG(q) ;
#endif

struct RGB{
    qint8 rgbRed,rgbGreen,rgbBlue;
};

void Error (QString er);

BOOL CALLBACK RecordingCallback(HRECORD handle, const void *buffer, DWORD length, void *user);

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

    void startRecording                     ();

    void stopRecording                      ();

    void updateInputInfo                    ();

    void writeToDisk                        ();

    void KaiserWindow                       ();

    float I0                                (float);

signals:
    void PlotSpectrumSignal                 ();

    void PlotKaiserWindowSignal             ();

private slots:
    void timerProc                          ();

    void plotSpectrums                      ();

    void plotKaiserWindow                   ();

    void stopRecordingSlot                  ();

    void clearGraphs                        ();

    void on_pushButton_clicked              ();

    void on_pushButton_2_clicked            ();

    void on_horizontalSlider_valueChanged   (qint32 value);

    void on_pushButton_3_clicked            ();

    void on_checkBox_clicked();

    void on_checkBox_2_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

private:
    QTimer      *timer;
    QTimer      *timer_for_record;
    float       *fft;               // recbuf represented by FFT;
    RGB         palette[256];
    Ui::Widget  *ui;
};




#endif // WIDGET_H
