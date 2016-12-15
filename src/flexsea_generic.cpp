/****************************************************************************
	[Project] FlexSEA: Flexible & Scalable Electronics Architecture
	[Sub-project] 'plan-gui' Graphical User Interface
	Copyright (C) 2016 Dephy, Inc. <http://dephy.com/>

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <http://www.gnu.org/licenses/>.
*****************************************************************************
	[Lead developper] Jean-Francois (JF) Duval, jfduval at dephy dot com.
	[Origin] Based on Jean-Francois Duval's work at the MIT Media Lab
	Biomechatronics research group <http://biomech.media.mit.edu/>
	[Contributors]
*****************************************************************************
	[This file] flexsea_generic: Generic functions used by many classes
*****************************************************************************
	[Change log] (Convention: YYYY-MM-DD | author | comment)
	* 2016-09-09 | jfduval | Initial GPL-3.0 release
	* 2016-09-12 | jfduval | Moved the status display here, from w_execute
	* 2016-09-16 | jfduval | Major code rework to support multiple boards
****************************************************************************/

//****************************************************************************
// Include(s)
//****************************************************************************

#include "flexsea_generic.h"
#include "main.h"
#include <QComboBox>
#include <QDebug>

#include "batteryDevice.h"
#include "executeDevice.h"
#include "gossipDevice.h"
#include "manageDevice.h"
#include "ricnuDevice.h"
#include "strainDevice.h"


//****************************************************************************
// Static Variable initialization
//****************************************************************************

//Lookup from list to actual slave number (FlexSEA convention):
uint8_t FlexSEA_Generic::list_to_slave[SL_LEN_ALL] = {FLEXSEA_EXECUTE_1,
														FLEXSEA_EXECUTE_2,
														FLEXSEA_EXECUTE_3,
														FLEXSEA_EXECUTE_4,
														FLEXSEA_MANAGE_1,
														FLEXSEA_MANAGE_2,
														FLEXSEA_PLAN_1,
														FLEXSEA_GOSSIP_1,
														FLEXSEA_GOSSIP_2,
														FLEXSEA_BATTERY_1,
														FLEXSEA_STRAIN_1,
														FLEXSEA_VIRTUAL_1};
//Slaves:
QStringList FlexSEA_Generic::var_list_slave =    QStringList()
												 << "Execute 1"
												 << "Execute 2"
												 << "Execute 3"
												 << "Execute 4"
												 << "Manage 1"
												 << "Manage 2"
												 << "Plan 1"
												 << "Gossip 1"
												 << "Gossip 2"
												 << "Battery 1"
												 << "Strain 1"
												 << "RIC/NU 1";

//Experiments:
QStringList FlexSEA_Generic::var_list_exp = QStringList()
											<< "Read All (Barebone)"
											<< "In Control"
											<< "RICNU Knee"
											<< "CSEA Knee"
											<< "2DOF Ankle"
											<< "[Your project]";

//****************************************************************************
// Constructor & Destructor:
//****************************************************************************

FlexSEA_Generic::FlexSEA_Generic(QWidget *parent) : QWidget(parent)
{

}

//****************************************************************************
// Public function(s):
//****************************************************************************

//Populates a Slave List ComboBox
void FlexSEA_Generic::populateSlaveComboBox(QComboBox *cbox, uint8_t base, \
											uint8_t len)
{
	QString slave_name;

	for(int index = base; index < (base+len); index++)
	{
		slave_name = var_list_slave.at(index);
		cbox->addItem(slave_name);
	}
}

//Populates an Experiment List Combo Box - no parameters
void FlexSEA_Generic::populateExpComboBox(QComboBox *cbox)
{
	QString exp_name;

	for(int index = 0; index < var_list_exp.length(); index++)
	{
		exp_name = var_list_exp.at(index);
		cbox->addItem(exp_name);
	}
}

//Sometimes we need to know if a board is an Execute, without caring about
//if it's Execute 1, 2 or N. This function returns the base code.
uint8_t FlexSEA_Generic::getSlaveBoardType(uint8_t base, uint8_t index)
{
	//Board type? Extract base via address&integer trick
	uint8_t tmp = 0, bType = 0;
	// TODO Won't work when you will have more than 10 board of one type.
	tmp = list_to_slave[base + index] / 10;
	bType = tmp * 10;

	return bType;
}

//Returns the slave name, as a QString
void FlexSEA_Generic::getSlaveName(uint8_t base, uint8_t index, \
								   QString *slaveName)
{
	*slaveName = var_list_slave.at(base+index);
}

//Returns the experiment name, as a QString
void FlexSEA_Generic::getExpName(uint8_t index, QString *expName)
{
	*expName = var_list_exp.at(index);
}

//Returns the FlexSEA Slave ID as a uint8
uint8_t FlexSEA_Generic::getSlaveID(uint8_t base, uint8_t index)
{
	return list_to_slave[base + index];
}

//Prints a FlexSEA packet on the debug terminal
void FlexSEA_Generic::packetVisualizer(uint numb, uint8_t *packet)
{
	QString msg1 = "";
	uint8_t cmd7bits = 0, rw = 0;

	//From CMD1 to R/W and 7-bit code:
	cmd7bits = CMD_7BITS(packet[5]);
	rw = IS_CMD_RW(packet[5]);
	if(rw == READ)
		msg1 = "(Read ";
	else
		msg1 = "(Write ";
	msg1 += QString::number(cmd7bits);
	msg1 += ")";

	qDebug() << "-------------------------";
	qDebug() << "FlexSEA Packet Visualizer";
	qDebug() << "-------------------------";

	QString msg2 = "Raw: ";
	for(uint i = 0; i < numb; i++)
	{
		msg2 += (QString::number(packet[i]) + ',');
	}
	qDebug() << msg2;

	qDebug() << "numb: " << numb;
	qDebug() << "HEADER: " << packet[0];
	qDebug() << "BYTES:: " << packet[1];
	qDebug() << "P_XID: " << packet[2];
	qDebug() << "P_RID: " << packet[3];
	qDebug() << "P_CMDS: " << packet[4];
	qDebug() << "P_CMD1: " << packet[5] << msg1;
	qDebug() << "P_DATA1:" << packet[6];
	qDebug() << "P_DATA2:" << packet[7];
	qDebug() << "...";
	qDebug() << "CHECKSUM:" << packet[packet[1]+2];
	qDebug() << "FOOTER: " << packet[packet[1]+3];

	qDebug() << "-------------------------";
}

//TODO these decoding functions should be in the w_board files

//Decodes some of Execute's fields
void FlexSEA_Generic::decodeExecute(uint8_t base, uint8_t index)
{
	struct execute_s *exPtr;
	assignExecutePtr(&exPtr, base, index);
	ExecuteDevice::decode(exPtr);
}

//RIC/NU is a special case of Execute board. It use the first struct of execute
// and strain.
void FlexSEA_Generic::decodeRicnu(uint8_t base, uint8_t index)
{
	(void)base;
	(void)index;
	ricnu_1.ex = exec1;
	ricnu_1.st = strain1;
	ExecuteDevice::decode(&ricnu_1.ex);
	StrainDevice::decode(&ricnu_1.st);
}

void FlexSEA_Generic::decodeManage(uint8_t base, uint8_t index)
{
	struct manage_s *mnPtr;
	assignManagePtr(&mnPtr, base, index);
	ManageDevice::decode(mnPtr);
}

//Decodes some of Gossip's fields
void FlexSEA_Generic::decodeGossip(uint8_t base, uint8_t index)
{
	struct gossip_s *goPtr;
	assignGossipPtr(&goPtr, base, index);
	GossipDevice::decode(goPtr);
}

void FlexSEA_Generic::decodeBattery(uint8_t base, uint8_t index)
{
	struct battery_s *baPtr;
	assignBatteryPtr(&baPtr, base, index);
	BatteryDevice::decode(baPtr);
}

//Decodes some of Strain's fields
void FlexSEA_Generic::decodeStrain(uint8_t base, uint8_t index)
{
	struct strain_s *stPtr;
	assignStrainPtr(&stPtr, base, index);
	StrainDevice::decode(stPtr);
}

//Decodes some of the slave's fields
void FlexSEA_Generic::decodeSlave(uint8_t base, uint8_t index)
{
	uint8_t bType = getSlaveBoardType(base, index);

	switch(bType)
	{
		case FLEXSEA_PLAN_BASE:

			break;
		case FLEXSEA_MANAGE_BASE:
			decodeManage(base, index);
			break;
		case FLEXSEA_EXECUTE_BASE:
			decodeExecute(base, index);
			decodeRicnu(base, index);
			break;
		case FLEXSEA_BATTERY_BASE:
			decodeBattery(base, index);
			break;
		case FLEXSEA_STRAIN_BASE:
			decodeStrain(base, index);
			break;
		case FLEXSEA_GOSSIP_BASE:
			decodeGossip(base, index);
			break;
		default:
			break;
	}
}

//Assign pointer
//TODO: should we use flexsea_system's executePtrXid instead?
void FlexSEA_Generic::assignExecutePtr(struct execute_s **myPtr, uint8_t base, \
									   uint8_t slave)
{
	//Based on selected slave, what structure do we use?
	switch(list_to_slave[base+slave])
	{
		case FLEXSEA_EXECUTE_1:
			*myPtr = &exec1;
			break;
		case FLEXSEA_EXECUTE_2:
			*myPtr = &exec2;
			break;
		case FLEXSEA_EXECUTE_3:
			*myPtr = &exec3;
			break;
		case FLEXSEA_EXECUTE_4:
			*myPtr = &exec4;
			break;
		default:
			*myPtr = &exec1;
			break;
	}
}

//Assign pointer
void FlexSEA_Generic::assignManagePtr(struct manage_s **myPtr, uint8_t base, \
									  uint8_t slave)
{
	//Based on selected slave, what structure do we use?
	switch(list_to_slave[base+slave])
	{
		case FLEXSEA_MANAGE_1:
			*myPtr = &manag1;
			break;
		case FLEXSEA_MANAGE_2:
			*myPtr = &manag2;
			break;
		default:
			*myPtr = &manag1;
			break;
	}
}

//Assign pointer
void FlexSEA_Generic::assignRicnuPtr(struct ricnu_s **myPtr, uint8_t base, \
									 uint8_t slave)
{
	//Based on selected slave, what structure do we use?
	switch(list_to_slave[base+slave])
	{
		case FLEXSEA_EXECUTE_1: //RIC/NU is the same as Execute
			*myPtr = &ricnu_1;
			break;
		default:
			*myPtr = &ricnu_1;
			break;
	}
}

//Assign pointer
void FlexSEA_Generic::assignGossipPtr(struct gossip_s **myPtr, uint8_t base, \
									  uint8_t slave)
{
	//Based on selected slave, what structure do we use?
	switch(list_to_slave[base+slave])
	{
		case FLEXSEA_GOSSIP_1:
			*myPtr = &gossip1;
			break;
		case FLEXSEA_GOSSIP_2:
			*myPtr = &gossip2;
			break;
		default:
			*myPtr = &gossip1;
			break;
	}
}

//Assign pointer
void FlexSEA_Generic::assignStrainPtr(struct strain_s **myPtr, uint8_t base, \
									  uint8_t slave)
{
	//Based on selected slave, what structure do we use?
	switch(list_to_slave[base+slave])
	{
		case FLEXSEA_STRAIN_1:
			*myPtr = &strain1;
			break;
		default:
			*myPtr = &strain1;
			break;
	}
}

//Assign pointer
void FlexSEA_Generic::assignBatteryPtr(struct battery_s **myPtr, uint8_t base, \
									   uint8_t slave)
{
	//Based on selected slave, what structure do we use?
	switch(list_to_slave[base+slave])
	{
		case FLEXSEA_BATTERY_1:
			*myPtr = &batt1;
			break;
		default:
			*myPtr = &batt1;
			break;
	}
}
