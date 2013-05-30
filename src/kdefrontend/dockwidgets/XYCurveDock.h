/***************************************************************************
    File                 : XYCurveDock.h
    Project            : LabPlot
    --------------------------------------------------------------------
    Copyright         : (C) 2010-2013 Alexander Semke (alexander.semke*web.de)
							(replace * with @ in the email addresses)
    Description      : widget for curve properties
                           
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *  This program is free software; you can redistribute it and/or modify   *
 *  it under the terms of the GNU General Public License as published by   *
 *  the Free Software Foundation; either version 2 of the License, or      *
 *  (at your option) any later version.                                    *
 *                                                                         *
 *  This program is distributed in the hope that it will be useful,        *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 *  GNU General Public License for more details.                           *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the Free Software           *
 *   Foundation, Inc., 51 Franklin Street, Fifth Floor,                    *
 *   Boston, MA  02110-1301  USA                                           *
 *                                                                         *
 ***************************************************************************/

#ifndef XYCURVEDOCK_H
#define XYCURVEDOCK_H

#include <QList>

#include "backend/core/AbstractColumn.h"
#include "ui_xycurvedock.h"
#include <memory>

class XYCurve;
class TreeViewComboBox;
class CurveSymbolFactory;
class AspectTreeModel;
class Column;

class XYCurveDock: public QWidget{
	Q_OBJECT
	
public:
	XYCurveDock(QWidget *parent);
	void setModel(std::auto_ptr<AspectTreeModel>);
	void setCurves(QList<XYCurve*>);
	
private:
	Ui::XYCurveDock ui;
	QList<XYCurve*> m_curvesList;
	std::auto_ptr<AspectTreeModel> m_aspectTreeModel;
	bool m_initializing;
	QStringList dateStrings;
	QStringList timeStrings;
	  
	TreeViewComboBox* cbXColumn;
	TreeViewComboBox* cbYColumn;
	TreeViewComboBox* cbValuesColumn;
	TreeViewComboBox* cbXErrorPlusColumn;
	TreeViewComboBox* cbXErrorMinusColumn;
	TreeViewComboBox* cbYErrorPlusColumn;
	TreeViewComboBox* cbYErrorMinusColumn;
	
	CurveSymbolFactory* symbolFactory;

	void fillSymbolStyles();
	void updateValuesFormatWidgets(const AbstractColumn::ColumnMode);
	void showValuesColumnFormat(const Column*);
	void setModelIndexFromColumn(TreeViewComboBox*, const AbstractColumn*);

private slots:
	void init();
	void retranslateUi();
  
	//SLOTs for changes triggered in XYCurveDock
	void nameChanged();
	void commentChanged();
	void xColumnChanged(const QModelIndex&);
	void yColumnChanged(const QModelIndex&);
	void visibilityChanged(bool);
	
	//Line-Tab
	void lineTypeChanged(int);
	void lineInterpolationPointsCountChanged(int);
  	void lineStyleChanged(int);
	void lineColorChanged(const QColor&);
	void lineWidthChanged(double);
	void lineOpacityChanged(int);
	
	void dropLineTypeChanged(int);
  	void dropLineStyleChanged(int);
	void dropLineColorChanged(const QColor&);
	void dropLineWidthChanged(double);
	void dropLineOpacityChanged(int);
	
	//Symbol-tab
  	void symbolsStyleChanged(int);
	void symbolsSizeChanged(double);
	void symbolsRotationChanged(int);
	void symbolsOpacityChanged(int);
	void symbolsFillingStyleChanged(int);
	void symbolsFillingColorChanged(const QColor&);
	void symbolsBorderStyleChanged(int);
	void symbolsBorderColorChanged(const QColor&);
	void symbolsBorderWidthChanged(double);
	
	//Values-Tab
	void valuesTypeChanged(int);
	void valuesColumnChanged(const QModelIndex&);
	void valuesPositionChanged(int);
	void valuesDistanceChanged(double);
	void valuesRotationChanged(int);
	void valuesOpacityChanged(int);
	void valuesPrefixChanged();
	void valuesSuffixChanged();
	void valuesFontChanged(const QFont&);
	void valuesFontColorChanged(const QColor&);

	//"Error bars"-Tab
	void xErrorTypeChanged(int) const;
	void yErrorTypeChanged(int) const;
	void xErrorPlusColumnChanged(const QModelIndex&) const;
	void xErrorMinusColumnChanged(const QModelIndex&) const;
	void yErrorPlusColumnChanged(const QModelIndex&) const;
	void yErrorMinusColumnChanged(const QModelIndex&) const;
	void errorBarsTypeChanged(int) const;
	void errorBarsCapSizeChanged(double) const;
  	void errorBarsStyleChanged(int) const;
	void errorBarsColorChanged(const QColor&) const;
	void errorBarsWidthChanged(double) const;
	void errorBarsOpacityChanged(int) const;

	//SLOTs for changes triggered in XYCurve
	//TODO
	//General-Tab
	void curveXColumnChanged(const AbstractColumn*);
	void curveYColumnChanged(const AbstractColumn*);
	
	//Values-Tab
	void curveValuesColumnChanged(const AbstractColumn*);

	//"Error bars"-Tab
	void curveXErrorPlusColumnChanged(const AbstractColumn*);
	void curveXErrorMinusColumnChanged(const AbstractColumn*);
	void curveYErrorPlusColumnChanged(const AbstractColumn*);
	void curveYErrorMinusColumnChanged(const AbstractColumn*);

	//load and save
	void loadConfig(KConfig&);
	void saveConfig(KConfig&);
};

#endif
