/***************************************************************************
    File                 : Spreadsheet.cpp
    Project              : SciDAVis
    Description          : Aspect providing a spreadsheet table with column logic
    --------------------------------------------------------------------
    Copyright            : (C) 2006-2008 Tilman Benkert (thzs*gmx.net)
    Copyright            : (C) 2006-2009 Knut Franke (knut.franke*gmx.de)
    Copyright            : (C) 2006-2007 Ion Vasilief (ion_vasilief*yahoo.fr)
                           (replace * with @ in the email addresses)

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
#include "spreadsheet/Spreadsheet.h"

#ifdef ACTIVATE_SCIDAVIS_SPECIFIC_CODE
#include "spreadsheet/SpreadsheetView.h"
#else
#include <KIcon>
#include "kdefrontend/SpreadsheetView.h"
#endif

#include <QApplication>
#include <QDateTime>

Spreadsheet::Spreadsheet(AbstractScriptingEngine *engine, int rows, int columns, const QString& name)
	: AbstractPart(name), scripted(engine)
{
	// set initial number of rows and columns
	for(int i=0; i<columns; i++)
	{
		Column * new_col = new Column(QString::number(i+1), SciDAVis::Numeric);
		new_col->setPlotDesignation(i == 0 ? SciDAVis::X : SciDAVis::Y);
		addChild(new_col);
	}
	setRowCount(rows);

	m_view = NULL;
}

Spreadsheet::~Spreadsheet()
{
}

QWidget *Spreadsheet::view() const
{
	if (!m_view)
	{
		m_view = new SpreadsheetView(const_cast<Spreadsheet*>(this));
	}
	return m_view;
}

int Spreadsheet::rowCount() const
{
	int col_rows, result=0;
	foreach(Column * col, children<Column>())
		if ((col_rows=col->rowCount()) > result)
			result = col_rows;
	return result;
}

void Spreadsheet::removeRows(int first, int count)
{
	if( count < 1 || first < 0 || first+count > rowCount()) return;
	WAIT_CURSOR;
	beginMacro(QObject::tr("%1: remove %2 row(s)").arg(name()).arg(count));
	foreach(Column * col, children<Column>(IncludeHidden))
	    col->removeRows(first, count);
	endMacro();
	RESET_CURSOR;
}

void Spreadsheet::insertRows(int before, int count)
{
	if( count < 1 || before < 0 || before > rowCount()) return;
	WAIT_CURSOR;
	beginMacro(QObject::tr("%1: insert %2 row(s)").arg(name()).arg(count));
	foreach(Column * col, children<Column>(IncludeHidden))
	    col->insertRows(before, count);
	endMacro();
	RESET_CURSOR;
}

void Spreadsheet::setRowCount(int new_size)
{
	int current_size = rowCount();
	if (new_size > current_size)
		insertRows(current_size, new_size-current_size);
	if (new_size < current_size && new_size >= 0)
		removeRows(new_size, current_size-new_size);
}

int Spreadsheet::columnCount(SciDAVis::PlotDesignation pd) const
{
	int count = 0;
	foreach(Column * col, children<Column>())
	    if (col->plotDesignation() == pd)
		count++;
	return count;
}

void Spreadsheet::removeColumns(int first, int count)
{
	if( count < 1 || first < 0 || first+count > columnCount()) return;
	WAIT_CURSOR;
	beginMacro(QObject::tr("%1: remove %2 column(s)").arg(name()).arg(count));
	for (int i=0; i<count; i++)
		child<Column>(first)->remove();
	endMacro();
	RESET_CURSOR;
}

void Spreadsheet::insertColumns(int before, int count)
{
	WAIT_CURSOR;
	beginMacro(QObject::tr("%1: insert %2 column(s)").arg(name()).arg(count));
	Column * before_col = column(before);
	int rows = rowCount();
	for (int i=0; i<count; i++) {
		Column * new_col = new Column(QString::number(i+1), SciDAVis::Numeric);
		new_col->setPlotDesignation(SciDAVis::Y);
		new_col->insertRows(0, rows);
		insertChildBefore(new_col, before_col);
	}
	endMacro();
	RESET_CURSOR;
}

void Spreadsheet::setColumnCount(int new_size)
{
	int old_size = columnCount();
	if ( old_size == new_size || new_size < 0 )
		return;

	if (new_size < old_size)
		removeColumns(new_size, old_size-new_size);
	else
		insertColumns(old_size, new_size-old_size);
}

void Spreadsheet::clear()
{
	WAIT_CURSOR;
	beginMacro(QObject::tr("%1: clear").arg(name()));
	foreach (Column * col, children<Column>())
		col->clear();
	endMacro();
	RESET_CURSOR;
}

void Spreadsheet::clearMasks()
{
	WAIT_CURSOR;
	beginMacro(QObject::tr("%1: clear all masks").arg(name()));
	foreach(Column * col, children<Column>())
	    col->clearMasks();
	endMacro();
	RESET_CURSOR;
}

QMenu *Spreadsheet::createContextMenu()
{
#ifdef ACTIVATE_SCIDAVIS_SPECIFIC_CODE
	QMenu *menu = AbstractPart::createContextMenu();
	Q_ASSERT(menu);
	emit requestProjectContextMenu(menu);
	return menu;
#else
	return new QMenu(0);
#endif
}

bool Spreadsheet::fillProjectMenu(QMenu * menu)
{
#ifdef ACTIVATE_SCIDAVIS_SPECIFIC_CODE
	bool rc = false;
	emit requestProjectMenu(menu, &rc);
	return rc;
#else
	return false;
#endif
}


void Spreadsheet::moveColumn(int from, int to)
{
	Column * col = child<Column>(from);
	beginMacro(tr("%1: move column %2 from position %3 to %4.").arg(name()).arg(col->name()).arg(from+1).arg(to+1));
	col->remove();
	insertChildBefore(col, child<Column>(to));
	endMacro();
}

void Spreadsheet::copy(Spreadsheet * other)
{
	WAIT_CURSOR;
	beginMacro(QObject::tr("%1: copy %2").arg(name()).arg(other->name()));

	foreach(Column * col, children<Column>())
		col->remove();
	foreach(Column * src_col, other->children<Column>()) {
		Column * new_col = new Column(src_col->name(), src_col->columnMode());
		new_col->copy(src_col);
		new_col->setPlotDesignation(src_col->plotDesignation());
		QList< Interval<int> > masks = src_col->maskedIntervals();
		foreach(Interval<int> iv, masks)
			new_col->setMasked(iv);
		QList< Interval<int> > formulas = src_col->formulaIntervals();
		foreach(Interval<int> iv, formulas)
			new_col->setFormula(iv, src_col->formula(iv.start()));
		new_col->setWidth(src_col->width());
		addChild(new_col);
	}
	setCaptionSpec(other->captionSpec());
	setComment(other->comment());

	endMacro();
	RESET_CURSOR;
}

int Spreadsheet::colX(int col)
{
	for(int i=col-1; i>=0; i--)
	{
		if (column(i)->plotDesignation() == SciDAVis::X)
			return i;
	}
	int cols = columnCount();
	for(int i=col+1; i<cols; i++)
	{
		if (column(i)->plotDesignation() == SciDAVis::X)
			return i;
	}
	return -1;
}

int Spreadsheet::colY(int col)
{
	int cols = columnCount();

	if (column(col)->plotDesignation() == SciDAVis::xErr || 
			column(col)->plotDesignation() == SciDAVis::yErr) {
		// look to the left first
		for(int i=col-1; i>=0; i--) {
			if (column(i)->plotDesignation() == SciDAVis::Y)
				return i;
		}
		for(int i=col+1; i<cols; i++) {
			if (column(i)->plotDesignation() == SciDAVis::Y)
				return i;
		}
	} else {
		// look to the right first
		for(int i=col+1; i<cols; i++) {
			if (column(i)->plotDesignation() == SciDAVis::Y)
				return i;
		}
		for(int i=col-1; i>=0; i--) {
			if (column(i)->plotDesignation() == SciDAVis::Y)
				return i;
		}
	}
	return -1;
}

void Spreadsheet::sortColumns(Column *leading, QList<Column*> cols, bool ascending)
{
	if(cols.isEmpty()) return;

	// the normal QPair comparison does not work properly with descending sorting
	// thefore we use our own compare functions
	class CompareFunctions {
		public:
			static bool doubleLess(const QPair<double, int>& a, const QPair<double, int>& b)
			{
				return a.first < b.first;
			}
			static bool doubleGreater(const QPair<double, int>& a, const QPair<double, int>& b)
			{
				return a.first > b.first;
			}
			static bool QStringLess(const QPair<QString, int>& a, const QPair<QString, int>& b)
			{
				return a < b;
			}
			static bool QStringGreater(const QPair<QString, int>& a, const QPair<QString, int>& b)
			{
				return a > b;
			}
			static bool QDateTimeLess(const QPair<QDateTime, int>& a, const QPair<QDateTime, int>& b)
			{
				return a < b;
			}
			static bool QDateTimeGreater(const QPair<QDateTime, int>& a, const QPair<QDateTime, int>& b)
			{
				return a > b;
			}
	};

	WAIT_CURSOR;
	beginMacro(tr("%1: sort column(s)").arg(name()));

	if(leading == 0) { // sort separately
		foreach(Column *col, cols) {
			switch (col->columnMode()) {
				case SciDAVis::Numeric:
					{
						int rows = col->rowCount();
						QList< QPair<double, int> > map;

						for(int j=0; j<rows; j++)
							map.append(QPair<double, int>(col->valueAt(j), j));

						if(ascending)
							qStableSort(map.begin(), map.end(), CompareFunctions::doubleLess);
						else
							qStableSort(map.begin(), map.end(), CompareFunctions::doubleGreater);

						QListIterator< QPair<double, int> > it(map);
						Column *temp_col = new Column("temp", col->columnMode());

						int k=0;
						// put the values in the right order into temp_col
						while(it.hasNext()) {
							temp_col->copy(col, it.peekNext().second, k, 1);
							temp_col->setMasked(col->isMasked(it.next().second));
							k++;
						}
						// copy the sorted column
						col->copy(temp_col, 0, 0, rows);
						delete temp_col;
						break;
					}
				case SciDAVis::Text:
					{
						int rows = col->rowCount();
						QList< QPair<QString, int> > map;

						for(int j=0; j<rows; j++)
							map.append(QPair<QString, int>(col->textAt(j), j));

						if(ascending)
							qStableSort(map.begin(), map.end(), CompareFunctions::QStringLess);
						else
							qStableSort(map.begin(), map.end(), CompareFunctions::QStringGreater);

						QListIterator< QPair<QString, int> > it(map);
						Column *temp_col = new Column("temp", col->columnMode());

						int k=0;
						// put the values in the right order into temp_col
						while(it.hasNext()) {
							temp_col->copy(col, it.peekNext().second, k, 1);
							temp_col->setMasked(col->isMasked(it.next().second));
							k++;
						}
						// copy the sorted column
						col->copy(temp_col, 0, 0, rows);
						delete temp_col;
						break;
					}
				case SciDAVis::DateTime:
				case SciDAVis::Month:
				case SciDAVis::Day:
					{
						int rows = col->rowCount();
						QList< QPair<QDateTime, int> > map;

						for(int j=0; j<rows; j++)
							map.append(QPair<QDateTime, int>(col->dateTimeAt(j), j));

						if(ascending)
							qStableSort(map.begin(), map.end(), CompareFunctions::QDateTimeLess);
						else
							qStableSort(map.begin(), map.end(), CompareFunctions::QDateTimeGreater);

						QListIterator< QPair<QDateTime, int> > it(map);
						Column *temp_col = new Column("temp", col->columnMode());

						int k=0;
						// put the values in the right order into temp_col
						while(it.hasNext()) {
							temp_col->copy(col, it.peekNext().second, k, 1);
							temp_col->setMasked(col->isMasked(it.next().second));
							k++;
						}
						// copy the sorted column
						col->copy(temp_col, 0, 0, rows);
						delete temp_col;
						break;
					}
			}
		}
	} else { // sort with leading column
		switch (leading->columnMode()) {
			case SciDAVis::Numeric:
				{
					QList< QPair<double, int> > map;
					int rows = leading->rowCount();

					for(int i=0; i<rows; i++)
						map.append(QPair<double, int>(leading->valueAt(i), i));

					if(ascending)
						qStableSort(map.begin(), map.end(), CompareFunctions::doubleLess);
					else
						qStableSort(map.begin(), map.end(), CompareFunctions::doubleGreater);
					QListIterator< QPair<double, int> > it(map);

					foreach (Column *col, cols) {
						Column *temp_col = new Column("temp", col->columnMode());
						it.toFront();
						int j=0;
						// put the values in the right order into temp_col
						while(it.hasNext()) {
							temp_col->copy(col, it.peekNext().second, j, 1);
							temp_col->setMasked(col->isMasked(it.next().second));
							j++;
						}
						// copy the sorted column
						col->copy(temp_col, 0, 0, rows);
						delete temp_col;
					}
					break;
				}
			case SciDAVis::Text:
				{
					QList< QPair<QString, int> > map;
					int rows = leading->rowCount();

					for(int i=0; i<rows; i++)
						map.append(QPair<QString, int>(leading->textAt(i), i));

					if(ascending)
						qStableSort(map.begin(), map.end(), CompareFunctions::QStringLess);
					else
						qStableSort(map.begin(), map.end(), CompareFunctions::QStringGreater);
					QListIterator< QPair<QString, int> > it(map);

					foreach (Column *col, cols) {
						Column *temp_col = new Column("temp", col->columnMode());
						it.toFront();
						int j=0;
						// put the values in the right order into temp_col
						while(it.hasNext()) {
							temp_col->copy(col, it.peekNext().second, j, 1);
							temp_col->setMasked(col->isMasked(it.next().second));
							j++;
						}
						// copy the sorted column
						col->copy(temp_col, 0, 0, rows);
						delete temp_col;
					}
					break;
				}
			case SciDAVis::DateTime:
			case SciDAVis::Month:
			case SciDAVis::Day:
				{
					QList< QPair<QDateTime, int> > map;
					int rows = leading->rowCount();

					for(int i=0; i<rows; i++)
						map.append(QPair<QDateTime, int>(leading->dateTimeAt(i), i));

					if(ascending)
						qStableSort(map.begin(), map.end(), CompareFunctions::QDateTimeLess);
					else
						qStableSort(map.begin(), map.end(), CompareFunctions::QDateTimeGreater);
					QListIterator< QPair<QDateTime, int> > it(map);

					foreach (Column *col, cols) {
						Column *temp_col = new Column("temp", col->columnMode());
						it.toFront();
						int j=0;
						// put the values in the right order into temp_col
						while(it.hasNext()) {
							temp_col->copy(col, it.peekNext().second, j, 1);
							temp_col->setMasked(col->isMasked(it.next().second));
							j++;
						}
						// copy the sorted column
						col->copy(temp_col, 0, 0, rows);
						delete temp_col;
					}
					break;
				}
		}
	}
	endMacro();
	RESET_CURSOR;
} // end of sortColumns()


QIcon Spreadsheet::icon() const
{
	QIcon ico;
#ifdef ACTIVATE_SCIDAVIS_SPECIFIC_CODE
	ico.addPixmap(QPixmap(":/16x16/table.png"));
	ico.addPixmap(QPixmap(":/24x24/table.png"));
	ico.addPixmap(QPixmap(":/32x32/table.png"));
#else
	ico = KIcon("x-office-spreadsheet");
#endif
	return ico;
}

QString Spreadsheet::text(int row, int col) const
{
	Column * col_ptr = column(col);
	if(!col_ptr)
		return QString();

	return col_ptr->asStringColumn()->textAt(row);
}
/* ========== loading and saving ============ */

void Spreadsheet::save(QXmlStreamWriter * writer) const
{
	writer->writeStartElement("spreadsheet");
	writeBasicAttributes(writer);
	writeCommentElement(writer);

	foreach (Column * col, children<Column>(IncludeHidden))
		col->save(writer);
	writer->writeEndElement(); // "spreadsheet"
}

bool Spreadsheet::load(XmlStreamReader * reader)
{
	if(reader->isStartElement() && reader->name() == "spreadsheet")
	{
		setColumnCount(0);
		setRowCount(0);
		setComment("");

		if (!readBasicAttributes(reader)) return false;

		// read child elements
		while (!reader->atEnd())
		{
			reader->readNext();

			if (reader->isEndElement()) break;

			if (reader->isStartElement())
			{
				if (reader->name() == "comment")
				{
					if (!readCommentElement(reader)) return false;
				}
				else if(reader->name() == "column")
				{
					Column * column = new Column(tr("Column %1").arg(1), SciDAVis::Text);
					if (!column->load(reader))
					{
						setColumnCount(0);
						return false;
					}
					addChild(column);
				}
				else // unknown element
				{
					reader->raiseWarning(tr("unknown element '%1'").arg(reader->name().toString()));
					if (!reader->skipToEndElement()) return false;
				}
			}
		}
	}
	else // no spreadsheet element
		reader->raiseError(tr("no spreadsheet element found"));

	return !reader->hasError();
}


/* ========== end of loading and saving ============ */
