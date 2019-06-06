/*
 * Copyright (C) 2011 Georgia Institute of Technology, University of Utah,
 * Weill Cornell Medical College
 *
 * This program is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/*
 * This is a template implementation file for a user module derived from
 * DefaultGUIModel with a custom GUI.
 */

#include "ss_ctrl.h"
#include <main_window.h>

using namespace adam;


extern "C" Plugin::Object*
createRTXIPlugin(void)
{
  return new SsCtrl();
}

static DefaultGUIModel::variable_t vars[] = {
  {
    "State-space controller", "Tooltip description",
    DefaultGUIModel::PARAMETER | DefaultGUIModel::DOUBLE,
  },


//definitely need to add reference here
	{
		"X_in","state in", DefaultGUIModel::INPUT | DefaultGUIModel::VECTORDOUBLE,
	},
	{  "r","ref", DefaultGUIModel::INPUT},
	{"q","state_index", DefaultGUIModel::INPUT | DefaultGUIModel::INTEGER},

	{
		"x1","state in", DefaultGUIModel::INPUT,
	},//hardcode
	{
		"x2","state in", DefaultGUIModel::INPUT,
	},//hardcode
	
	{"u","stim out", DefaultGUIModel::OUTPUT,},
	//{"u_sw","stim out", DefaultGUIModel::OUTPUT,},
	//{"debug2","stim out", DefaultGUIModel::OUTPUT,},


};

static size_t num_vars = sizeof(vars) / sizeof(DefaultGUIModel::variable_t);

SsCtrl::SsCtrl(void)
  : DefaultGUIModel("SsCtrl with Custom GUI", ::vars, ::num_vars)
{
  setWhatsThis("<p><b>SsCtrl:</b><br>QWhatsThis description.</p>");
  DefaultGUIModel::createGUI(vars,
                             num_vars); // this is required to create the GUI
  customizeGUI();
  initParameters();
  update(INIT); // this is optional, you may place initialization code directly
                // into the constructor
  refresh();    // this is required to update the GUI with parameter and state
                // values
  QTimer::singleShot(0, this, SLOT(resizeMe()));
}

SsCtrl::~SsCtrl(void)
{
}

void
SsCtrl::execute(void)
{
  stdVec x_in = inputVector(0);
  r = input(1);
  switch_idx = input(2);
  sw_ctrl.switchSys(switch_idx);

//pad x_in?
  //xa = arma::conv_to<Vec>::from(x_in); 
  for (int i=0; i<x.n_rows; i++)
  {
	//handle cases when x_in is the wrong size
	x[i] = ( (i< (x_in.size()-1)) ? x_in[i] : 0 );
  }
 
/*
  u=ctrlr.calcU(r,x);

  output(0) = u;
*/
  u=sw_ctrl.calcU(r,x);
  output(0) = u;

  return;
}

void
SsCtrl::initParameters(void)
{
	switch_scale = 1.4;

	x = arma::vec(2); x.fill(0);
	u = 0;
/*
	ctrlr = lds_ctrl_adam();
	ctrlr.printGains();
	ctrlr.calcU(r,x);
*/
	sw_ctrl = slds_ctrl();
	sw_ctrl.calcU(r,x);
}


void
SsCtrl::update(DefaultGUIModel::update_flags_t flag)
{
  switch (flag) {
    case INIT:
      period = RT::System::getInstance()->getPeriod() * 1e-6; // ms
      break;

    case MODIFY:
      break;

    case UNPAUSE:
      break;

    case PAUSE:
      break;

    case PERIOD:
      period = RT::System::getInstance()->getPeriod() * 1e-6; // ms
      break;

    default:
      break;
  }
}

void
SsCtrl::customizeGUI(void)
{
  QGridLayout* customlayout = DefaultGUIModel::getLayout();

  QGroupBox* button_group = new QGroupBox;

  QPushButton* abutton = new QPushButton("Load Gains");
  QPushButton* bbutton = new QPushButton("Reset Sys");
  QPushButton* zbutton = new QPushButton("Set ctrl gain to 0");
  zbutton->setCheckable(true);
  QHBoxLayout* button_layout = new QHBoxLayout;

  button_group->setLayout(button_layout);
  button_layout->addWidget(abutton);
  button_layout->addWidget(bbutton);
  button_layout->addWidget(zbutton);
  QObject::connect(abutton, SIGNAL(clicked()), this, SLOT(aBttn_event()));
  QObject::connect(bbutton, SIGNAL(clicked()), this, SLOT(bBttn_event()));
  QObject::connect(zbutton, SIGNAL(toggled(bool)), this, SLOT(zBttn_event(bool)));
  customlayout->addWidget(button_group, 0, 0);
  setLayout(customlayout);
}

// functions designated as Qt slots are implemented as regular C++ functions
void
SsCtrl::aBttn_event(void)
{
	//ctrlr.loadGains();
	sw_ctrl.loadGains();
}

void
SsCtrl::bBttn_event(void)
{
	//resetSys();
}

void SsCtrl::zBttn_event(bool tog)
{
	//ctrlr.toggleSilent();
	sw_ctrl.toggleSilent();
}


























