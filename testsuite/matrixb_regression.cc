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
/* Operations with matrices to test the implementation                  */
/*                                                                      */
/************************************************************************/

#include <cstdlib>
#include "EST_DMatrix.h"

using namespace std;

int main(void)
{
    EST_DMatrix a(3,4);
    EST_DMatrix b(4,2);
    EST_DMatrix c;

    a(0,0) = 1;
    a(0,1) = 1.5;
    a(0,2) = -2;
    a(0,3) = 1;

    a(1,0) = 1;
    a(1,1) = 1.5;
    a(1,2) = 0;
    a(1,3) = 3;

    a(2,0) = 1;
    a(2,1) = 0;
    a(2,2) = -2;
    a(2,3) = -1;

    b(0,0) = -1;
    b(0,1) = 0;

    b(1,0) = 2;
    b(1,1) = -1;

    b(1,0) = 0;
    b(1,1) = -3;

    b(2,0) = 1.3;
    b(2,1) = 0;
    b+=b;
    b-=(b-2*b);
    b*=3.0;
    cout << "Matrix A" << endl
         << a
         << "10*A" << endl
         << 10*a
         << "A*10" << endl
         << a*10
         << "A+A" << endl
         << a+a
         << "A-A" << endl
         << a-a
         << "Matrix B" << endl
         << b
         << "A*B" << endl
         << a*b
         << "4*A*B" << endl
         << 4*a*b
         << "End of test" << endl;
    return 0;
}


