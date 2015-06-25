#ifndef WIDGET_H
#define WIDGET_H

# include <iostream>
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
# include <complex>
//---------------------------------------------------------------------------------------------
# include <sqlite3.h>       //DB include

# include <malloc.h>
# include "bass.h"
# include "qcustomplot.h"

# define BUFSTEP    200000
# define SPECWIDTH  368     // display width (should be multiple of 4)
# define SPECHEIGHT 127     // height (changing requires palette adjustments too)

#ifdef QT_DEBUG
    #define QDEBUG(q) qDebug()<< q
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

    std::float_t I0                                (std::float_t);

signals:
    void PlotSpectrumSignal                 ();

    void PlotKaiserWindowSignal             ();

    void fill_ftt_complex_Signal            ();

    void post_filtering_Signal              ();

    void liftering_Signal                   ();

    void kaiser_window_signal               ();

private slots:
    void timerProc                          ();

    void kaiser_window                      ();

    void liftering                          ();

    void post_filtering                     ();

    void plotSpectrums                      ();

    void plotKaiserWindow                   ();

    void from_ftt_to_complex                ();

    void stopRecordingSlot                  ();

    void clearGraphs                        ();

    void on_pushButton_clicked              ();

    void on_pushButton_2_clicked            ();

    void on_horizontalSlider_valueChanged   (std::int32_t value);

    void on_pushButton_3_clicked            ();

    void on_checkBox_clicked                ();

    void on_checkBox_2_clicked              ();

    void on_pushButton_4_clicked            ();

    void on_pushButton_5_clicked            ();

    void on_pushButton_6_clicked();

    void on_pushButton_7_clicked();

private:
    QTimer                                      *timer;
    QTimer                                      *timer_for_record;
    std::float_t                                *fft;               // recbuf represented by FFT;
    RGB                                         palette[256];
    Ui::Widget                                  *ui;
    std::vector<std::complex<std::float_t>>     fft_complex;
    std::vector<std::float_t>                   freq;
    std::vector<std::float_t>                   one;
    std::vector<std::float_t>                   two;
    std::vector<std::float_t>                   three;
    std::vector<std::float_t>                   four;
};




#endif // WIDGET_H
