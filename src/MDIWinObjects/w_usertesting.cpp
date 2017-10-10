/****************************************************************************
	[Project] FlexSEA: Flexible & Scalable Electronics Architecture
	[Sub-project] 'plan-gui' Graphical User Interface
	Copyright (C) 2017 Dephy, Inc. <http://dephy.com/>
*****************************************************************************
	[Lead developper] Jean-Francois (JF) Duval, jfduval at dephy dot com.
	[Contributors]
*****************************************************************************
	[This file] w_usertesting.h: User testing Form Window
*****************************************************************************
	[Change log] (Convention: YYYY-MM-DD | author | comment)
	* 2017-10-03 | jfduval | New sub-project
	*
****************************************************************************/

//****************************************************************************
// Include(s)
//****************************************************************************

#include <QtWidgets>
#include <stdint.h>
#include "flexsea_generic.h"
#include "w_usertesting.h"
#include "ui_w_usertesting.h"
#include <flexsea_system.h>
#include <flexsea_comm.h>
#include <flexsea_board.h>
#include <QString>
#include <QTextStream>
#include <QColor>
#include "scribblearea.h"
#include <QMessageBox>
#include <QTimer>
#include <QElapsedTimer>
#include <QFileDialog>
#include <QTextStream>
#include <QDateTime>
#include "w_event.h"
#include <QButtonGroup>
#include "cmd-UTT.h"


//****************************************************************************
// Constructor & Destructor:
//****************************************************************************

W_UserTesting::W_UserTesting(QWidget *parent,
							QString appPath) :
	QWidget(parent),
	ui(new Ui::W_UserTesting)
{
	ui->setupUi(this);

	setWindowTitle(this->getDescription());
	setWindowIcon(QIcon(":icons/d_logo_small.png"));

	uiSetup = true;

	initTabs();
	initTabSubject();
	initTabExperiment();
	initTabTweaks();
	initTimers();

	mwAppPath = appPath;
	tweakHasChanged = false;
	uiSetup = false;
}

W_UserTesting::~W_UserTesting()
{
	emit windowClosed();
	delete ui;
}

//****************************************************************************
// Public function(s):
//****************************************************************************


//****************************************************************************
// Public slot(s):
//****************************************************************************

void W_UserTesting::logFileName(QString fn, QString fnp)
{
	logFn = fn;
	logFnP = fnp;
}

void W_UserTesting::extFlags(int index)
{
	qDebug() << "Received from Event" << index;
	if(ui->tabWidget->currentIndex() != 0)
	{
		//We do not send flags before the session has started
		flags(index, true);
	}
}

//****************************************************************************
// Private function(s):
//****************************************************************************

void W_UserTesting::initTabs(void)
{
	ui->tabWidget->setCurrentIndex(0);
	ui->tabWidget->setTabEnabled(0, true);
	ui->tabWidget->setTabEnabled(1, false);
	ui->tabWidget->setTabEnabled(2, false);

	ongoingSession = false;
	pbSession(true);
}

void W_UserTesting::initTabSubject(void)
{
	ui->radioButtonSexM->setChecked(true);
	userID = "U-";
	ui->lineEditNameUID->setText(userID);
	initSigBox();
}

void W_UserTesting::initTabExperiment(void)
{
	ui->radioButtonActWalk->setChecked(true);
	sliderToSpin();

	ui->label_ExpUID->setText(userID);

	ui->radioButtonDataF->setChecked(true);
	ui->radioButtonDUT_L->setChecked(true);

	//Start/Stop:
	ui->pushButtonExpStart->setEnabled(true);
	ui->pushButtonExpStop->setEnabled(false);

	ongoingExperiment = false;

	currentSpeed = ui->doubleSpinBoxSpeed->value();
	currentIncline = ui->doubleSpinBoxIncline->value();

	//Button group - Activity:
	qbgActivity = new QButtonGroup(this);
	qbgActivity->addButton(ui->radioButtonActWalk);
	qbgActivity->addButton(ui->radioButtonActRun);
	qbgActivity->addButton(ui->radioButtonActOther);
	connect(qbgActivity, SIGNAL(buttonClicked(QAbstractButton*)), this, SLOT(activityButton(QAbstractButton*)));

	//Button groups - DUT:
	qbgDUT = new QButtonGroup(this);
	qbgDUT->addButton(ui->radioButtonDUT_L);
	qbgDUT->addButton(ui->radioButtonDUT_R);
	qbgDUT->addButton(ui->radioButtonDUT_D);
	connect(qbgDUT, SIGNAL(buttonClicked(QAbstractButton*)), this, SLOT(dutButton(QAbstractButton*)));

	//Button groups - Data:
	qbgData = new QButtonGroup(this);
	qbgData->addButton(ui->radioButtonDataF);
	qbgData->addButton(ui->radioButtonDataA);
	connect(qbgData, SIGNAL(buttonClicked(QAbstractButton*)), this, SLOT(dataButton(QAbstractButton*)));
}

void W_UserTesting::initTabTweaks(void)
{
	//Controller list:
	planUTT.ctrl = 0;
	ui->comboBoxTweaksController->addItem("Unknown");
	ui->comboBoxTweaksController->addItem("No Motor");
	ui->comboBoxTweaksController->addItem("Shadow");
	ui->comboBoxTweaksController->addItem("Spring Only");
	ui->comboBoxTweaksController->addItem("Power");
	//ui->comboBoxTweaksController->setCurrentIndex(planUTT.ctrl);

	//Controller Option list:
	planUTT.ctrlOption = 0;
	ui->comboBoxTweaksControllerOptions->addItem("Unknown/None");
	ui->comboBoxTweaksControllerOptions->addItem("Level 0");
	ui->comboBoxTweaksControllerOptions->addItem("Level 1");
	ui->comboBoxTweaksControllerOptions->addItem("Level 2");
	//ui->comboBoxTweaksControllerOptions->setCurrentIndex(planUTT.ctrlOption);
	//ui->comboBoxTweaksControllerOptions->setEnabled(false);

	//Auto & Write:
	automaticMode = false;
	ui->checkBoxTweaksAutomatic->setChecked(automaticMode);
	ui->pushButtonTweaksWrite->setEnabled(true);
	ui->checkBoxTweaksAutomatic->setEnabled(false);	//ToDo enable once programmed

	//Dials & inputs:
	planUTT.amplitude = 0;
	//ui->dialAmplitude->setValue(planUTT.amplitude);
	//ui->spinBoxTweaksAmp->setValue(ui->dialAmplitude->value());
	planUTT.timing = 0;
	ui->dialTiming->setValue(planUTT.timing);
	ui->spinBoxTweaksTim->setValue(ui->dialTiming->value());

	//Power buttons:
	ui->pushButtonPowerOff->setStyleSheet("background-color: rgb(255, 0, 0); \
											color: rgb(0, 0, 0)");
	ui->pushButtonPowerOn->setStyleSheet("background-color: rgb(0, 255, 0); \
											color: rgb(0, 0, 0)");

	planUTT.powerOn = 0;
	setTweaksUI();
}

//Set all the Tweaks widgets according to planUTT
void W_UserTesting::setTweaksUI(void)
{
	ui->comboBoxTweaksController->setCurrentIndex(planUTT.ctrl);
	ui->comboBoxTweaksControllerOptions->setCurrentIndex(planUTT.ctrlOption);
	ui->comboBoxTweaksControllerOptions->setEnabled(false);
	ui->dialAmplitude->setValue(planUTT.amplitude);
	ui->spinBoxTweaksAmp->setValue(planUTT.amplitude);
	ui->dialTiming->setValue(planUTT.timing);
	ui->spinBoxTweaksTim->setValue(planUTT.timing);
}

void W_UserTesting::initSigBox(void)
{
	QColor penColor;
	penColor.setNamedColor("black");
	scribbleArea = new ScribbleArea;
	//ui->gridLayout_Sig->addWidget(scribbleArea);
	ui->frame->layout()->addWidget(scribbleArea);
			//addWidget(scribbleArea);
	scribbleArea->setPenColor(penColor);
	scribbleArea->setPenWidth(5);

	//Draw box around the area:
	ui->frame->setFrameShape(QFrame::Box);
	ui->frame->setFrameShadow(QFrame::Plain);
	ui->frame->setLineWidth(2);
	ui->frame->setMidLineWidth(1);
}

//Experimental session button: color & text
void W_UserTesting::pbSession(bool ss)
{
	if(ss == true)
	{
		ui->pushButtonExpSession->setStyleSheet("background-color: rgb(0, 255, 0); \
											color: rgb(0, 0, 0)");
		ui->pushButtonExpSession->setText("Start Experimental Session");
	}
	else
	{
		ui->pushButtonExpSession->setStyleSheet("background-color: rgb(255, 0, 0); \
											color: rgb(0, 0, 0)");
		ui->pushButtonExpSession->setText("Stop Experimental Session");
	}
}

void W_UserTesting::initTimers(void)
{
	//Experiment: counts between Start and Stop
	expTime = 0;

	//Display refresh 10Hz:
	dispTimer = new QTimer(this);
	connect(dispTimer, SIGNAL(timeout()), this, SLOT(dispTimerTick()));
	dispTimer->start(100);
}

void W_UserTesting::createNewFile(void)
{
	QString dirName = "UserTesting";
	textFile = new QFile(this);
	textStream = new QTextStream;

	//Create directory if needed
	if(!QDir().exists(dirName)){QDir().mkdir(dirName);}

	QString path = mwAppPath;
	QString pathExp = path + '/' +dirName + '/';
	QString filename = pathExp + "Exp_" + getTimestamp() + '_' + userID + ".txt";
	//Save path for other functions
	utPath = pathExp;

	//Now we open it:
	textFile->setFileName(filename);

	//Check if the file was successfully opened
	if(textFile->open(QIODevice::ReadWrite) == false)
	{
		qDebug() << "Couldn't RW open file " << filename;
		return;
	}

	qDebug() << "Opened:" << filename;
	textStream->setDevice(textFile);

	//textFile->close();	//Done by other function
}

void W_UserTesting::saveSignature(void)
{
	QString fn = utPath + "Sig_" + userID + ".png";
	qDebug() << fn;
	scribbleArea->saveImage(fn, "png");
}

//Read all the input widgets and save data
void W_UserTesting::latchSubjectInfo(void)
{
	saveSignature();

	name[0] = ui->lineEditNameFirst->text();
	name[1] = ui->lineEditNameM->text();
	name[2] = ui->lineEditNameLast->text();

	sex = "Male";
	if(ui->radioButtonSexF->isChecked()){sex = "Female";}
	if(ui->radioButtonSexO->isChecked()){sex = "Other";}

	DOB = "" + QString::number(ui->spinBoxDOB_YYYY->value()) + '-' \
		  + QString::number(ui->spinBoxDOB_MM->value()) + '-' \
		  + QString::number(ui->spinBoxDOB_DD->value());
	height[0] = ui->spinBoxHeight_ft->value();
	height[1] = ui->spinBoxHeightIn->value();
	weight = ui->spinBoxWeight->value();
	//QString sigFileName;
}

//bool start = true for start, false for stop
void W_UserTesting::recordTimestampStartStop(bool start, int len, QString un)
{
	QString prefix, suffix = "", wtf;
	if(start)
	{
		prefix = " Start";
	}
	else
	{
		prefix = " Stop";
		suffix = " (Length: " + QString::number(len) + "s)";
	}

	//Write to file:
	wtf = getTimestamp() + prefix + suffix;
	*textStream << wtf << endl;

	if(start)
	{
		//Filenames used:
		*textStream << "#Log file: " << logFn << endl;
		*textStream << "#Log file (full path): " << logFnP << endl;
		*textStream << "#Log file user notes: " << un << endl;
		//All parameters:
		getAllInputs();
	}
}

QString W_UserTesting::getTimestamp(void)
{
	return (QDate::currentDate().toString("yyyy-MM-dd_") +
			QTime::currentTime().toString("HH'h'mm'm'ss's'"));
}

//Write 'txt' To File. Timestamp added automatically.
void W_UserTesting::wtf(QString txt)
{
	*textStream << getTimestamp() << " " << txt << endl;
}

void W_UserTesting::writeSubjectInfo(void)
{
	*textStream << "UID: " << userID << endl;
	*textStream << "First name: " << name[0] << endl;
	*textStream << "Middle name: " << name[1] << endl;
	*textStream << "Middle name: " << name[2] << endl;
	*textStream << "Sex: " << sex << endl;
	*textStream << "DOB (YYYY-MM-DD): " << DOB << endl;
	*textStream << "Height: " << height[0] << "'" << height[1] << '"' << endl;
	*textStream << "Weight (lbs): " << weight << endl;
	*textStream << "---" << endl;
}

void W_UserTesting::writeSpeedIncline(double spd, double inc)
{
	wtf("Speed = " + QString::number(spd) + ", Incline = " + QString::number(inc));
}

void W_UserTesting::writeAmplitudeTiming(int amp, int tim)
{
	wtf("Amplitude = " + QString::number(amp) + "%, Timing = " + QString::number(tim) + "%");
}

void W_UserTesting::writeNotes(void)
{
	*textStream << "---" << endl << "User notes begin >>>" << endl;
	*textStream << ui->plainTextEdit->toPlainText() << endl << "<<< End of user notes." << endl;
}

void W_UserTesting::closeTextFile(void)
{
	textFile->close();
}

//Start of a session
void W_UserTesting::startOfSession()
{
	//Move to the next tab, lock this one
	ui->tabWidget->setCurrentIndex(1);
	ui->tabWidget->setTabEnabled(0, false);
	ui->tabWidget->setTabEnabled(1, true);
	ui->tabWidget->setTabEnabled(2, true);

	createNewFile();
	latchSubjectInfo();
	writeSubjectInfo();
}

//End of a session
void W_UserTesting::endOfSession()
{
	//Move to the Subject tab, lock this one
	ui->tabWidget->setCurrentIndex(0);
	ui->tabWidget->setTabEnabled(0, true);
	ui->tabWidget->setTabEnabled(1, false);
	ui->tabWidget->setTabEnabled(2, false);

	writeNotes();
	closeTextFile();
}

void W_UserTesting::writeUTT(void)
{
	uint16_t numb = 0;
	uint8_t info[2] = {PORT_USB, PORT_USB};

	//Prep & send:
	tx_cmd_utt_w(TX_N_DEFAULT, 0, &planUTT);
	pack(P_AND_S_DEFAULT, FLEXSEA_MANAGE_1, info, &numb, comm_str_usb);
	emit writeCommand(numb, comm_str_usb, WRITE);
}

//****************************************************************************
// Private slot(s):
//****************************************************************************

void W_UserTesting::dispTimerTick(void)
{
	if(ongoingExperiment){expTime = expTimer.elapsed() / 1000;}
	ui->lcdNumberSeconds->display(expTime);

	//We use this slow refresh to filter out the speed and incline logs:
	static uint8_t spdDiv = 0;
	static double lastSpeed = currentSpeed;
	static double lastIncline = currentIncline;
	spdDiv++;
	spdDiv %= 10;
	if(!spdDiv)
	{
		if(currentSpeed != lastSpeed || currentIncline != lastIncline)
		{
			lastSpeed = currentSpeed;
			lastIncline = currentIncline;
			writeSpeedIncline(currentSpeed, currentIncline);
		}
	}

	//Same filtering for the Tweaks tab:
	static uint8_t twDiv = 0;
	static double lastAmplitude = planUTT.amplitude;
	static double lastTiming = planUTT.timing;
	twDiv++;
	twDiv %= 10;
	if(!twDiv)
	{
		if(planUTT.amplitude != lastAmplitude || planUTT.timing != lastTiming)
		{
			lastAmplitude = planUTT.amplitude;
			lastTiming = planUTT.timing;
			writeAmplitudeTiming(planUTT.amplitude, planUTT.timing);
			tweakHasChanged = true;
		}
	}

	//Read button:
	if(readDisplayLag > 0)
	{
		readDisplayLag--;
		if(!readDisplayLag)
		{
			//We have waited long enough
			planUTT = utt;
			qDebug() << "Refreshing display based on read data.";
			setTweaksUI();
		}
	}

	//Color-code Write to Device:
	if(tweakHasChanged == true)
	{
		ui->pushButtonTweaksWrite->setStyleSheet("background-color: rgb(255, 255, 0); \
												color: rgb(0, 0, 0)");
	}
	else
	{
		ui->pushButtonTweaksWrite->setStyleSheet("");
	}
}

void W_UserTesting::on_pushButtonExpSession_clicked()
{
	if(ongoingSession == false)
	{
		pbSession(false);
		startOfSession();
		ongoingSession = true;

		//Clear the timer - cleaner start:
		expTime = 0;
	}
	else
	{
		//End any ongoing experiment
		if(ongoingExperiment)
		{
			emit on_pushButtonExpStop_clicked();
		}

		pbSession(true);
		endOfSession();
		ongoingSession = false;
	}
}

//****************************************************************************
// Private slot(s) - Tab User:
//****************************************************************************

void W_UserTesting::on_pushButtonSigClear_clicked(){scribbleArea->clearImage();}

void W_UserTesting::on_lineEditNameFirst_editingFinished(){nameEditingFinished(0);}
void W_UserTesting::on_lineEditNameM_editingFinished(){nameEditingFinished(1);}
void W_UserTesting::on_lineEditNameLast_editingFinished(){nameEditingFinished(2);}
void W_UserTesting::on_lineEditNameUID_editingFinished(){nameEditingFinished(3);}

void W_UserTesting::nameEditingFinished(uint8_t i)
{
	if(i < 3)
	{
		//Generate new UID based on initials. Skips empties.
		QString uid = "U-";
		QChar letter;
		letter = ui->lineEditNameFirst->text().at(0);
		if(letter != 0){uid += letter;}
		letter = ui->lineEditNameM->text().at(0);
		if(letter != 0){uid += letter;}
		letter = ui->lineEditNameLast->text().at(0);
		if(letter != 0){uid += letter;}

		userID = uid;
		ui->lineEditNameUID->setText(uid);
	}
	else
	{
		userID = ui->lineEditNameUID->text();
	}

	ui->label_ExpUID->setText(userID);	//Label on Exp tab
}

//****************************************************************************
// Private slot(s) - Tab Experiment:
//****************************************************************************

void W_UserTesting::on_pushButtonExpStart_clicked()
{
	int refreshRate = 7;	//Index that corresponds to 200Hz
	ui->pushButtonExpStart->setEnabled(false);
	ui->pushButtonExpStop->setEnabled(true);
	ongoingExperiment = true;
	QString offs = "";
	if(ui->radioButtonDataA->isChecked()){offs = "o=0,1,2,3;";}
	else{offs = "o=0;";}

	qint64 numericalTimestamp = QDateTime::currentMSecsSinceEpoch();
	QString logTxt = userID + "-" + QString::number(numericalTimestamp);
	emit startExperiment(refreshRate, true, true, offs, logTxt);
	expTimer.start();
	recordTimestampStartStop(true, 0, logTxt);
}

void W_UserTesting::on_pushButtonExpStop_clicked()
{
	ui->pushButtonExpStart->setEnabled(true);
	ui->pushButtonExpStop->setEnabled(false);
	ongoingExperiment = false;
	emit stopExperiment();
	recordTimestampStartStop(false, expTime, "");
}

void W_UserTesting::on_horizontalSliderSpeed_valueChanged(int value)
{
	double realValue = (double)value / 10;
	speed(0, realValue);
}

void W_UserTesting::on_horizontalSliderIncline_valueChanged(int value)
{
	double realValue = (double)value / 10;
	incline(0, realValue);
}

void W_UserTesting::on_doubleSpinBoxSpeed_valueChanged(double arg1){speed(1, arg1);}
void W_UserTesting::on_doubleSpinBoxIncline_valueChanged(double arg1){incline(1, arg1);}

void W_UserTesting::speed(int index, double val)
{
	if(index == 0)
	{
		//qDebug() << "Speed:" << val;
		//Change came from the slider
		ui->doubleSpinBoxSpeed->setValue(val);
	}
	else
	{
		//Change came from the spinBox
		double rV = val*10;
		ui->horizontalSliderSpeed->setValue((int)rV);
	}

	currentSpeed = val;
}

void W_UserTesting::incline(int index, double val)
{
	if(index == 0)
	{
		qDebug() << "Incline:" << val;
		//Change came from the slider
		ui->doubleSpinBoxIncline->setValue(val);
	}
	else
	{
		//Change came from the spinBox
		double rV = val*10;
		ui->horizontalSliderIncline->setValue((int)rV);
	}

	currentIncline = val;
}

//Call this once at boot to match all the displays
void W_UserTesting::sliderToSpin(void)
{
	double val = (double)ui->horizontalSliderSpeed->value()/10;
	ui->doubleSpinBoxSpeed->setValue(val);
	val = (double)ui->horizontalSliderIncline->value()/10;
	ui->doubleSpinBoxIncline->setValue(val);
}

void W_UserTesting::on_pushButtonClearNotes_clicked(){	ui->plainTextEdit->clear();}

void W_UserTesting::on_pushButtonFlagA_clicked(){flags(0, false);}
void W_UserTesting::on_pushButtonFlagB_clicked(){flags(1, false);}
void W_UserTesting::on_pushButtonFlagC_clicked(){flags(2, false);}
void W_UserTesting::on_pushButtonFlagD_clicked(){flags(3, false);}

void W_UserTesting::flags(int index, bool external)
{
	static bool butStat[4] = {false, false, false, false};
	QString wtf = "", txt = "";
	if(index == 0){txt = ui->lineEditFlagA->text();}

	if(external && butStat[index])
	{
		butStat[index] = false;
		return;
	}

	wtf = getTimestamp() + " Flag " + QString::number(1 << index) + " " + txt;
	*textStream << wtf << endl;

	if(external == false)
	{
		//qDebug() << "Internal flag";
		butStat[index] = true;
		emit userFlags(index);
	}
}

void W_UserTesting::activityButton(QAbstractButton* myb)
{
	QString actPrefixText = "Activity = ", actText = "Walk";
	if(myb == ui->radioButtonActRun){actText = "Run";}
	if(myb == ui->radioButtonActOther){actText = "Other";}
	//qDebug() << (actPrefixText + actText);
	wtf(actPrefixText + actText);
}

void W_UserTesting::dutButton(QAbstractButton* myb)
{
	QString dutPrefixText = "DUT = ", dutText = "Left";
	if(myb == ui->radioButtonDUT_R){dutText = "Right";}
	if(myb == ui->radioButtonDUT_D){dutText = "Dual";}

	wtf(dutPrefixText + dutText);
}

void W_UserTesting::dataButton(QAbstractButton* myb)
{
	QString dataPrefixText = "Data = ", dataText = "Fast";
	if(myb == ui->radioButtonDataF){dataText = "All";}

	wtf(dataPrefixText + dataText);
}

//Call this when it starts so we know the starting condition
void W_UserTesting::getAllInputs(void)
{
	QString tmstp = getTimestamp();

	//Activity:
	QString actPre = "Activity = ", act = "Walk";
	if(ui->radioButtonActRun->isChecked()){act = "Run";}
	if(ui->radioButtonActOther->isChecked()){act = "Other";}

	//Data:
	QString dataPre = "Data = ", data = "Fast";
	if(ui->radioButtonDataA->isChecked()){data = "All";}

	//DUT:
	QString dutPre = "DUT = ", dut = "Left";
	if(ui->radioButtonDUT_R->isChecked()){dut = "Right";}
	if(ui->radioButtonDUT_D->isChecked()){dut = "Dual";}

	//Speed:
	QString spdPre = "Speed = ", spd = "";
	spd = QString::number((double)ui->horizontalSliderSpeed->value()/10);

	//Incline
	QString incPre = "Incline ", inc = "";
	inc = QString::number((double)ui->horizontalSliderIncline->value()/10);

	//Write to file:
	*textStream << tmstp << " Starting conditions: " << (actPre + act) << ", " << \
					(dataPre + data) << ", " << (dutPre + dut) << ", " << \
					(spdPre + spd) << ", " << (incPre + inc) << endl;
}

//****************************************************************************
// Private slot(s) - Tab Tweaks:
//****************************************************************************

void W_UserTesting::on_comboBoxTweaksController_currentIndexChanged(int index)
{
	//"Power" has sub-options:
	bool enableOptions = false;
	if(index == 4){enableOptions = true;}
	else
	{
		ui->comboBoxTweaksControllerOptions->setCurrentIndex(0);
	}
	ui->comboBoxTweaksControllerOptions->setEnabled(enableOptions);

	tweaksController(0, index);
}

void W_UserTesting::on_comboBoxTweaksControllerOptions_currentIndexChanged(int index)
{
	tweaksController(1, index);
}

void W_UserTesting::tweaksController(int source, int index)
{
	if(uiSetup){return;}

	if(source == 0)
	{
		//Main controller:
		planUTT.ctrl = index;
		if(index == 4)
		{
			planUTT.ctrlOption = ui->comboBoxTweaksControllerOptions->currentIndex();
		}
		else
		{
			planUTT.ctrlOption = 0;
		}
	}
	else
	{
		//Sub:
		planUTT.ctrl = ui->comboBoxTweaksController->currentIndex();
		planUTT.ctrlOption = index;
	}

	//qDebug() << "Controller:" << planUTT.ctrl << " Option:" << planUTT.ctrlOption;
	wtf("Controller: " + QString::number(planUTT.ctrl) + " (" + QString::number(planUTT.ctrlOption) + ")");
	tweakHasChanged = true;
}

void W_UserTesting::on_dialAmplitude_valueChanged(int value){tweaksAmplitude(0, value);}
void W_UserTesting::on_spinBoxTweaksAmp_valueChanged(int arg1){tweaksAmplitude(1, arg1);}

void W_UserTesting::tweaksAmplitude(int source, int val)
{
	//Protect against recursions:
	static bool internal = false;
	if(internal == true)
	{
		internal = false;
		return;
	}

	if(source == 0)
	{
		//Change came from the dial:
		internal = true;
		ui->spinBoxTweaksAmp->setValue(val);
	}
	else
	{
		//Change came from the spinbox:
		internal = true;
		ui->dialAmplitude->setValue(val);
	}

	planUTT.amplitude = val;
	//qDebug() << "Amplitude:" << planUTT.amplitude;
}

void W_UserTesting::on_dialTiming_valueChanged(int value){tweaksTiming(0, value);}
void W_UserTesting::on_spinBoxTweaksTim_valueChanged(int arg1){tweaksTiming(1, arg1);}

void W_UserTesting::tweaksTiming(int source, int val)
{
	//Protect against recursions:
	static bool internal = false;
	if(internal == true)
	{
		internal = false;
		return;
	}

	if(source == 0)
	{
		//Change came from the dial:
		internal = true;
		ui->spinBoxTweaksTim->setValue(val);
	}
	else
	{
		//Change came from the spinbox:
		internal = true;
		ui->dialTiming->setValue(val);
	}

	planUTT.timing = val;
	//qDebug() << "Timing:" << planUTT.timing;
}

void W_UserTesting::on_checkBoxTweaksAutomatic_stateChanged(int arg1)
{
	QString txt = "";

	if(arg1 == 0)
	{
		//Unchecked: normal (non-automatic) mode
		ui->pushButtonTweaksWrite->setEnabled(true);
		txt = "Normal mode (unchecked)";
	}
	else
	{
		ui->pushButtonTweaksWrite->setEnabled(false);
		txt = "Automatic mode (checked)";
	}

	automaticMode = (bool)arg1;
	wtf("Automatic checkbox clicked: " + txt);
	//qDebug() << "Automatic mode:" << automaticMode;
}

void W_UserTesting::on_pushButtonTweaksRead_clicked()
{
	wtf("Read From Device was clicked");

	uint16_t numb = 0;
	uint8_t info[2] = {PORT_USB, PORT_USB};

	//Prep & send:
	tx_cmd_utt_r(TX_N_DEFAULT, 0);
	pack(P_AND_S_DEFAULT, FLEXSEA_MANAGE_1, info, &numb, comm_str_usb);
	emit writeCommand(numb, comm_str_usb, READ);

	readDisplayLag = 5;
}

void W_UserTesting::on_pushButtonTweaksWrite_clicked()
{
	wtf("Write to Device was clicked");
	writeUTT();
	tweakHasChanged = false;
}

void W_UserTesting::on_pushButtonPowerOff_clicked()
{
	planUTT.powerOn = 0;
	wtf("Power Off was clicked");
	writeUTT();
}

void W_UserTesting::on_pushButtonPowerOn_clicked()
{
	planUTT.powerOn = 1;
	wtf("Power On was clicked");
	writeUTT();
}
