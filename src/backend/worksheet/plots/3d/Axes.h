/***************************************************************************
    File                 : Axes.h
    Project              : LabPlot
    Description          : 3D plot axes
    --------------------------------------------------------------------
    Copyright            : (C) 2015 by Minh Ngo (minh@fedoraproject.org)

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

#ifndef PLOT3D_AXES_H
#define PLOT3D_AXES_H

#include <QColor>

#include <vtkSmartPointer.h>

class vtkProp;
class vtkActor;
class vtkRenderer;

class Axes{
	public:
		enum AxesType{
			AxesType_Cube = 0,
			AxesType_Plain = 1
		};

		struct Properties{
			AxesType type;
			int fontSize;
			double width;
			QColor xLabelColor;
			QColor yLabelColor;
			QColor zLabelColor;

			Properties();
		};

		Axes(vtkRenderer& renderer, const Properties& props);
		~Axes();

		void updateBounds();

		bool operator==(vtkProp* prop) const;
		bool operator!=(vtkProp* prop) const;

	private:
		void init();

	private:
		vtkRenderer& renderer;
		const Properties props;
		vtkSmartPointer<vtkProp> vtkAxes;
};

#endif