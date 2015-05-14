/***************************************************************************
File                 : HDFFilterPrivate.h
Project              : LabPlot
Description          : Private implementation class for HDFFilter.
--------------------------------------------------------------------
Copyright            : (C) 2015 Stefan Gerlach (stefan.gerlach@uni.kn)
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
#ifndef HDFFILTERPRIVATE_H
#define HDFFILTERPRIVATE_H

#ifdef HAVE_HDF5
#include <hdf5.h>
#endif

class AbstractDataSource;

class HDFFilterPrivate {

	public:
		explicit HDFFilterPrivate(HDFFilter*);

		void parse(const QString & fileName, QTreeWidgetItem* rootItem);
		void read(const QString & fileName, AbstractDataSource* dataSource,
					AbstractFileFilter::ImportMode importMode = AbstractFileFilter::Replace);
		QString readCurrentDataSet(const QString & fileName, AbstractDataSource* dataSource, AbstractFileFilter::ImportMode importMode=AbstractFileFilter::Replace, int lines=-1);
		void write(const QString & fileName, AbstractDataSource* dataSource);

		const HDFFilter* q;

		QString currentDataSet;
		int startRow;
		int endRow;
		int startColumn;
		int endColumn;
		bool autoModeEnabled;

	private:
		const static int MAXNAMELENGTH=1024;
		QList<unsigned long> multiLinkList;	// used to find hard links
#ifdef HAVE_HDF5
		QString translateHDFOrder(H5T_order_t);
		QString translateHDFClass(H5T_class_t);
		QStringList readHDFAttr(hid_t aid);
		QStringList scanHDFAttrs(hid_t oid);
		QStringList readHDFDataType(hid_t tid);
		void scanHDFDataType(hid_t tid, char *dataTypeName,  QTreeWidgetItem* parentItem);
		void scanHDFLink(hid_t gid, char *linkName,  QTreeWidgetItem* parentItem);
		void scanHDFDataSet(hid_t dsid, char *dataSetName,  QTreeWidgetItem* parentItem);
		void scanHDFGroup(hid_t gid, char *groupName, QTreeWidgetItem* parentItem);
#endif
};

#endif
