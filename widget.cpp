# include "widget.h"
# include "ui_widget.h"

std::int32_t     input;				// current input source;
char    *recbuf = NULL;		// recording buffer;
DWORD   reclen;             // recording length;

HRECORD rchan   = 0;		// recording channel;
HSTREAM chan    = 0;		// playback channel;

QPixmap *specpb  = 0;
std::int32_t   *specbuf;


/**
 * @brief Always calling when recodr is running and check recbuf. If recbuf have not enoth memory this function is realloc mem for recbuf.
 */
BOOL CALLBACK RecordingCallback(HRECORD handle, const void *buffer, DWORD length, void *user)
{
    // increase buffer size if needed;
    if ((reclen%BUFSTEP)+length>=BUFSTEP) {
        recbuf=(char*)realloc(recbuf,((reclen+length)/BUFSTEP+1)*BUFSTEP);
        if (!recbuf) {
            rchan=0;
            Error("Out of memory!");
            return FALSE; // stop recording;
        }
    }
    // buffer the data;
    memcpy(recbuf+reclen,buffer,length);
    reclen+=length;
    return TRUE; // continue recording;
}

/**
 * @brief Default constructor. Set a start up configuration.
 * @param QWidget *parent - Link to parent Widget (default is null).
 */
Widget::Widget(QWidget *parent) : QWidget(parent), ui(new Ui::Widget)
{
    try {
        timer = new QTimer(this);
        this->timer_for_record = new QTimer(this);

        connect(timer, SIGNAL(timeout()), this, SLOT(timerProc()));
        connect(timer_for_record, SIGNAL(timeout()), this, SLOT(stopRecordingSlot()));
        connect(this, SIGNAL(PlotSpectrumSignal()), this, SLOT(plotSpectrums()));
        connect(this, SIGNAL(fill_ftt_complex_Signal()), this, SLOT(from_ftt_to_complex()));
        connect(this, SIGNAL(post_filtering_Signal()), this, SLOT(post_filtering()));
        connect(this, SIGNAL(liftering_Signal()), this, SLOT(liftering()));
        connect(this, SIGNAL(kaiser_window_signal()), this, SLOT(kaiser_window()));

        ui->setupUi(this);
        ui->lineEdit->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        /* {
            ui->plot1->xAxis->setBasePen(QPen(Qt::white, 1));
            ui->plot1->yAxis->setBasePen(QPen(Qt::white, 1));
            ui->plot1->xAxis->setTickPen(QPen(Qt::white, 1));
            ui->plot1->yAxis->setTickPen(QPen(Qt::white, 1));
            ui->plot1->xAxis->setSubTickPen(QPen(Qt::white, 1));
            ui->plot1->yAxis->setSubTickPen(QPen(Qt::white, 1));
            ui->plot1->xAxis->setTickLabelColor(Qt::white);
            ui->plot1->yAxis->setTickLabelColor(Qt::white);
            ui->plot1->xAxis->grid()->setPen(QPen(QColor(140, 140, 140), 1, Qt::DotLine));
            ui->plot1->yAxis->grid()->setPen(QPen(QColor(140, 140, 140), 1, Qt::DotLine));
            ui->plot1->xAxis->grid()->setSubGridPen(QPen(QColor(80, 80, 80), 1, Qt::DotLine));
            ui->plot1->yAxis->grid()->setSubGridPen(QPen(QColor(80, 80, 80), 1, Qt::DotLine));
            ui->plot1->xAxis->grid()->setSubGridVisible(true);
            ui->plot1->yAxis->grid()->setSubGridVisible(true);
            ui->plot1->xAxis->grid()->setZeroLinePen(Qt::NoPen);
            ui->plot1->yAxis->grid()->setZeroLinePen(Qt::NoPen);
            ui->plot1->xAxis->setUpperEnding(QCPLineEnding::esSpikeArrow);
            ui->plot1->yAxis->setUpperEnding(QCPLineEnding::esSpikeArrow);

            QLinearGradient plotGradient;
            plotGradient.setStart(0, 0);
            plotGradient.setFinalStop(0, 350);
            plotGradient.setColorAt(0, QColor(80, 80, 80));
            plotGradient.setColorAt(1, QColor(50, 50, 50));
            ui->plot1->setBackground(plotGradient);
            QLinearGradient axisRectGradient;
            axisRectGradient.setStart(0, 0);
            axisRectGradient.setFinalStop(0, 350);
            axisRectGradient.setColorAt(0, QColor(80, 80, 80));
            axisRectGradient.setColorAt(1, QColor(30, 30, 30));
            ui->plot1->axisRect()->setBackground(axisRectGradient);

            ui->plot1->rescaleAxes();
        }*/
        {
            ui->plot2->xAxis->setBasePen(QPen(Qt::white, 1));
            ui->plot2->yAxis->setBasePen(QPen(Qt::white, 1));
            ui->plot2->xAxis->setTickPen(QPen(Qt::white, 1));
            ui->plot2->yAxis->setTickPen(QPen(Qt::white, 1));
            ui->plot2->xAxis->setSubTickPen(QPen(Qt::white, 1));
            ui->plot2->yAxis->setSubTickPen(QPen(Qt::white, 1));
            ui->plot2->xAxis->setTickLabelColor(Qt::white);
            ui->plot2->yAxis->setTickLabelColor(Qt::white);
            ui->plot2->xAxis->grid()->setPen(QPen(QColor(140, 140, 140), 1, Qt::DotLine));
            ui->plot2->yAxis->grid()->setPen(QPen(QColor(140, 140, 140), 1, Qt::DotLine));
            ui->plot2->xAxis->grid()->setSubGridPen(QPen(QColor(80, 80, 80), 1, Qt::DotLine));
            ui->plot2->yAxis->grid()->setSubGridPen(QPen(QColor(80, 80, 80), 1, Qt::DotLine));
            ui->plot2->xAxis->grid()->setSubGridVisible(true);
            ui->plot2->yAxis->grid()->setSubGridVisible(true);
            ui->plot2->xAxis->grid()->setZeroLinePen(Qt::NoPen);
            ui->plot2->yAxis->grid()->setZeroLinePen(Qt::NoPen);
            ui->plot2->xAxis->setUpperEnding(QCPLineEnding::esSpikeArrow);
            ui->plot2->yAxis->setUpperEnding(QCPLineEnding::esSpikeArrow);
            QLinearGradient plotGradient;
            plotGradient.setStart(0, 0);
            plotGradient.setFinalStop(0, 350);
            plotGradient.setColorAt(0, QColor(80, 80, 80));
            plotGradient.setColorAt(1, QColor(50, 50, 50));
            ui->plot2->setBackground(plotGradient);
            QLinearGradient axisRectGradient;
            axisRectGradient.setStart(0, 0);
            axisRectGradient.setFinalStop(0, 350);
            axisRectGradient.setColorAt(0, QColor(80, 80, 80));
            axisRectGradient.setColorAt(1, QColor(30, 30, 30));
            ui->plot2->axisRect()->setBackground(axisRectGradient);

            ui->plot2->rescaleAxes();
        }
        QGraphicsDropShadowEffect *dse  = new QGraphicsDropShadowEffect(this);  // for a Record button
        QGraphicsDropShadowEffect *dse2 = new QGraphicsDropShadowEffect(this);  // for a Play button
        QGraphicsDropShadowEffect *dse3 = new QGraphicsDropShadowEffect(this);  // for a Save
        QGraphicsDropShadowEffect *dse4 = new QGraphicsDropShadowEffect(this);  // for a LineEdit

        dse ->  setBlurRadius(1);
        dse2->  setBlurRadius(1);
        dse3->  setBlurRadius(1);
        dse4->  setBlurRadius(1);

        dse ->  setOffset(QPoint(0,1));
        dse2->  setOffset(QPoint(0,1));
        dse3->  setOffset(QPoint(0,1));
        dse4->  setOffset(QPoint(0,1));

        QColor color;
        color.setRgb(1, 137, 255, 80);

        dse ->setColor(color);
        dse2->setColor(color);
        dse3->setColor(color);
        dse4->setColor(color);


        ui->pushButton  ->  setGraphicsEffect(dse);
        //ui->pushButton_2->  setGraphicsEffect(dse2);
        ui->pushButton_3->  setGraphicsEffect(dse3);
        ui->lineEdit    ->  setGraphicsEffect(dse4);

        this->fft       =   new std::float_t[2048];

        for (std::int32_t i = 0; i < 2048; i++) fft[i] = 0;
    }
    catch (...){
        QDEBUG("err");
    }
}

/**
 * @brief Widget::~Widget Defaul destructor.
 */
Widget::~Widget()
{
    // release all BASS stuff;
    delete fft;
    BASS_RecordFree();
    BASS_Free();
    delete ui;
}

/**
 * @brief Starts the record and set default confuration for audio file. If start record is failure free RECBUF is comming.
 */
void Widget::startRecording() {
    WAVEFORMATEX *wf;
    if (recbuf) { // free old recording;
        BASS_StreamFree(chan);
        chan=0;
        free(recbuf);
        recbuf=NULL;
        //ui->pushButton_2->setEnabled(false);
        ui->pushButton_3->setEnabled(false);
        // close output device before recording incase of half-duplex device;
        BASS_Free();
    }
    // allocate initial buffer and make space for WAVE header;
    recbuf=(char*)malloc(BUFSTEP);
    reclen=44;
    // fill the WAVE header;
    memcpy(recbuf,"RIFF\0\0\0\0WAVEfmt \20\0\0\0",20);
    memcpy(recbuf+36,"data\0\0\0\0",8);
    wf=(WAVEFORMATEX*)(recbuf+20);
    wf->wFormatTag=1;
    wf->nChannels=2;
    wf->wBitsPerSample=16;
    wf->nSamplesPerSec=44100;
    wf->nBlockAlign=wf->nChannels*wf->wBitsPerSample/8;
    wf->nAvgBytesPerSec=wf->nSamplesPerSec*wf->nBlockAlign;
    // start recording @ 44100hz 32-bit stereo;
    if (!(rchan=BASS_RecordStart(44100,2,0,&RecordingCallback,0))) {
        QDEBUG(BASS_ErrorGetCode());
        Error("Couldn't start recording");
        free(recbuf);
        recbuf=0;
        return;
    }
    ui->pushButton->setEnabled(false);
}

/**
 * @brief No incoming parameters. Get data from RCHAN. Creating the stream file. If BASS_StreamCreateFile is failure colled BASS_Free().
 * @param No params
 */
void Widget::stopRecording()
{
    if (BASS_ChannelGetData(rchan, this->fft, BASS_DATA_FFT1024 | BASS_DATA_FFT_COMPLEX) == -1) {
        QDEBUG("Cannot get recbuf from rchan");
        QDEBUG(BASS_ErrorGetCode());
    }

    BASS_ChannelStop(rchan);
    rchan=0;
    ui->pushButton->setText(tr("Record"));
    // complete the WAVE header
    *(DWORD*)(recbuf+4)=reclen-8;
    *(DWORD*)(recbuf+40)=reclen-44;
    // enable "save" button
    ui->pushButton_3->setEnabled(true);
    // setup output device (using default device)
    if (!BASS_Init(-1,44100,0,NULL,NULL)) {
        QDEBUG(BASS_ErrorGetCode());
        QDEBUG("Can't initialize output device");
        return;
    }
    // create a stream from the recording
    if (chan=BASS_StreamCreateFile(TRUE,recbuf,0,reclen,BASS_SAMPLE_FLOAT)) {
        //ui->pushButton_2->setEnabled(true); // enable "play" button
    }
    else
        BASS_Free();
    ui->pushButton->setEnabled(true);
}

/**
 * @brief Widget::updateInputInfo
 */
void Widget::updateInputInfo()
{
    std::float_t level;
    std::int32_t r=BASS_RecordGetInput(input,&level); // get info on the input
    if (r==-1 || level<0) { // failed
        BASS_RecordGetInput(-1,&level); // try master input instead
        if (level<0) level=1; // that failed too, just display 100%
    }
    ui->horizontalSlider->setValue(level*100); // set the level slider
}

/**
 * @brief No incoming parameters. Write recording data to disk.
 * @param No param
 */
void Widget::writeToDisk()
{
    QString file_name = QFileDialog::getSaveFileName(this, tr("Save File"), "", tr("Music Files (*.mp3)"));
    if (file_name != "") {
        FILE *file = fopen(file_name.toStdString().c_str(), "wb");
        if (file) {
            fwrite(recbuf,reclen,1,file);
            fclose(file);
        }
        else
            Error("Can't create the file");

    }
}

/**
 * @brief Plot spectrums on two graphs (first- FFT, second - Kaiser Window)
 */
/*void Widget::plotSpectrums() {
    ui->plot1->addGraph();


    ui->plot1->graph(0)->setPen(QPen(QColor(218,242,4)));
    ui->plot1->graph(0)->setBrush(QColor(10,74,50));

    QVector<double> x;
    QVector<double> y;

    for (std::int32_t i = 1; i < 256; i++) {
        x.push_back(i);
        y.push_back((fft[i*2]+fft[i*2+1])*20);
    }

    ui->plot1->graph(0)->setData(x,y);
    ui->plot1->xAxis->setRange(0, 256);
    ui->plot1->xAxis->setVisible(true);
    ui->plot1->yAxis->setVisible(true);
    // ui->plot1->yAxis->setRange(-1,0);
    ui->plot1->replot();

    emit this->plotKaiserWindow();
}*/

/**
 * @brief Widget::plotKaiserWindow
 */
void Widget::plotKaiserWindow() {
    ui->plot2->addGraph();
    //ui->plot2->addGraph();

    QVector<double> x;
    QVector<double> y;
    QVector<double> reverse_y;

    for (std::int32_t i = 1; i < freq.size() - 1; i++) {
        if (!qIsNaN(freq[i])) {
            x.push_back((i-1)*44100/1024);
            //y.push_back(std::atan2(fft[i*2],fft[i*2+1]));
            y.push_back(std::sqrt(std::pow(fft[i*2],2)+std::pow(fft[i*2+1],2))*10);
            //y.push_back(freq[i]*20);
        }
        //reverse_y.push_back(std::atan2(fft_complex[i].real(), fft_complex[i].imag()));
    }

    //QDEBUG(freq.size());

    ui->plot2->graph(0)->setData(x,y);
    ui->plot2->graph(0)->setPen(QPen(QColor(218,242,4)));
    //ui->plot2->graph(1)->setData(x,reverse_y);
    ui->plot2->xAxis->setRange(0, 22050);
    ui->plot2->xAxis->setVisible(true);
    ui->plot2->xAxis->setLabel(tr("Frequency"));
    ui->plot2->yAxis->setVisible(true);
    ui->plot2->yAxis->setLabel(tr("Magnitude"));
    //ui->plot2->graph(0)->setBrush(QBrush(Qt::black));
    //ui->plot2->graph(1)->setBrush(QBrush(Qt::black));
    ui->plot2->yAxis->setRange(-10, 10);
    ui->plot2->replot();
}

/**
 * @brief zeroth order modified Bessel function of the first kind, which has a complex mathematical description.
 * @param x
 * @return E result of Bessel function std::float_t
 */
std::float_t Widget::I0(std::float_t x) {
    std::float_t Y = x / 2.0f;
    std::float_t T = 0.0000000001;
    std::float_t E, SDE, DE = 1.0;

    E = DE;

    for (std::int32_t i = 1; i < 50; i++) {
        DE = DE*Y / i;
        //QDEBUG(DE << "DE" << i << "i");
        SDE = std::pow(DE, 2);
        E += SDE;
        if ((E*T - SDE) > 0) break;
    }

    return E;
}

/**
 * @brief It is a one-parameter family of window functions used for digital signal processing.
 */
void Widget::kaiser_window() {
    std::float_t iI0b = 0, h = 0;
    std::int32_t k = 0;
    std::float_t b = 1.8;
    std::float_t L = 0.54;
    std::float_t w = 0.0;
    try {
        k = -512;
        //QDEBUG(k << "k");
        iI0b =  1.0 / this->I0(b);
        //QDEBUG(iI0b << "iI0b");

        for (std::int32_t i = 1; i < freq.size() - 1; i++) {
            k++;
            h = std::abs(this->I0(b*std::sqrt(1 - std::pow(2.0*k/(1023),2)))) * iI0b;
            //QDEBUG(h << "h");
            this->freq[i] *= h;

            //this->freq[256 - 1 - i] *= h;
            QDEBUG(freq[i]);
        }



        for (std::int32_t i = 1; i <freq.size() - 1; i++) {
            w = L - (1 - L)*std::cos(2*M_PI*(i+1)/(freq.size()-1));
            w /= 4;
            freq.at(i) *= w;
        }


        emit this->plotKaiserWindow();
    }
    catch (std::exception &ex) {
        std::cerr << ex.what();
        std::cerr << "Kaiser windwow";
    }
}

/**
 * @brief Simple messanger
 * @param er String message
 */
void Error(QString er) {
    QMessageBox msg;
    QString str = er;
    msg.setText(str);
    msg.setIcon(QMessageBox::Critical);
    msg.setStandardButtons(QMessageBox::Ok);
    msg.exec();
}

/**
 * @brief Colled every 0.3 secinds and update text field where represented recording size.
 */
void Widget::timerProc() {
    // update the recording/playback counter
    QString text = "no data";
    if (rchan) // recording
        text = QString::number(BASS_ChannelGetPosition(rchan,BASS_POS_BYTE));
    else if (chan) {
        if (BASS_ChannelIsActive(chan)) // playing
            text = QString::number(BASS_ChannelGetPosition(chan,BASS_POS_BYTE))+"/"+QString::number(BASS_ChannelGetLength(chan,BASS_POS_BYTE));
        else
            text = QString::number(BASS_ChannelGetLength(chan,BASS_POS_BYTE));
    }
    ui->lineEdit->setText(text);
}

/**
 * @brief Colled this events on every events (on buttons, SliderBar etc).
 * @param value a sound value
 */
void Widget::on_horizontalSlider_valueChanged(std::int32_t value) {
    std::float_t level=value/100.0f;
    if (!BASS_RecordSetInput(input,BASS_INPUT_ON,level)){ // failed to set input level
        BASS_RecordSetInput(-1,BASS_INPUT_ON,level); // try master level instead
    }
}
/**
 * @brief Widget::clearGraphs
 */
void Widget::clearGraphs() {
    //this->ui->plot1->clearGraphs();
    this->ui->plot2->clearGraphs();
}
/**
 * @brief Widget::on_pushButton_3_clicked
 */
void Widget::on_pushButton_3_clicked() {
    writeToDisk();
}
/**
 * @brief Widget::on_pushButton_clicked
 */
void Widget::on_pushButton_clicked() {
    if (!rchan) {
        //this->clearGraphs();
        this->timer->start(300);
        this->timer_for_record->start(3000);
        startRecording();
    }
}
/**
 * @brief Widget::stopRecordingSlot
 */
void Widget::stopRecordingSlot() {
    fft_complex.clear();
    freq.clear();
    this->timer->stop();
    this->timer_for_record->stop();
    this->stopRecording();
    emit this->fill_ftt_complex_Signal();
}
/**
 * @brief Widget::from_ftt_to_complex
 */
void Widget::from_ftt_to_complex() {
    std::complex<std::float_t> complex_diggit;
    try {
        for (std::int32_t i = 1; i < 511; i++) {
            complex_diggit.real(fft[i*2]);
            complex_diggit.imag(0);
            //QDEBUG("real" << complex_diggit.real() << " imag" << complex_diggit.imag());
            this->fft_complex.push_back(complex_diggit);
        }

        emit this->post_filtering_Signal();
    }

    catch (std::exception &ex) {
        std::cerr << ex.what();
        std::cerr << "To complex";
    }
}
/**
 * @brief Widget::post_filtering
 */
void Widget::post_filtering() {
    std::float_t k = 0.0;
    std::float_t sq = 0.0;
    try {
        for (std::int32_t i = 1; i < fft_complex.size()-1; i++) {
            freq.push_back(std::sqrt(std::pow(fft_complex[i].real(),2)+std::pow(fft_complex[i].imag(),2))); //magnetude
           // freq[i] = (20*std::log10(std::abs(freq[i])));
            //freq.push_back(std::atan2(fft_complex[i].real(),fft_complex[i].imag()));
            //freq.push_back(fft[i]*100);
            //QDEBUG(freq[i]);
        }
        //emit this->kaiser_window_signal();
        /*
        k = *(std::max_element(freq.begin(), freq.end()));

        sq = 1.58 * std::sqrt(k);

        for (std::int32_t i = 1; i < freq.size(); i++) {
            freq[i] = freq[i]*std::exp(sq/32);
            //QDEBUG(freq[i]);
        }
*/
        emit this->liftering_Signal();

    }

    catch (std::exception &ex) {
        std::cerr << ex.what();
        std::cerr << "Postfiltering";
    }
}
/**
 * @brief Widget::liftering
 */
void Widget::liftering() {
    std::float_t w = 0.0;
    std::float_t xout = 1.0;
    try {
        for (std::int32_t i = 1; i < freq.size(); i++) {
            w = 1 +8*std::sin(M_PI*i/16);
            freq.at(i) *= w;
            //QDEBUG(freq[i]);
        }

        emit this->kaiser_window_signal();
    }
    catch (std::exception &ex) {
        std::cerr << ex.what();
        std::cerr << "Liftering";
    }
}

void Widget::on_pushButton_2_clicked()
{
    QDEBUG(freq.size() << "freq size");
    for (std::int32_t i = 0; i < freq.size(); i++) {
        one.push_back(freq[i]);
    }
    QDEBUG(one.size() << "one size");
}

void Widget::on_pushButton_4_clicked()
{
    for (std::int32_t i = 0; i < freq.size(); i++) {
        two.push_back(freq[i]);
    }
    QDEBUG(two.size() << "two size");
}

void Widget::on_pushButton_5_clicked()
{
    try {
        std::float_t m_one, m_two, m_three, m_four;
        std::float_t sum_one, sum_two, sum_three, sum_four;
        std::float_t sum_one1, sum_two1, sum_three1, sum_four1;

        const std::int32_t some_constant = 4;

        sum_one = 0.0;
        sum_two = 0.0;
        sum_three = 0.0;
        sum_four = 0.0;

        QDEBUG(freq.size());
        for (std::int32_t i = 1; i < freq.size()/some_constant; i++){
            sum_one += one.at(i);
            sum_two += two.at(i);
            sum_three += three.at(i);
            sum_four += four.at(i);
        }

        m_one = sum_one / freq.size()/some_constant;
        m_two = sum_two / freq.size()/some_constant;
        m_three = sum_three / freq.size()/some_constant;
        m_four = sum_four / freq.size()/some_constant;

        sum_one = 0.0;
        sum_two = 0.0;
        sum_three = 0.0;
        sum_four = 0.0;

        sum_one1 = 0.0;
        sum_two1 = 0.0;
        sum_three1 = 0.0;
        sum_four1 = 0.0;

        for (std::int32_t i = 1; i < freq.size()/some_constant; i++) {
            sum_one += (one.at(i) - m_one) * (two.at(i) - m_two);
            sum_two += (one.at(i) - m_one) * (three.at(i) - m_three);
            sum_three += (one.at(i) - m_one) * (four.at(i) - m_four);
            sum_one1 += std::pow((two.at(i) - m_two), 2);
            sum_two1 += std::pow((three.at(i) - m_three), 2);
            sum_three1 += std::pow((four.at(i) - m_four), 2);
            sum_four1 += std::pow((one.at(i) - m_one), 2);
        }

        std::float_t result = 0.0;
        std::float_t result1 = 0.0;
        std::float_t result2= 0.0;
        //std::float_t result3 = 0.0;

        result = std::abs(sum_one / (std::sqrt(sum_four1) * std::sqrt(sum_one1)));
        result1 = std::abs(sum_two / (std::sqrt(sum_four1) * std::sqrt(sum_two1)));
        result2 = std::abs(sum_three / (std::sqrt(sum_four1) * std::sqrt(sum_three1)));

        QMessageBox msg;
        QString temp_str;

        temp_str.setNum(((result+result1+result2)/3.0)*100);

        msg.setText("Compare result: " + temp_str);

        msg.exec();

        QDEBUG((result+result1+result2)/3 << "result");
        one.clear();
        two.clear();
    }
    catch (std::exception &ex) {
        std::cerr << ex.what();
        QDEBUG(freq.size() << "freq");
        QDEBUG(one.size() << "one");
        QDEBUG(two.size() << "two");
    }
}

void Widget::on_pushButton_6_clicked()
{
    for (std::int32_t i = 0; i < freq.size(); i++) {
        three.push_back(freq[i]);
    }
    QDEBUG(two.size() << "two size");
}

void Widget::on_pushButton_7_clicked()
{
    for (std::int32_t i = 0; i < freq.size(); i++) {
        four.push_back(freq[i]);
    }
    QDEBUG(two.size() << "two size");
}
