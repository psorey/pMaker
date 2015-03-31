#define _CRT_SECURE_NO_DEPRECATE
#include "stdafx.h"     

#include "ReadDXF.h"

ReadDXF::ReadDXF( SoSeparator *_parent)
{
    coordIndexNum = 0;
    vertexNum = 0;
    parent = _parent;
    //theVP = new SoVertexProperty;
    theLines = new SoIndexedLineSet;
    theCoords = new SoCoordinate3;
    
}


CString 
ReadDXF::readFile() 
{
    lnct = 0;
    count = 0;
    int         n;
 
    BOOL        bAddFileDialog = TRUE;
    LPCTSTR     lpszFilter = NULL;
    LPCTSTR     lpszDefExt = "dxf";
    LPCTSTR     lpszFileName = "*.dxf";
    DWORD       dwFlags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;
    CWnd        *pParentWnd = NULL;
    
    CFileDialog addFileDialog(bAddFileDialog,
        lpszDefExt, lpszFileName, dwFlags,
        lpszFilter, pParentWnd);

    //addFileDialog.m_ofn.Flags |= OFN_ALLOWMULTISELECT;
    //addFileDialog.m_ofn.nMaxFile = 256;

    addFileDialog.m_ofn.lpstrTitle = LPCTSTR("select a DXF file... ");
    int nModal = addFileDialog.DoModal();
    CString m_strAddFile;

    if (nModal == IDOK) {
            m_strAddFile = addFileDialog.GetFileName();
            SetCurrentDirectory(addFileDialog.GetFolderPath());
            TRACE("filename = %s\n", LPCTSTR(m_strAddFile));
		    ifp = fopen( m_strAddFile, "r");
		    if(ifp == NULL){
		        TRACE("can't open %s\n", LPCTSTR(m_strAddFile));
                CString nullStr;
		        return nullStr;
	        }	    
	        char layer[] = "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0";

            /* find the beginning of the ENTITIES section */
	    
	        count = x1 = y1 = z1 = x2 = y2 = z2 = 0;           
	        while(strcmp((fgets(line,100,ifp)),"ENTITIES\n"))
		        lnct++;
	        while ((fgets(line,100,ifp)) != NULL ){
		        lnct++;		
		        if((strcmp( line, "LINE\n")) == 0 ){
			        getLINE();
		        }
		        if((strcmp( line, "LWPOLYLINE\n")) == 0 ){
                    getLWPOLYLINE();
		        }
                if((strcmp(line,"POLYLINE\n")) == 0) {
                    getPOLYLINE();
                }
	        }
            fclose(ifp);
        }
        //if((theVP->vertex.getNum() > 1) && parent != NULL) {
        if((theCoords->point.getNum() > 1) && parent != NULL) {
            //parent->addChild(theVP);
            parent->addChild(theCoords);
            parent->addChild(theLines);
        }
        //if((theVP->vertex.getNum() > 1) && parent == NULL) {
        char nwfile[50];
        if((theCoords->point.getNum() > 1) && parent == NULL) {
            SoSeparator *topSep = new SoSeparator;
            topSep->ref();
           // topSep->addChild(theVP);
            topSep->addChild(theCoords);
            topSep->addChild(theLines);
            for ( n = 1; n < 50; n++)
		        nwfile[n] = '\0';

	        strncpy( nwfile, m_strAddFile, (strcspn( m_strAddFile,"." )));
	        strcat( nwfile, ".iv");
	        TRACE( "\noutput filename is:  %s\n", nwfile );

            SoWriteAction wa;
            wa.getOutput()->openFile(nwfile);
            wa.apply(topSep);
            wa.getOutput()->closeFile();
            topSep->unref();        
        }
        CString returnString = CString(nwfile);
        return returnString;
}


void 
ReadDXF::get_group ( char group_number[5] )
{
	while ( strcmp (( fgets( line, 50, ifp)), group_number) != 0 ) 
		lnct++;
	get_xyz();
}

void 
ReadDXF::get_xyz( void )
{
	fscanf ( ifp, "%lf", &x );
    lnct++;
	fscanf ( ifp, "%f", &trash );
    lnct++;
	fscanf ( ifp, "%lf", &y );
    lnct++;
	fscanf ( ifp, "%f", &trash );
    lnct++;
	fscanf ( ifp, "%lf", &z );
    lnct++;
}

void
ReadDXF::getLINE(void)
{ 
        get_group( " 10\n" );
        x1=x;
        y1=y;
        z1=z;
        if (( x2 != x1) || (y2 != y1) || (z2 != z1 )) {
            theLines->coordIndex.set1Value(coordIndexNum++, -1);
            //theVP->vertex.set1Value(vertexNum++, (SbVec3f(x,y,z)));
            theCoords->point.set1Value(vertexNum++, (SbVec3f(x,y,z)));
            theLines->coordIndex.set1Value(coordIndexNum++, vertexNum-1);
        }

        get_group( " 11\n" );
        x2=x;
        y2=y;
        z2=z;

        //theVP->vertex.set1Value(vertexNum++, (SbVec3f(x,y,z)));
        theCoords->point.set1Value(vertexNum++, (SbVec3f(x,y,z)));
        theLines->coordIndex.set1Value(coordIndexNum++, vertexNum-1);
}

void
ReadDXF::getPOLYLINE(void)
{
    count = 0;
    while((fgets(line,100,ifp))!= NULL){
        lnct++;
        if((strcmp(line," 10\n")) == 0){
            fgets(line,100,ifp);
            lnct++;
            sscanf_s(line,"%lf", &x);
            fgets(line,100,ifp);
            lnct++;
            fgets(line,100,ifp);
            lnct++;
            sscanf_s(line,"%lf", &y);
            fgets(line,100,ifp);
            lnct++;
            fgets(line,100,ifp);
            lnct++;
            sscanf_s(line,"%lf", &z);
            if(count != 0) {
                //theVP->vertex.set1Value(vertexNum++, (SbVec3f(x,y,z)));
                theCoords->point.set1Value(vertexNum++, (SbVec3f(x,y,z)));
                theLines->coordIndex.set1Value(coordIndexNum++, vertexNum-1);
                count++;
            }
            else count++;
        }
        if((strcmp(line,"SEQEND\n")) == 0) {
           // theVP->vertex.set1Value(vertexNum++, (SbVec3f(x,y,z)));
            theCoords->point.set1Value(vertexNum++, (SbVec3f(x,y,z)));
            theLines->coordIndex.set1Value(coordIndexNum++, vertexNum-1);
            theLines->coordIndex.set1Value(coordIndexNum++, -1);
            count--;
            return;
        }
    }
}

void
ReadDXF::getLWPOLYLINE(void)
{
    SoMFVec3f coords;
    SbVec3f   az;
    z=0.0;
    int closed = 0;
    int acount = 0;
    while((fgets(line,100,ifp))!= NULL){
        lnct++;
        //get elevation...
        if((strcmp(line, " 70\n")) == 0) {
            fgets(line,100,ifp);
            lnct++;
            sscanf_s(line,"%d", &closed);
            if(closed == 1 || closed == 129)
                closed = 1;
            else closed = 0;
            continue;
        }
		if((strcmp( line, " 38\n")) == 0 ){
            fgets(line,100,ifp);
            lnct++;
            sscanf_s(line,"%lf", &z);
            continue;
        }
        // get x and y coordinates...
        if((strcmp(line," 10\n")) == 0){
            //TRACE("found 10\n");
            SbVec3f temp;
            fgets(line,100,ifp);
            lnct++;
            sscanf_s(line,"%lf", &x);
            temp[0] = x;
            fgets(line,100,ifp);
            lnct++;
            if((strcmp(line," 20\n")) != 0) continue;
            fgets(line,100,ifp);
            lnct++;
            sscanf_s(line,"%lf", &y);
            temp[1] = y;
            temp[2] = z;
            coords.set1Value(acount++, temp);
            continue;
        }
        // get ocs z-axis
        if((strcmp(line,"210\n")) == 0){
            fgets(line,100,ifp);
            lnct++;
            sscanf_s(line,"%lf", &x);
            az[0] = x;
            fgets(line,100,ifp);
            lnct++;
            if((strcmp(line,"220\n")) != 0) continue;
            fgets(line,100,ifp);
            lnct++;
            sscanf_s(line,"%lf", &y);
            az[1] = y;
            fgets(line,100,ifp);
            lnct++;
            fgets(line,100,ifp);
            lnct++;
            sscanf_s(line,"%lf", &z);
            az[2] = z;

            int j=coords.getNum();            
            SbVec3f result;
            for (int i=0; i<j; i++) {
                result = getWorld(coords[i],az);
                //theVP->vertex.set1Value(vertexNum++, result[0],result[1],result[2]);
                theCoords->point.set1Value(vertexNum++, result[0],result[1],result[2]);
                theLines->coordIndex.set1Value(coordIndexNum++, vertexNum-1);
                count++;
            }

            if(!closed) {
                theLines->coordIndex.set1Value(coordIndexNum++, -1);
            }
            else {
                int k;
                int newValue;
                for(k=coordIndexNum; k>=0; k--) {
                    if (*theLines->coordIndex.getValues(k) == -1) {
                        newValue = *theLines->coordIndex.getValues(k+1);
                        k=-1;
                    }
                    if(k==0) newValue = *theLines->coordIndex.getValues(k);
                }
                theLines->coordIndex.set1Value(coordIndexNum++, newValue);
                theLines->coordIndex.set1Value(coordIndexNum++, -1);
            }
            return;
        }
        if((strcmp(line,"  0\n")) == 0){
            int j=coords.getNum();            
            SbVec3f result;
            for (int i=0; i<j; i++) {
                result = getWorld(coords[i],az);
                //theVP->vertex.set1Value(vertexNum++, coords[i][0],coords[i][1],coords[i][2]);
                theCoords->point.set1Value(vertexNum++, coords[i][0],coords[i][1],coords[i][2]);
                theLines->coordIndex.set1Value(coordIndexNum++, vertexNum-1);
            }

            if(!closed) {
                theLines->coordIndex.set1Value(coordIndexNum++, -1);
            }
            else {
                int k;
                int newValue;
                for(k=coordIndexNum; k>=0; k--) {
                    if (*theLines->coordIndex.getValues(k) == -1) {
                        newValue = *theLines->coordIndex.getValues(k+1);
                        k=-1;
                    }
                    if(k==0) newValue = *theLines->coordIndex.getValues(k);
                }
                theLines->coordIndex.set1Value(coordIndexNum++, newValue);
                theLines->coordIndex.set1Value(coordIndexNum++, -1);
            }
            return;
       }
    }
    return;
}

SbVec3f
ReadDXF::getWorld(SbVec3f pointVector /*point to transform*/, SbVec3f az /*arbitrary z-axis*/)
{

    //find arbitrary x-axis...
    SbVec3f ax;
    SbVec3f wx = SbVec3f(1.0,0.0,0.0);       //world x
    SbVec3f wy = SbVec3f(0.0,1.0,0.0);       //world y
    SbVec3f wz = SbVec3f(0.0,0.0,1.0);       //world z
    if((fabs(az[0])) < .015625 && (fabs(az[1])) < .015625) {
        ax = wy.cross(az);
        //fprintf(ofp,"near z\n");
    }
    else {
        ax = wz.cross(az);
    }
    ax.normalize();

    //find arbitrary y-axis...
    SbVec3f ay = az.cross(ax);
    ay.normalize();
    
    //make the transformation matrix and multiply it by input point...
    SbVec3f  sum;
    float m[4][4] = {ax[0],ay[0],az[0], 0,
                     ax[1],ay[1],az[1], 0,
                     ax[2],ay[2],az[2], 0,
                         0,    0,    0, 1};
    int disp = 0;
    SbVec3f bp = pointVector;
    SbVec3f ap;
    for (int i=0; i<3; i++)
        sum[i] = m[i][0]*bp[0]+m[i][1]*bp[1]+m[i][2]*bp[2]
              +(disp ? 0.0 : m[i][3]);
    for (int i=0; i<3; i++) ap[i] = sum[i];
    //TRACE("%f  %f  %f\n", ap[0],ap[1],ap[2]);
    return ap;
}
