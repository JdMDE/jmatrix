/*
 *
 * Copyright (C) 2022 Juan Domingo (Juan.Domingo@uv.es)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "../headers/fullmatrix.h"
#include "../headers/templatemacros.h"

extern unsigned char DEB;

/****************************************
  TEMPLATED CONSTRUCTORS AND FUNCTIONS
*****************************************/

template <typename T>
FullMatrix<T>::FullMatrix() : JMatrix<T>(MTYPEFULL)
{
 data=nullptr;
}

TEMPLATES_CONST(FullMatrix,)

/////////////////////////////////////////////////////////

template <typename T>
FullMatrix<T>::FullMatrix(indextype nrows,indextype ncols) : JMatrix<T>(MTYPEFULL,nrows,ncols)
{
 data = new T* [this->nr];
 for (unsigned long r=0;r<this->nr;r++)
 {
     data[r] = new T[this->nc];
     for (unsigned long c=0;c<this->nc;c++)
         data[r][c]=0;
 }
}

TEMPLATES_CONST(FullMatrix,SINGLE_ARG(indextype nrows,indextype ncols))

/////////////////////////////////////////////////////////

template <typename T>
FullMatrix<T>::FullMatrix(indextype nrows,indextype ncols,bool warn) : JMatrix<T>(MTYPEFULL,nrows,ncols)
{
 if (warn)
  MemoryWarnings(nrows,ncols,sizeof(T));
 data = new T* [this->nr];
 for (unsigned long r=0;r<this->nr;r++)
 {
     data[r] = new T[this->nc];
     for (unsigned long c=0;c<this->nc;c++)
         data[r][c]=0;
 }
}

TEMPLATES_CONST(FullMatrix,SINGLE_ARG(indextype nrows,indextype ncols, bool warn))

//////////////////////////////////////////////////////////////////

template <typename T>
FullMatrix<T>::FullMatrix(const FullMatrix<T>& other) : JMatrix<T>(other)
{
 data = new T* [this->nr];
 for (unsigned long r=0;r<this->nr;r++)
 {
     data[r] = new T[this->nc];
     for (unsigned long c=0;c<this->nc;c++)
         data[r][c]=other.data[r][c];
 }
}

TEMPLATES_COPY_CONST(FullMatrix)

//////////////////////////////////////////////////////////////////

template <typename T>
void FullMatrix<T>::Resize(indextype newnr,indextype newnc)
{
   if (data != nullptr)
   {
    if (this->nr!=0)
    {
     if (this->nc!=0)
      for (unsigned long r=0;r<this->nr;r++)
       delete[] data[r];
     delete[] data;
    }
   }
   
   ((JMatrix<T> *)this)->Resize(newnr,newnc);
   
   if (DEB & DEBJM)
    std::cout << "Full matrix resized to (" << this->nr << "," << this->nc << ")\n";
   
   data = new T* [this->nr];
   for (unsigned long r=0;r<this->nr;r++)
   {
     data[r] = new T[this->nc];
     for (unsigned long c=0;c<this->nc;c++)
         data[r][c]=T(0);
   }
}

TEMPLATES_FUNC(void,FullMatrix,Resize,SINGLE_ARG(indextype newnr,indextype newnc))

//////////////////////////////////////////////////////////////////

template <typename T>
FullMatrix<T>::~FullMatrix()
{
 if (this->nr!=0)
 {
  if (this->nc!=0)
   for (unsigned long r=0;r<this->nr;r++)
      delete[] data[r];
  delete[] data;
 }
}

TEMPLATES_DEFAULT_DEST(FullMatrix)

//////////////////////////////////////////////////////////////////

// Constructor to read from a binary file
template <typename T>
FullMatrix<T>::FullMatrix(std::string fname) : JMatrix<T>(fname,MTYPEFULL)
{
    data = new T* [this->nr];
    for (indextype r=0;r<this->nr;r++)
        data[r] = new T[this->nc];

    for (indextype r=0;r<this->nr;r++)  	
        this->ifile.read((char *)data[r],this->nc*sizeof(T));
      
    this->ReadMetadata();                  // This is exclusively used when reading from a binary file, not from a csv file
      
    this->ifile.close();
}

TEMPLATES_CONST(FullMatrix,std::string fname)

//////////////////////////////////////////////////////////////////

// Constructor to read from a binary file with warnings
template <typename T>
FullMatrix<T>::FullMatrix(std::string fname, bool warn) : JMatrix<T>(fname,MTYPEFULL)
{
    if (warn)
     MemoryWarnings(this->nr,this->nc,sizeof(T));
    data = new T* [this->nr];
    for (indextype r=0;r<this->nr;r++)
        data[r] = new T[this->nc];

    for (indextype r=0;r<this->nr;r++)
        this->ifile.read((char *)data[r],this->nc*sizeof(T));

    this->ReadMetadata();                  // This is exclusively used when reading from a binary file, not from a csv file

    this->ifile.close();
}

TEMPLATES_CONST_WITH_ARG(FullMatrix,std::string fname,bool warn)

//////////////////////////////////////////////////////////////////

template <typename T>
FullMatrix<T>& FullMatrix<T>::operator=(const FullMatrix<T>& other)
{
 if (data != nullptr)
 {
  if (this->nr!=0)
  {
   if (this->nc!=0)
    for (unsigned long r=0;r<this->nr;r++)
     delete[] data[r];
   delete[] data;
  }
 }
 
 ((JMatrix<T> *)this)->operator=((const JMatrix<T> &)other);
 
 data = new T* [this->nr];
 for (unsigned long r=0;r<this->nr;r++)
 {
     data[r] = new T[this->nc];
     for (unsigned long c=0;c<this->nc;c++)
         data[r][c]=other.data[r][c];
 }
 
 return *this;
}

TEMPLATES_OPERATOR(FullMatrix,=)

//////////////////////////////////////////////////////////////////

template <typename T>
FullMatrix<T>& FullMatrix<T>::operator!=(const FullMatrix<T>& other)
{
 if (data != nullptr)
 {
  if (this->nr!=0)
  {
   if (this->nc!=0)
    for (unsigned long r=0;r<this->nr;r++)
     delete[] data[r];
   delete[] data;
  }
 }
 
 ((JMatrix<T> *)this)->operator!=((const JMatrix<T> &)other);
 
 data = new T* [this->nr];
 for (unsigned long r=0;r<this->nr;r++)
     data[r] = new T[this->nc];
 
 for (unsigned long r=0;r<other.nr;r++)
     for (unsigned long c=0;c<other.nc;c++)
         data[c][r]=other.data[r][c];
 
 return *this;
}

TEMPLATES_OPERATOR(FullMatrix,!=)

//////////////////////////////////////////////////////////////////

// Constructor to read from a csv file
template <typename T>
FullMatrix<T>::FullMatrix(std::string fname,unsigned char vtype,char csep) : JMatrix<T>(fname,MTYPEFULL,vtype,csep)
{
    std::string line;
    // This is just to know number of rows
    this->nr=0;
    while (!this->ifile.eof())
    {
        getline(this->ifile,line);
        if (!this->ifile.eof())
            this->nr++;
    }
    if (DEB & DEBJM)
    {
        std::cout << this->nr << " lines (excluding header) in file " << fname << std::endl;
        std::cout << "Data will be read from each line and stored as ";
        switch (vtype)
        {
         case ULTYPE: std::cout << "unsigned 32-bit integers.\n"; break;
         case FTYPE: std::cout << "float values.\n"; break;
         case DTYPE: std::cout << "double values.\n"; break;
         default: std::cout << "unknown type values??? (Is this an error?).\n"; break;
        }
    }
    
    data = new T* [this->nr];
    for (indextype r=0;r<this->nr;r++)
        data[r] = new T [this->nc];
    
    // Reposition pointer at the begin and re-read first (header) line
    // and no, seekg does not work (possibly, because we had reached the eof ???)
    this->ifile.close();
    this->ifile.open(fname.c_str());
    size_t p=0;
    getline(this->ifile,line);
    // No need to process first line here, it was done at the parent's class constructor

    while (!this->ifile.eof())
    {
        getline(this->ifile,line);
        if (!this->ifile.eof())
        {
          if (!this->ProcessDataLineCsv(line,csep,data[p]))
          {
              std::ostringstream errst;
              errst << "Format error reading line " << p << " of file " << fname << ".\n";
              JMatrixStop(errst.str());
          }
          p++;
          
          //if ( (DEB & DEBJM) && (this->nr>1000) && (!(p % 100)) )
          // std::cout << p << " ";
        }
    }
    
    if (DEB & DEBJM)
    {
        std::cout << "Read " << p << " data lines of file " << fname;
        if (p==this->nr)
            std::cout << ", as expected.\n";
        else
            std::cout << " instead of " << this->nr << ".\n";
    }
    
    // No call to ReadMetadata must be done here, since these data are NOT binary. The column names were read by the parent's class constructor
    // and the row names are stored as they are read by the former call to ProcessDataLine
    
    this->ifile.close();  
}

TEMPLATES_CONST(FullMatrix,SINGLE_ARG(std::string fname,unsigned char vtype,char csep))

//////////////////////////////////////////////////////////////////

template <typename T>
inline T FullMatrix<T>::Get(indextype r,indextype c)
{ 
#ifdef WITH_CHECKS_MATRIX
    if ((r>=this->nr) || (c>=this->nc))
    {
    	std::ostringstream errst;
        errst << "Runtime error in FullMatrix<T>::Get: at least one index (" << r << " or " << c << ") out of bounds.\n";
        errst << "This matrix was of dimension (" << this->nr << " x " << this->nc << ")\n";
        JMatrixStop(errst.str());
    }
#endif
    return data[r][c];
}

TEMPLATES_FUNCR(FullMatrix,Get,SINGLE_ARG(indextype r,indextype c))

///////////////////////////////////////////////////////////////////////////////

template <typename T>
inline void FullMatrix<T>::Set(indextype r,indextype c,T v)
{
#ifdef WITH_CHECKS_MATRIX
    if ((r>=this->nr) || (c>=this->nc))
    {
        std::ostringstream errst;
        errst << "Runtime error in FullMatrix<T>::Set: at least one index (" << r << " or " << c << ") out of bounds.\n";
        errst << "This matrix was of dimension (" << this->nr << " x " << this->nc << ")\n";
        JMatrixStop(errst.str());
    }
#endif
    data[r][c]=v;
}

TEMPLATES_SETFUNC(void,FullMatrix,Set,SINGLE_ARG(indextype r,indextype c),v)

///////////////////////////////////////////////////////////////////////////////////////////////////////////

template <typename T>
void FullMatrix<T>::GetRow(indextype r,T *v)
{
#ifdef WITH_CHECKS_MATRIX
    if (r>=this->nr)
    {
    	std::ostringstream errst;
        errst << "Runtime error in FullMatrix<T>::GetRow: the row index " << r << " is out of bounds.\n";
        errst << "This matrix was of dimension (" << this->nr << " x " << this->nc << ")\n";
        JMatrixStop(errst.str());
    }
#endif
 // Fill the positions in v which are not zero.
 for (indextype c=0;c<this->nc;c++)
     v[c]=data[r][c];
}

TEMPLATES_SETFUNC(void,FullMatrix,GetRow,indextype r,*v)

///////////////////////////////////////////////////////////////////////////////////////////////////////////

template <typename T>
void FullMatrix<T>::GetFullRow(indextype r,unsigned char *m,unsigned char s,T *v)
{
#ifdef WITH_CHECKS_MATRIX
    if (r>=this->nr)
    {
        std::ostringstream errst;
        errst << "Runtime error in FullMatrix<T>::GetFullRow: the row index " << r << " is out of bounds.\n";
        errst << "This matrix was of dimension (" << this->nr << " x " << this->nc << ")\n";
        JMatrixStop(errst.str());
    }
#endif
  // Fill the positions in v which are not zero and also sum the value s to those positions in array m
  for (indextype c=0;c<this->nc;c++)
  {
     if (data[r][c]!=T(0))
     {
      v[c]=data[r][c];  
      m[c] |= s;
     }
  }
}

TEMPLATES_SETFUNC(void,FullMatrix,GetFullRow,SINGLE_ARG(indextype r,unsigned char *m,unsigned char s),*v)

////////////////////////////////////////////////////////////////////////////////////////////////////////

template <typename T>
void FullMatrix<T>::GetMarksOfFullRow(indextype r,unsigned char *m,unsigned char s)
{
#ifdef WITH_CHECKS_MATRIX
    if (r>=this->nr)
    {
    	std::ostringstream errst;
        errst << "Runtime error in FullMatrix<T>::GetMarksOfFullRow: the row index " << r << " is out of bounds.\n";
        errst << "This matrix was of dimension (" << this->nr << " x " << this->nc << ")\n";
        JMatrixStop(errst.str());
    }
#endif
  for (indextype c=0;c<this->nc;c++)
   if (data[r][c]!=T(0))
     m[c] |= s;
}

TEMPLATES_FUNC(void,FullMatrix,GetMarksOfFullRow,SINGLE_ARG(indextype r,unsigned char *m,unsigned char s))

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <typename T>
void FullMatrix<T>::SelfRowNorm(std::string ctype)
{
 if (DEB & DEBJM)
  std::cout << "Normalizing... ";
  
 if ((ctype=="log1") || (ctype=="log1n"))
 {
  for (indextype r=0;r<this->nr;r++)
   for (indextype c=0;c<this->nc;c++)
    data[r][c] = log2(data[r][c]+1.0);
 }
 
 if (ctype=="log1")
 {
  if (DEB & DEBJM)
   std::cout << "done!\n";
  return;
 }
  
 // Here we are either in the rawn or in the log1n case 
 T sum;
 for (indextype r=0;r<this->nr;r++)
 {
  sum=T(0);
  for (indextype c=0;c<this->nc;c++)
   sum+=data[r][c];

  if (sum!=T(0))
   for (indextype c=0;c<this->nc;c++)
    data[r][c] /= sum;
 }
 if (DEB & DEBJM)
   std::cout << "done!\n";
}

TEMPLATES_FUNC(void,FullMatrix,SelfRowNorm,std::string ctype)

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <typename T>
void FullMatrix<T>::SelfColNorm(std::string ctype)
{
 if ((ctype=="log1") || (ctype=="log1n"))
 {
  for (indextype r=0;r<this->nr;r++)
   for (indextype c=0;c<this->nc;c++)
    data[r][c] = log2(data[r][c]+1.0);
 }
   
 if (ctype=="log1")
  return;
 
 // Here we are either in the rawn or in the log1n case    
 T sum;
 for (indextype c=0;c<this->nc;c++)
 {
  sum=T(0);
  for (indextype r=0;r<this->nr;r++)
   sum+=data[r][c];

  if (sum!=T(0))
   for (indextype r=0;r<this->nr;r++)
    data[r][c] /= sum;
 }
}

TEMPLATES_FUNC(void,FullMatrix,SelfColNorm,std::string ctype)


////////////////////////////////////////////////////////////////////////////////////////////////////////

template <typename T>
void FullMatrix<T>::WriteBin(std::string fname)
{
    ((JMatrix<T> *)this)->WriteBin(fname,MTYPEFULL);
    
    if (DEB & DEBJM)
    {
     std::cout << "Writing binary matrix " << fname << " of (" << this->nr << "x" << this->nc << ")\n";
     std::cout.flush();
    }
    
    for (unsigned long r=0;r<this->nr;r++)
        this->ofile.write((const char *)data[r],this->nc*sizeof(T));
    
    unsigned long long endofbindata = this->ofile.tellp();
    
    if (DEB & DEBJM)
     std::cout << "End of block of binary data at offset " << endofbindata << "\n";
     
    this->WriteMetadata();                // Here we must write the metadata at the end of the binary contents of the matrix
    
    this->ofile.write((const char *)&endofbindata,sizeof(unsigned long long));  // This writes the point where binary data ends at the end of the file
    
    this->ofile.close();
}

TEMPLATES_FUNC(void,FullMatrix,WriteBin,std::string fname)

////////////////////////////////////////////////////////////////////////////////////////////////////////

template <typename T>
void FullMatrix<T>::WriteCsv(std::string fname,char csep,bool withquotes)
{
    ((JMatrix<T> *)this)->WriteCsv(fname,csep,withquotes);
    
    bool with_row_headers=true;
    bool with_col_headers=true;
    size_t nch=this->colnames.size();
    size_t nrh=this->rownames.size();
    
    if (nch==0)
     with_col_headers=false;
    if (nch>0 && nch!=this->nc)
    {
     JMatrixWarning("Different size of column headers and matrix. Column Headers will not be written in the .csv file.\n");
     with_col_headers=false;
    }
    if (nrh==0)
     with_row_headers=false;
    if (nrh>0 && nrh!=this->nr)
    {
     JMatrixWarning("Different size of row headers and matrix. Column Headers will not be written in the .csv file.\n");
     with_row_headers=false;
    }
    
    for (indextype r=0;r<this->nr;r++)
    {
        if (with_col_headers)
        {
         if (withquotes)
           this->ofile << "\"\"" << csep;
         else
           this->ofile << csep;   // Blank empty field at the beginning of each line
        }
        
        if (with_row_headers)
            this->ofile << FixQuotes(this->rownames[r],withquotes) << csep;
            
        for (indextype c=0;c<this->nc-1;c++)
            this->ofile << ((data[r][c]<1E-10) ? 0 : data[r][c] ) << csep;
        this->ofile << ((data[r][this->nc-1]<1E-10) ? 0 : data[r][this->nc-1] ) << std::endl; 
    }
    this->ofile.close();
}

TEMPLATES_FUNC(void,FullMatrix,WriteCsv,SINGLE_ARG(std::string fname,char csep,bool withquotes))

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <typename T>
float FullMatrix<T>::GetUsedMemoryMB()
{
    unsigned long long num_elem=(unsigned long long)this->nr*(unsigned long long)this->nc;
    std::cout << num_elem << " elements of " << sizeof(T) << " bytes each with accounts for ";
    return float(num_elem)*float(sizeof(T))/(1024.0*1024.0);
}

TEMPLATES_FUNC(float,FullMatrix,GetUsedMemoryMB,)


