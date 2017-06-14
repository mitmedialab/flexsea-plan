/****************************************************************************
	[Project] FlexSEA: Flexible & Scalable Electronics Architecture
	[Sub-project] 'plan-gui' Graphical User Interface
	Copyright (C) 2017 Dephy, Inc. <http://dephy.com/>
*****************************************************************************
	[Lead developper] Jean-Francois (JF) Duval, jfduval at dephy dot com.
	[Origin] Based on Jean-Francois Duval's work at the MIT Media Lab
	Biomechatronics research group <http://biomech.media.mit.edu/>
	[Contributors]
*****************************************************************************
	[This file] w_control.cpp: Cycle Tester Window
*****************************************************************************
	[Change log] (Convention: YYYY-MM-DD | author | comment)
	* 2017-05-26 | jfduval | Initial GPL-3.0 release
	*
****************************************************************************/

#ifndef W_CYCLETESTER_H
#define W_CYCLETESTER_H

//****************************************************************************
// Include(s)
//****************************************************************************

#include <QWidget>
#include "counter.h"
#include "flexsea_generic.h"
#include "cmd-CycleTester.h"

//****************************************************************************
// Namespace & Class Definition:
//****************************************************************************

namespace Ui {
class W_CycleTester;
}

class W_CycleTester : public QWidget, public Counter<W_CycleTester>
{
	Q_OBJECT

public:
	//Constructor & Destructor:
	explicit W_CycleTester(QWidget *parent = 0);
	~W_CycleTester();

public slots:

signals:
	void windowClosed(void);
	void writeCommand(uint8_t numb, uint8_t *tx_data, uint8_t r_w);

private slots:

	void on_pushButtonInit_clicked();
	void on_pushButtonStart_clicked();
	void on_pushButtonStop_clicked();
	void on_pushButtonRead_clicked();
	void on_pushButtonStartStreaming_clicked();
	void on_pushButtonReset_clicked();
	void on_pushButtonConfirmReset_clicked();
	void timerEvent(void);

	void on_pushButtonPause_clicked();

private:
	//Variables & Objects:
	Ui::W_CycleTester *ui;
	QTimer *timer;
	bool resetPBstate, streamingPBstate;

	//Function(s):
	void init(void);
	void experimentControl(enum expCtrl e);
	void experimentStats(enum expStats e);
	void resetStats(void);
	void displayStatus(uint8_t s);
	void displayTemp(int8_t t);
};

#define TIMER_PERIOD	40		//40ms / 25Hz

#define TEMP_MIN		15
#define TEMP_MAX		80
#define TEMP_SPAN		(TEMP_MAX-TEMP_MIN)

#endif // W_CYCLETESTER_H
