/************************************************************************/
/*                                                                      */
/*                Centre for Speech Technology Research                 */
/*                     University of Edinburgh, UK                      */
/*                       Copyright (c) 1996,1997                        */
/*                        All Rights Reserved.                          */
/*                                                                      */
/*  Permission is hereby granted, free of charge, to use and distribute */
/*  this software and its documentation without restriction, including  */
/*  without limitation the rights to use, copy, modify, merge, publish, */
/*  distribute, sublicense, and/or sell copies of this work, and to     */
/*  permit persons to whom this work is furnished to do so, subject to  */
/*  the following conditions:                                           */
/*   1. The code must retain the above copyright notice, this list of   */
/*      conditions and the following disclaimer.                        */
/*   2. Any modifications must be clearly marked as such.               */
/*   3. Original authors' names are not deleted.                        */
/*   4. The authors' names are not used to endorse or promote products  */
/*      derived from this software without specific prior written       */
/*      permission.                                                     */
/*                                                                      */
/*  THE UNIVERSITY OF EDINBURGH AND THE CONTRIBUTORS TO THIS WORK       */
/*  DISCLAIM ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING     */
/*  ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT  */
/*  SHALL THE UNIVERSITY OF EDINBURGH NOR THE CONTRIBUTORS BE LIABLE    */
/*  FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES   */
/*  WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN  */
/*  AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION,         */
/*  ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF      */
/*  THIS SOFTWARE.                                                      */
/*                                                                      */
/************************************************************************/
/*                 Author: Sergio Oller                                 */
/*                   Date: March 2014                                   */
/************************************************************************/
/*                                                                      */
/* Portable file I/O functions supporting large files                   */
/*                                                                      */
/* In order to provide large file support, we need to use an 8 byte     */
/* integer type at least. On POSIX systems this is usually done by      */
/* using off_t type to store ftell function return values and by        */
/* compiling with -D_FILE_OFFSET_BITS=64.                               */
/*                                                                      */
/* On MSVS2012 we can use __int64, _fseeki64, _ftelli64                 */
/*                                                                      */
/* For now and until a safe, common, standard and portable solution is  */
/* known, this file will be our portability layer among systems         */
/* I just hope this file to become useless some day                     */
/************************************************************************/


#ifndef EST_File_H_
#define EST_File_H_

#if defined(_WIN32) /* Win32 or Win64 */

typedef __int64 EST_FilePos;

inline EST_FilePos EST_ftell(FILE *fp) {
  return _ftelli64(fp);
}

inline int EST_fseek(FILE *fp, EST_FilePos offset, int whence) {
  return _fseeki64(fp, offset, whence);
}

#else

typedef off_t EST_FilePos;

inline EST_FilePos EST_ftell(FILE *fp) {
  return ftell(fp);
}

inline int EST_fseek(FILE *fp, EST_FilePos offset, int whence) {
  return fseek(fp, offset, whence);
}

#endif
#endif /* header guard */
