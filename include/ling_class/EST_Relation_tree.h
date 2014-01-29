/*************************************************************************/
/*                                                                       */
/*                Centre for Speech Technology Research                  */
/*                     University of Edinburgh, UK                       */
/*                         Copyright (c) 1998                            */
/*                        All Rights Reserved.                           */
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
/*                       Author :  Alan W Black                          */
/*                       Date   :  February 1998                         */
/* --------------------------------------------------------------------- */
/*   Functions for TREE relations                                        */
/*                                                                       */
/*************************************************************************/
#ifndef __EST_RELATION_TREE_H__
#define __EST_RELATION_TREE_H__

#include "EST_Item.h"

/**@defgroup buildtraversetrees Functions for building and traversing tree relations

 */

/**@defgroup treetraversalfunctions Tree traversal functions
 * @ingroup buildtraversetrees
*/

///@{

/// \brief return parent of `n`
inline EST_Item *parent(const EST_Item *n) { return n->first()->up(); }

/// \brief return first daughter of `n`
inline EST_Item *daughter1(const EST_Item *n) { return n->down(); }

/// \brief return second daughter of `n`
inline EST_Item *daughter2(const EST_Item *n) { return n->down()->next(); }

/// \brief return nth daughter of `n`
EST_Item *daughtern(const EST_Item *n, int nth);

/// \brief return last daughter of `n`
inline EST_Item *daughtern(const EST_Item *n) { return n->down()->last(); }

/// \brief return next sibling (sister) of `n`
inline EST_Item *next_sibling(const EST_Item *n) { return n->next(); }

/// \brief return previous sibling (sister) of `n`
inline EST_Item *prev_sibling(const EST_Item *n) { return n->prev(); }

/// \brief return root node of treeprevious sibling (sister) of `n`
inline EST_Item *root(const EST_Item *n) { return n->top(); }

/** \brief return parent of `n` as seen from relation `relname` */
inline EST_Item *parent(const EST_Item *n,const char *relname)
    { return parent(as(n,relname)); }

//inline EST_Item *daughters(const EST_Item *n,const char *relname)
//    { return daughters(as(n,relname)); }

/** \brief return first daughter of `n` as seen from relation 
    `relname` */
inline EST_Item *daughter1(const EST_Item *n,const char *relname)
    { return daughter1(as(n,relname)); }

/** \brief return second daughter of `n` as seen from relation 
    `relname` */
inline EST_Item *daughter2(const EST_Item *n,const char *relname)
    { return daughter2(as(n,relname)); }

/** \brief return last daughter of `n` as seen from relation 
    `relname` */
inline EST_Item *daughtern(const EST_Item *n,const char *relname)
    { return daughtern(as(n,relname)); }

/** \brief return next sibling (sister) of `n` as seen
     from relation `relname` */
inline EST_Item *next_sibling(const EST_Item *n,const char *relname)
    { return next_sibling(as(n,relname)); }

/** \brief return previous sibling (sister) of `n` as seen 
    from relation `relname` */
inline EST_Item *prev_sibling(const EST_Item *n,const char *relname)
    { return prev_sibling(as(n,relname)); }

/** \brief return root of tree of `n` as seen from
    relation `relname` */
inline EST_Item *root(const EST_Item *n,const char *relname)
    { return root(as(n,relname)); }

// should be deleted.
EST_Item *first_leaf_in_tree(const EST_Item *root);

// should be deleted.
EST_Item *last_leaf_in_tree(const EST_Item *root);

/** \brief  return the first leaf (terminal node) which is dominated by
 `n`. Note that this is different from daughter1 etc
as this descends the tree to find the leftmost terminal node (it
is like the transitive closure of daughter1).
*/
inline EST_Item *first_leaf(const EST_Item *n) {return first_leaf_in_tree(n);}

/** return the last leaf (terminal node) which is dominated by
 `n`. Note that this is different from daughter1 etc
as this descends the tree to find the right terminal node (it is
like the transitive closure of daughtern).
*/
inline EST_Item *last_leaf(const EST_Item *n) { return last_leaf_in_tree(n); }

/** Return next leaf in tree given `n`.  If
`n` is a terminal node, next_leaf() will return
the next leaf in the tree. If `n` is not
terminal, this will return the leftmost terminal node dominated by
`n`. This will return 0 only when the last leaf in
the relation has been passed.
*/
inline EST_Item *next_leaf(const EST_Item *n) { return n->next_leaf(); }

/** Return number of leaves (terminal nodes) under `n`
 */
int num_leaves(const EST_Item *n);

/** Given a node `t`, return true if
    `c` is under it in a tree */
int in_tree(const EST_Item *c,const  EST_Item *t);

///@}

/**@defgroup treebuildfunctions Tree building functions 
   @ingroup buildtraversetrees
 */
///@{

/** Add a daughter to node `n`, after any
existing daughters, and return the next daughter. If
`p` is 0, make a new node for the daughter,
otherwise add `p` to this relation as
`n`'s daughter.  */

EST_Item *append_daughter(EST_Item *n, EST_Item *p=0);

/** Add a daughter to node `n` as seen from
relation `relname`, after any existing
daughters, and return the next daughter. If `p`
is 0, make a new node for the daughter, otherwise add
`p` to this relation as
`n`'s daughter.  */

EST_Item *append_daughter(EST_Item *n, const char *relname, EST_Item *p=0);

/** Add a daughter to node `n`, before any
existing daughters, and return the next daughter. If
`p` is 0, make a new node for the daughter,
otherwise add `p` to this relation as
`n`'s daughter.  */

EST_Item *prepend_daughter(EST_Item *n, EST_Item *p=0);

/** Add a daughter to node `n` as seen from
relation `relname`, before any existing
daughters, and return the next daughter. If `p`
is 0, make a new node for the daughter, otherwise add
`p` to this relation as
`n`'s daughter.  */

EST_Item *prepend_daughter(EST_Item *n, const char *relname, EST_Item *p=0);

///@}

#endif
