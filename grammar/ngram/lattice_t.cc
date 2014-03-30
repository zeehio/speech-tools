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
 /*************************************************************************/
 /*                                                                       */
 /*                 Author: Richard Caley (rjc@cstr.ed.ac.uk)             */
 /*                   Date: Thu Sep 25 1997                               */
 /* --------------------------------------------------------------------  */
 /* Template instantiation for lattice class.                             */
 /*                                                                       */
 /*************************************************************************/

#include "EST_TList.h"
#include "EST_TSortable.h"
#include "EST_lattice.h"

Declare_TList_T(Lattice::Arc *, Lattice_Arc_P)
Declare_TSortable_T(Lattice::Arc *, Lattice_Arc_P)

Declare_TList_T(Lattice::Node *, Lattice_Node_P)
Declare_TSortable_T(Lattice::Node *, Lattice_Node_P)

Declare_TList_T(Lattice::symbol_t, Lattice_symbol_t_P)
Declare_TSortable_T(Lattice::symbol_t, Lattice_symbol_t_P)

/* The next Declare_TVector_Base_T does not work, so I have written
 * the macro contents directly. There should be a way to make the
 * preprocessor understand, but I am not able to find it. Sergio Oller */
//Declare_TVector_Base_T(Lattice::symbol_t, {0,0}, {0,0}, Lattice_symbol_t_P)
static Lattice::symbol_t const Lattice_symbol_t_P_vec_def_val_s={0,0};
static Lattice::symbol_t Lattice_symbol_t_P_vec_error_return_s={0,0};
template <> Lattice::symbol_t const *EST_TVector<Lattice::symbol_t>::def_val=&Lattice_symbol_t_P_vec_def_val_s;
template <> Lattice::symbol_t *EST_TVector<Lattice::symbol_t>::error_return=&Lattice_symbol_t_P_vec_error_return_s;


#if defined(INSTANTIATE_TEMPLATES)

#include "../base_class/EST_TList.cc"
#include "../base_class/EST_TSortable.cc"
#include "../base_class/EST_TVector.cc"

Instantiate_TList_T(Lattice::Arc *, Lattice_Arc_P)
Instantiate_TSortable_T(Lattice::Arc *, Lattice_Arc_P)

Instantiate_TList_T(Lattice::Node *, Lattice_Node_P)
Instantiate_TSortable_T(Lattice::Node *, Lattice_Node_P)

Instantiate_TList_T(Lattice::symbol_t, Lattice_symbol_t_P)
Instantiate_TSortable_T(Lattice::symbol_t, Lattice_symbol_t_P)

Instantiate_TVector_T(Lattice::symbol_t, Lattice_symbol_t_P)

#endif
