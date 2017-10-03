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

#ifndef W_USERTESTING_H
#define W_USERTESTING_H

//****************************************************************************
// Include(s)
//****************************************************************************

#include <QWidget>
#include "counter.h"
#include "flexsea_generic.h"
#include "scribblearea.h"

//class ScribbleArea;

namespace Ui {
class W_UserTesting;
}

class W_UserTesting : public QWidget, public Counter<W_UserTesting>
{
	Q_OBJECT

public:
	explicit W_UserTesting(QWidget *parent = 0);
	~W_UserTesting();

signals:
	void windowClosed(void);

private slots:
	void on_pushButtonApprove_clicked();
	void on_pushButtonSigClear_clicked();
	void on_lineEditNameFirst_editingFinished();
	void on_lineEditNameM_editingFinished();
	void on_lineEditNameLast_editingFinished();
	void on_lineEditNameUID_editingFinished();

private:
	Ui::W_UserTesting *ui;
	ScribbleArea *scribbleArea;
	QString userID;

	void initSigBox(void);
	void nameEditingFinished(uint8_t i);
};

//****************************************************************************
// Definition(s)
//****************************************************************************

#endif // W_USERTESTING_H