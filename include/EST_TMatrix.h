 /*************************************************************************/
 /*                                                                       */
 /*                Centre for Speech Technology Research                  */
 /*                     University of Edinburgh, UK                       */
 /*                         Copyright (c) 1996                            */
 /*                        All Rights Reserved.                           */
 /*                                                                       */
 /*  Permission is hereby granted, free of charge, to use and distribute  */
 /*  this software and its documentation without restriction, including   */
 /*  without limitation the rights to use, copy, modify, merge, publish,  */
 /*  distribute, sublicense, and/or sell copies of this work, and to      */
 /*  permit persons to whom this work is furnished to do so, subject to   */
 /*  the following conditions:                                            */
 /*   1. The code must retain the above copyright notice, this list of    */
 /*      conditions and the following disclaimer.                         */
 /*   2. Any modifications must be clearly marked as such.                */
 /*   3. Original authors' names are not deleted.                         */
 /*   4. The authors' names are not used to endorse or promote products   */
 /*      derived from this software without specific prior written        */
 /*      permission.                                                      */
 /*                                                                       */
 /*  THE UNIVERSITY OF EDINBURGH AND THE CONTRIBUTORS TO THIS WORK        */
 /*  DISCLAIM ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING      */
 /*  ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT   */
 /*  SHALL THE UNIVERSITY OF EDINBURGH NOR THE CONTRIBUTORS BE LIABLE     */
 /*  FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES    */
 /*  WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN   */
 /*  AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION,          */
 /*  ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF       */
 /*  THIS SOFTWARE.                                                       */
 /*                                                                       */
 /*************************************************************************/
 /*                                                                       */
 /*                     Author :  Paul Taylor                             */
 /*                  Rewritten :  Richard Caley                           */
 /* --------------------------------------------------------------------- */
 /*                         Matrix class                                  */
 /*                                                                       */
 /*************************************************************************/

#ifndef __TMatrix_H__
#define __TMatrix_H__

#include <iostream>
#include <cstddef>

#include "EST_rw_status.h"
#include "EST_TVector.h"
#include "instantiate/EST_TMatrixI.h"

/* When set bounds checks (safe but slow) are done on matrix access */
#ifndef TMATRIX_BOUNDS_CHECKING
#    define TMATRIX_BOUNDS_CHECKING 0
#endif

#if TMATRIX_BOUNDS_CHECKING
#define A_CHECK a_check
#else
#define A_CHECK a_no_check
#endif

#define INLINE inline 

/* This doesn't work as I thought so I have disabled it for now.
 */

#if defined(__GNUC__) && 0
#    define mx_move_pointer(P, TY, STEP, N) \
            ((TY *)\
	     ((void *) (((char (*) [sizeof(TY)*STEP])P) + N) ) \
	      )
#    define fast_a_m_gcc(R,C) \
	( * mx_move_pointer(mx_move_pointer(p_memory,T,p_column_step,C),T,p_row_step,R))
#    define fast_a_m_x(R,C) (fast_a_m_gcc(R,C))
#else
#    define fast_a_m_x(R,C) (fast_a_m(R,C))
#endif



/** \class EST_TMatrix
  * \ingroup containerclasses
  * \brief Template Matrix class. This is an extension of the EST_TVector class to two dimensions.
  *
  * @see matrix_example
  * @see EST_TVector
  */
template <class T> 
class EST_TMatrix : public EST_TVector<T>
{

protected:
  /// Visible shape
  ssize_t p_num_rows; 
  
  /// How to access the memory
  ssize_t p_row_step;

  INLINE ssize_t mcell_pos(ssize_t r, ssize_t c,
			       ssize_t rs, ssize_t cs) const
    { return (rs==1?r:(r*rs)) + (cs==1?c:(c*cs));}


  INLINE ssize_t mcell_pos(ssize_t r, ssize_t c) const
    {

      return mcell_pos(r, c, 
		       this->p_row_step, this->p_column_step);
    }

  INLINE ssize_t mcell_pos_1(ssize_t r, ssize_t c) const
    {

      (void)r;
      return c;
    }

  /// quick method for returning `x[m][n]`
  INLINE const T &fast_a_m(ssize_t r, ssize_t c) const 
    { return this->p_memory[mcell_pos(r,c)]; }
  INLINE T &fast_a_m(ssize_t r, ssize_t c) 
    { return this->p_memory[mcell_pos(r,c)]; }

  INLINE const T &fast_a_1(ssize_t r, ssize_t c) const 
    { return this->p_memory[mcell_pos_1(r,c)]; }
  INLINE T &fast_a_1(ssize_t r, ssize_t c) 
    { return this->p_memory[mcell_pos_1(r,c)]; }
  

    /// Get and set values from array
  void set_values(const T *data, 
		  ssize_t r_step, ssize_t c_step,
		  ssize_t start_r, ssize_t num_r,
		  ssize_t start_c, ssize_t num_c
		  );
  void get_values(T *data, 
		  ssize_t r_step, ssize_t c_step,
		  ssize_t start_r, ssize_t num_r,
		  ssize_t start_c, ssize_t num_c
		  ) const;

  /// private resize and copy function. 
  void copy(const EST_TMatrix<T> &a);
  /// just copy data, no resizing, no size check.
  void copy_data(const EST_TMatrix<T> &a); 

  /// resize the memory and reset the bounds, but don't set values.
  void just_resize(ssize_t new_rows, ssize_t new_cols, T** old_vals);

  /// sets data and length to default values (0 in both cases).
  void default_vals();
public:

  ///default constructor
  EST_TMatrix(); 

  /// copy constructor
  EST_TMatrix(const EST_TMatrix<T> &m); 

  /// "size" constructor
  EST_TMatrix(ssize_t rows, ssize_t cols); 

  /// construct from memory supplied by caller
  EST_TMatrix(ssize_t rows, ssize_t cols, 
	      T *memory, ptrdiff_t offset=0, int free_when_destroyed=0);

  /// EST_TMatrix

  ~EST_TMatrix();

  /**@name access
    * Basic access methods for matrices.
    */
  //@{

  /// return number of rows
  ssize_t num_rows() const {return this->p_num_rows;}
  /// return number of columns
  ssize_t num_columns() const {return this->p_num_columns;}

  /// const access with no bounds check, care recommend
  INLINE const T &a_no_check(ssize_t row, ssize_t col) const 
    { return fast_a_m_x(row,col); }
  /// access with no bounds check, care recommend
  INLINE T &a_no_check(ssize_t row, ssize_t col) 
    { return fast_a_m_x(row,col); }

  INLINE const T &a_no_check_1(ssize_t row, ssize_t col) const { return fast_a_1(row,col); }
  INLINE T &a_no_check_1(ssize_t row, ssize_t col) { return fast_a_1(row,col); }

  /// const element access function 
  const T &a_check(ssize_t row, ssize_t col) const;
  /// non-const element access function 
  T &a_check(ssize_t row, ssize_t col);

  const T &a(ssize_t row, ssize_t col) const { return A_CHECK(row,col); }
  T &a(ssize_t row, ssize_t col) { return A_CHECK(row,col); }

  /// const element access operator
  const T &operator () (ssize_t row, ssize_t col) const { return a(row,col); }
  /// non-const element access operator
  T &operator () (ssize_t row, ssize_t col) { return a(row,col); }
  
  //@}

  bool have_rows_before(ssize_t n) const;
  bool have_columns_before(ssize_t n) const;

  /** resize matrix. If `set=1`, then the current values in
      the matrix are preserved up to the new size `n`. If the
      new size exceeds the old size, the rest of the matrix is
      filled with the `def_val`
  */
    void resize(ssize_t rows, ssize_t cols, ssize_t set=1); 

  /// fill matrix with value v
  void fill(const T &v);
  void fill() { fill(*this->def_val); }

  /// assignment operator
  EST_TMatrix &operator=(const EST_TMatrix &s); 

  /// The two versions of what might have been operator +=
  EST_TMatrix &add_rows(const EST_TMatrix &s); 
  EST_TMatrix &add_columns(const EST_TMatrix &s); 

  /**@name Sub-Matrix/Vector Extraction
    *
    * All of these return matrices and vectors which share
    * memory with the original, so altering values them alters
    * the original. 
    */
  //@{

  /// Make the vector `rv` a window onto row `r`
  void row(EST_TVector<T> &rv, ssize_t r, ssize_t start_c=0, int len=-1);
  /// Make the vector `cv` a window onto column `c`
  void column(EST_TVector<T> &cv, ssize_t c, ssize_t start_r=0, int len=-1);
  /// Make the matrix `sm` a window into this matrix.
  void sub_matrix(EST_TMatrix<T> &sm,
		  ssize_t r=0, ptrdiff_t numr=EST_ALL, 
		  ssize_t c=0, ptrdiff_t numc=EST_ALL);
  //@}

  /**@name Copy in and out
    * Copy data between buffers and the matrix.
    */
  //@{
    /** Copy row `r` of matrix to `buf`. `buf`
        should be pre-malloced to the correct size.
        */
    void copy_row(ssize_t r, T *buf, ptrdiff_t offset=0, int num=-1) const;

    /** Copy row `r` of matrix to
        `buf`. `buf` should be
        pre-malloced to the correct size.  */
    
    void copy_row(ssize_t r, EST_TVector<T> &t, ptrdiff_t offset=0, int num=-1) const;

    /** Copy column `c` of matrix to `buf`. `buf`
        should be pre-malloced to the correct size.
        */
    void copy_column(ssize_t c, T *buf, ptrdiff_t offset=0, int num=-1) const;

    /** Copy column `c` of matrix to
        `buf`. `buf` should
        be pre-malloced to the correct size.  */

    void copy_column(ssize_t c,  EST_TVector<T> &t, ptrdiff_t offset=0, int num=-1)const;

    /** Copy buf into row `n` of matrix. 
        */
    void set_row(ssize_t n, const T *buf, ptrdiff_t offset=0, int num=-1);

    void set_row(ssize_t n, const EST_TVector<T> &t, ptrdiff_t offset=0, int num=-1)
      { set_row(n, t.memory(), offset, num); }

    void set_row(ssize_t r, 
                 const EST_TMatrix<T> &from, ssize_t from_r, ssize_t from_offset=0,
                 ptrdiff_t offset=0, int num=-1); // set nth row


    /** Copy buf into column `n` of matrix.         
      */
    void set_column(ssize_t n, const T *buf, ptrdiff_t offset=0, int num=-1);

    void set_column(ssize_t n, const EST_TVector<T> &t, ptrdiff_t offset=0, int num=-1)
      { set_column(n, t.memory(), offset, num); }
    
    void set_column(ssize_t c, 
                    const EST_TMatrix<T> &from, ssize_t from_c, ssize_t from_offset=0, 
                    ptrdiff_t offset=0, int num=-1); // set nth column

  /** For when you absolutely have to have access to the memory.
    */
  void set_memory(T *buffer, ptrdiff_t offset, ssize_t rows, ssize_t columns, 
		  int free_when_destroyed=0);

  //@}

  /**@name Matrix file input / output
    */
  //@{
  /// load Matrix from file - Not currently implemented.
  EST_read_status  load(const class EST_String &filename);
  /// save Matrix to file `filename`
  EST_write_status save(const class EST_String &filename) const;

  /// print matrix.
  friend std::ostream& operator << (std::ostream &st,const EST_TMatrix<T> &a)
    {ssize_t i, j; 
        for (i = 0; i < a.num_rows(); ++i) {
            for (j = 0; j < a.num_columns(); ++j) 
                st << a.a_no_check(i, j) << " "; st << std::endl;
        }
        return st;
    }
  //@}
  
};

#undef A_CHECK

#endif

