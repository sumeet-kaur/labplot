/***************************************************************************
    File                 : SimpleCopyThroughFilter.h
    Project              : SciDAVis
    --------------------------------------------------------------------
    Copyright            : (C) 2007 by Knut Franke, Tilman Benkert
    Email (use @ for *)  : knut.franke*gmx.de, thzs*gmx.net
    Description          : Filter which copies the provided input unaltered
                           to the output

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
#ifndef SIMPLE_COPY_THROUGH_FILTER_H
#define SIMPLE_COPY_THROUGH_FILTER_H

#include "backend/core/AbstractSimpleFilter.h"

/**
 * \brief Filter which copies the provided input unaltered to the output
 *
 * Most of the necessary methods for this filter are already implemented
 * in AbstractSimpleFilter.
 *
 * The difference between this filter and CopyThroughFilter is that
 * this inherits AbstractColumn and thus can be directly used
 * as input for other filters and plot functions.
 */
class SimpleCopyThroughFilter : public AbstractSimpleFilter
{
	Q_OBJECT

	protected:
		//! All types are accepted.
		virtual bool inputAcceptable(int, const AbstractColumn *)
		{
			return true;
		}
};

#endif // ifndef SIMPLE_COPY_THROUGH_FILTER_H

