/****************************************************************************
** ui.h extension file, included from the uic-generated form implementation.
**
** If you wish to add, delete or rename functions or slots use
** Qt Designer which will update this file, preserving your code. Create an
** init() function in place of a constructor, and a destroy() function in
** place of a destructor.
*****************************************************************************/
#include <qmessagebox.h>
#include <qfiledialog.h>
#include <qfile.h>
#include <qprogressdialog.h>

// neroconv (c)2004 Jonas Björk <jonas@trinix.se>
// Version: 1.0.20040412
//
// Licensed under GPL v2, with sole exception that license does not hold.
// 
// Thanks to: Alexander Schachtschabel, Torbjörn Svensson and of course my beloved wife Jessica.
//
int Form1::openConvert()
{
    int fail = 0;
    // inaktivera openandconvert-knappen
    pbtnOpen->setEnabled(FALSE);
    
    // vilken fil skall konverteras?
    QString filename = QFileDialog::getOpenFileName(
	    ".",
	    "Imagefiles (*.nrg *.bin *.iso)",
	    this,
	    "open file dialog",
	    "Choose a file");
    
    if(!filename)
    {
	QMessageBox::warning(this, "NRG2ISO converter",
			     "No file choosen\nConvertion aborted.");
	pbtnOpen->setEnabled(TRUE);
	return -1;
    }
    
    // kolla om den redan är en iso
    // $8000 01 43 44 30 30 31 01 00 = .CD001.
    
    char data[ 8 ];
    
    QFile file( filename );
    file.open( IO_ReadOnly );
    file.at( 32768 );
    file.readBlock(data, 8);
    file.close();
    
    if( data[0] == 1 && data[1] == 67 && data[2] == 68 && data[3] == 48 &&
	data[4] == 48 && data[5] == 49 && data[6] == 1 && data[7] == 0 )
    {
	QMessageBox::information(this, "NRG2ISO converter",
				 "The file is already an standard ISO9660 file\nConvertion aborted");
	pbtnOpen->setEnabled(TRUE);
	return -1;
    }
    	
    // konvertera
    char buff[1048576];		// en megabyte
    int filesize = 0, fNameLen = 0, pgress= 0;
    QString outFileName = filename;
    fNameLen = outFileName.length();
    outFileName.replace( fNameLen-3, 3, "iso" );
    
    QFile outFile( outFileName );
    
    file.open( IO_ReadOnly );
    outFile.open( IO_WriteOnly );
    
    file.at( 307200 );
    filesize = file.size()-307200;
    
    
    QProgressDialog pdia( "Converting file, please wait ...", "Abort", filesize,
                          this, "progress", TRUE );
    
    while( file.readBlock( buff, 1048576 ) > 0 )
    {
	outFile.writeBlock( buff, 1048576 );
	pgress = pgress + 1048576;
	pdia.setProgress(pgress);
	if ( pdia.wasCanceled() )
	{
	    fail = 1;
	    break;
	}
    }

    file.close();
    outFile.close();
    
    
    
    // meddela att konv är klar
    if(fail)
    {
	QMessageBox::warning(this, "NRG2ISO converter",
			     "The convertion was interrupted!");
    }
    else
    {
	QMessageBox::information(this, "NRG2ISO converter",
			     "The file converted successfully!");
    }
    // aktivera openandconvert-knappen
    pbtnOpen->setEnabled(TRUE);

    return 0;
 
}


