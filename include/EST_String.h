/*************************************************************************/
/*                                                                       */
/*                Centre for Speech Technology Research                  */
/*                     University of Edinburgh, UK                       */
/*                        Copyright (c) 1997                             */
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

#ifndef __EST_STRING_H__
#define __EST_STRING_H__

class EST_String;
class EST_Regex;

#define EST_Regex_max_subexpressions 10

#include <cstring>
#include <iostream>
#include <climits>
#include <limits>

#include "EST_Chunk.h"
#include "EST_strcasecmp.h"
#include "EST_bool.h"

#include <cstdlib>

class EST_String;
int fcompare(const EST_String &a, const EST_String &b, const unsigned char *table=NULL);
int fcompare(const EST_String &a, const char *b, const unsigned char *table=NULL);

/** @class EST_String
  * @ingroup stringclasses
  * A non-copyleft implementation of a string class to use with
  * compilers that aren't GNU C++.
  *
  * Strings are reference-counted and reasonably efficient (eg you
  * can pass them around, into and out of functions and so on
  * without worrying too much about the cost).
  *
  * The associated class EST_Regex can be used to represent regular
  * expressions.
  *
  * @see EST_Chunk
  * @see EST_Regex
  * @see string_example
  * @author Alan W Black <awb@cstr.ed.ac.uk>
  * @author Richard Caley <rjc@cstr.ed.ac.uk>
  * @version $Id: EST_String.h,v 1.7 2009/07/03 17:13:56 awb Exp $
  */
class EST_String {

    /** Allow gsub() to be used in multi-threaded applications
      * This will cause gsub to use a local table of substitution points
      * walloced for each gsub. Otherwise one global one is used which
      * should be faster, but non reentrant.
      */
#   define __GSUB_REENTRANT__ (1)

/// Gripe about weird arguments like Nulls
#define __STRING_ARG_GRIPE__ (1)

///  When we find something to gripe about we die then and there.
#define __GRIPE_FATAL__ (1)

#if __GRIPE_FATAL__
#    define gripe(WHAT) (std::cerr<< ("oops! " WHAT "\n"),abort())
#else
#    define gripe(WHAT) (std::cerr<< ("oops! " WHAT "\n"))
#endif

#if __STRING_ARG_GRIPE__
#   define safe_strlen(S) ((S)?strlen(S):(gripe("null strlen"),0))
#   define CHECK_STRING_ARG(S) if (!(S)) gripe("null string arg")
#else
#   define safe_strlen(S) ((S)?strlen(S):0)
#   define CHECK_STRING_ARG(S) /* empty */
#endif

public:
    /// Global version string.
    static const char *version;

    /// Constant empty string
    static const EST_String Empty;

    /// Type of string size field.
    typedef size_t EST_string_size;
    /// Maximum string size.
#  define MAX_STRING_SIZE (std::numeric_limits<std::size_t>::max()-1)
#  define EST_STRING_ERR_IDX ((std::numeric_limits<std::size_t>::max()))

private:
    /// Smart pointer to actual memory.
    EST_ChunkPtr memory;
    /// Size of string.
    EST_string_size size;

    // Make sure this is exactly the same as an EST_String. This is being too
    // clever by half.
  
    struct EST_dumb_string {
	EST_ChunkPtr memory;
	EST_string_size size;
    } ;

    /// Flags indicating which bit of a string to extract.
    enum EST_chop_direction {
	Chop_Before = -1,
	Chop_At     = 0,
	Chop_After  = 1
    };

    /// Simple utility which removes const-ness from memory
    static inline EST_ChunkPtr &NON_CONST_CHUNKPTR(const EST_ChunkPtr &ecp) 
	{ return const_cast<EST_ChunkPtr&> (ecp);}

    /// private constructor which uses the buffer given.
    EST_String(size_t len, EST_ChunkPtr cp) {
	size=len;
	memory = cp;
    }

    /// Is more than one String represented by the same memory?
    int shareing (void) { return memory.shareing();}

    /**@name Finding substrings */
    ///@{
    /// Find substring 
    size_t locate(const char *it, size_t len, ssize_t from, size_t &start, size_t &end) const;
    /// Find substring
    size_t locate(const EST_String &s, ssize_t from, size_t &start, size_t &end) const
	{ return locate((const char *)s.memory, s.size, from, start, end); }
    /// Find match for regexp.
    size_t locate(EST_Regex &ex, ssize_t from, size_t &start, size_t &end, size_t *starts=NULL, size_t *ends=NULL) const;
    ///@}


    /**@name Extract Substrings */
    ///@{
    int extract(const char *it, size_t len, ssize_t from, size_t &start, size_t &end) const;
    int extract(const EST_String &s, ssize_t from, size_t &start, size_t &end) const
	{ return extract((const char *)s.memory, s.size, from, start, end); }
    int extract(EST_Regex &ex, ssize_t from, size_t &start, size_t &end) const;
    ///@}

    /**@name Chop out part of string */
    ///@{
    /// Locate subsring and chop.
    EST_String chop_internal(const char *s, size_t length, ssize_t pos, EST_chop_direction directionult) const;
    /// Chop at given position.
    EST_String chop_internal(ssize_t pos, size_t length, EST_chop_direction directionult) const;
  
    /// Locate match for expression and chop.
    EST_String chop_internal(EST_Regex &ex, ssize_t pos, EST_chop_direction directionult) const;
    ///@}

    /**@name Global search and replace */
    ///@{
    /// Substitute for string
    int gsub_internal(const char *os, int olength, const char *s, int length);
    /// Substitute for matches of regexp.
    int gsub_internal(EST_Regex &ex, const char *s, int length);
    ///@}

    /// Split the string down into parts. 
    int split_internal(EST_String result[], int max, const char* s_seperator, size_t slen, EST_Regex *re_separator, char quote) const;

    int Int(bool *ok_p) const;
    long Long(bool *ok_p) const;
    float Float(bool *ok_p) const;
    double Double(bool *ok_p) const;
public:

    /// Construct an empty string.
    EST_String(void) :memory() {size=0;}

    /// Construct from char *
    EST_String(const char *s);

    /// Construct from part of char * or fill with given character.
    EST_String(const char *s, int start_or_fill, ssize_t len);

    /// Construct from C string.
    EST_String(const char *s, size_t s_size, size_t start, ssize_t len);

    // Create from EST_String
    EST_String(const EST_String &s, size_t start, ssize_t len);

    /** Copy constructor
      * We have to declare our own copy constructor to lie to the
      * compiler about the constness of the RHS.
      */
    EST_String(const EST_String &s) {
      memory = NON_CONST_CHUNKPTR(s.memory);
      size = s.size;
    }

    /// Destructor.
    ~EST_String() {
	size=0;
	memory=NULL;
    }

    /// Length of string ({\em not} length of underlying chunk)
    size_t length(void) const { return size; }
    /// Size of underlying chunk.
    int space (void) const { return memory.size(); }
    /// Get a const-pointer to the actual memory.
    const char *str(void) const { return size==0?"":(const char *)memory; }
    /// Get a writable pointer to the actual memory.
    char *updatable_str(void) { return size==0?(char *)"":(char *)memory; }
    void make_updatable(void) { cp_make_updatable(memory, size+1);}


    /// Build string from a single character.
    static EST_String FromChar(const char c) 
	{ const char s[2] = { c, 0 }; return EST_String(s); }

    /// Build string from an integer.
    static EST_String Number(int i, int base=10);

    /// Build string from a long integer.
    static EST_String Number(long i, int base=10);

    /// Build string from a double.
    static EST_String Number(double d);

    /// Build string from a float
    static EST_String Number(float f);

    /// Convert to an integer
    int Int(bool &ok) const { return Int(&ok); }
    int Int(void) const { return Int((bool *)NULL); }

    /// Convert to a long
    long Long(bool &ok) const { return Long(&ok); }
    long Long(void) const { return Long((bool *)NULL); }

    /// Convert to a float
    float Float(bool &ok) const { return Float(&ok); }
    float Float(void) const { return Float((bool *)NULL); }

    /// Convert to a double
    double Double(bool &ok) const { return Double(&ok); }
    double Double(void) const { return Double((bool *)NULL); }

    /**@name Before */
    ///@{
    /// Part before position
    EST_String before(int pos, int len=0) const
	{ return chop_internal(pos, len, Chop_Before); }
    /// Part before first matching substring after pos.
    EST_String before(const char *s, int pos=0) const
	{ return chop_internal(s, safe_strlen(s), pos, Chop_Before); }
    /// Part before first matching substring after pos.
    EST_String before(const EST_String &s, int pos=0) const
	{ return chop_internal(s.str(), s.size, pos, Chop_Before); }
    /// Part before first match of regexp after pos.
    EST_String before(EST_Regex &e, int pos=0) const
	{ return chop_internal(e,  pos, Chop_Before); }
    ///@}

    /**@name At */
    ///@{
    /// Return part at position
    EST_String at(int from, int len=0) const
	{ return EST_String(str(),size,from<0?(size+from):from,len); }
    /// Return part where substring found (not useful, included for completeness)
    EST_String at(const char *s, int pos=0) const
	{ return chop_internal(s, safe_strlen(s), pos, Chop_At); }
    /// Return part where substring found (not useful, included for completeness)
    EST_String at(const EST_String &s, int pos=0) const
	{ return chop_internal(s.str(), s.size, pos, Chop_At); }
    /// Return part matching regexp.
    EST_String at(EST_Regex &e, int pos=0) const
	{ return chop_internal(e,  pos, Chop_At); }
    ///@}

    /**@name After */
    ///@{
    /// Part after pos+len
    EST_String after(int pos, int len=1) const
	{ return chop_internal(pos, len, Chop_After); }
    /// Part after substring.
    EST_String after(const char *s, int pos=0) const
	{ return chop_internal(s, safe_strlen(s), pos, Chop_After); }
    /// Part after substring.
    EST_String after(const EST_String &s, int pos=0) const
	{ return chop_internal(s.str(), s.size, pos, Chop_After); }
    /// Part after match of regular expression.
    EST_String after(EST_Regex &e, int pos=0) const
	{ return chop_internal(e,  pos, Chop_After); }
    ///@}
  
    /**@name Search for something */
    ///@{
    /// Find a substring.
    size_t search(const char *s, size_t len, size_t &mlen, ssize_t pos=0) const
	{ size_t start, end;
	if (locate(s, len, pos, start, end))
	{ mlen=end-start; return start; }
	return EST_STRING_ERR_IDX;
	}

    /// Find a substring.
    size_t search(const EST_String s, size_t &mlen, ssize_t pos=0) const
	{ size_t start, end;
	if (locate(s, pos, start, end))
	{ mlen=end-start; return start; }
	return EST_STRING_ERR_IDX;
	}

    /// Find a match of the regular expression.
    size_t search(EST_Regex &re, size_t &mlen, ssize_t pos=0, size_t *starts=NULL, size_t *ends=NULL) const
	{ size_t start=0, end=0;
	if (locate(re, pos, start, end, starts, ends))
	{ mlen=end-start; return start; }
	return EST_STRING_ERR_IDX;
	}
    ///@}


    /**@name Get position of something */
    ///@{
    /// Position of substring (starting at pos)
    size_t index(const char *s, ssize_t pos=0) const
	{ size_t start, end; return locate(s, safe_strlen(s), pos, start, end)?start:EST_STRING_ERR_IDX; }
    /// Position of substring (starting at pos)
    size_t index(const EST_String &s, ssize_t pos=0) const
	{ size_t start, end; return locate(s, pos, start, end)?start:EST_STRING_ERR_IDX; }
    /// Position of match of regexp (starting at pos)
    size_t index(EST_Regex &ex, ssize_t pos=0) const
	{ size_t start, end; return locate(ex, pos, start, end)?start:EST_STRING_ERR_IDX; }
    ///@}
  
    /**@name Does string contain something? */
    ///@{
    /// Does it contain this substring?
    int contains(const char *s, ssize_t pos=-1) const
	{ size_t start, end; return extract(s, safe_strlen(s), pos, start, end); }
    /// Does it contain this substring?
    int contains(const EST_String &s, ssize_t pos=-1) const
	{ size_t start, end; return extract(s, pos, start, end); }
    /// Does it contain this character?
    int contains(const char c, ssize_t pos=-1) const
	{ size_t start, end; char s[2] = {c,0}; return extract(s, 1, pos, start, end); }
    /// Does it contain a match for  this regular expression?
    int contains(EST_Regex &ex, ssize_t pos=-1) const
	{ size_t start, end; return extract(ex, pos, start, end); }
    ///@}

    /**@name Does string exactly match? */
    ///@{
    /// Exactly match this string?
    int matches(const char *e, ssize_t pos=0) const;
    /// Exactly match this string?
    int matches(const EST_String &e, ssize_t pos=0) const;
    /// Exactly matches this regular expression, can return ends of sub-expressions.
    int matches(EST_Regex &e, ssize_t pos=0, size_t *starts=NULL, size_t *ends=NULL) const;
    ///@}

    /**@name Global replacement */
    ///@{
    /// Substitute one string for another.
    int gsub(const char *os, const EST_String &s)
	{ return gsub_internal(os, safe_strlen(os), s, s.size); }
    /// Substitute one string for another.
    int gsub(const char *os, const char *s)
	{ return gsub_internal(os, safe_strlen(os), s, safe_strlen(s)); }
    /// Substitute one string for another.
    int gsub(const EST_String &os, const EST_String &s)
	{ return gsub_internal(os, os.size, s, s.size); }
    /// Substitute one string for another.
    int gsub(const EST_String &os, const char *s)
	{ return gsub_internal(os, os.size, s, safe_strlen(s)); }

    /// Substitute string for matches of regular expression.
    int gsub(EST_Regex &ex, const EST_String &s)
	{ return gsub_internal(ex, s, s.size); }
    /// Substitute string for matches of regular expression.
    int gsub(EST_Regex &ex, const char *s)
	{ return gsub_internal(ex, s, safe_strlen(s)); }
    /// Substitute string for matches of regular expression.
    int gsub(EST_Regex &ex, int bracket_num)
	{ return gsub_internal(ex, NULL, bracket_num); }
    /// Substitute the result of a match into a string.
    int subst(EST_String source, 
	      size_t (&starts)[EST_Regex_max_subexpressions], 
	      size_t (&ends)[EST_Regex_max_subexpressions]);
    ///@}

    /**@name Frequency counts */
    ///@{
    /// Number of occurrences of substring
    size_t freq(const char *s) const;
    /// Number of occurrences of substring
    size_t freq(const EST_String &s) const;
    /// Number of matches of regular expression.
    size_t freq(EST_Regex &s) const;
    ///@}

    /**@name Quoting */
    ///@{
    /// Return the string in quotes with internal quotes protected.
    EST_String quote(const char quotec) const;
    /// Return in quotes if there is something to protect (e.g. spaces)
    EST_String quote_if_needed(const char quotec) const;
    /// Remove quotes and unprotect internal quotes.
    EST_String unquote(const char quotec) const;
    /// Remove quotes if any.
    EST_String unquote_if_needed(const char quotec) const;
    ///@}

    /**@name Operators */
    ///@{

    /// Function style access to constant strings.
    char operator () (int i) const { return memory[i]; }
    /// Array style access to writable strings.
    char &operator [] (int i) { return memory(i); }

    /// Cast to const char * by simply giving access to pointer.
    operator const char*() const {return str(); }
    operator const char*() {return str(); }
    /// Cast to char *, may involve copying.
    operator char*() { return updatable_str(); }

    ///@}

    /**@name Add to end of string. */
    ///@{
    /// Add C string to end of EST_String
    EST_String &operator += (const char *b);
    /// Add EST_String to end of EST_String
    EST_String &operator += (const EST_String b);
    ///@}

    /**@name Assignment */
    ///@{
    /// Assign C string to EST_String
    EST_String &operator = (const char *str);
    /// Assign single character to EST_String
    EST_String &operator = (const char c);
    /// Assign EST_String to EST_String.
    EST_String &operator = (const EST_String &s);
    ///@}

    /**@name Concatenation */
    ///@{
    /// Concatenate  two EST_Strings
    friend EST_String operator + (const EST_String &a, const EST_String &b);
    /// Concatenate C String with EST_String
    friend EST_String operator + (const char *a, const EST_String &b);
    /// Concatenate EST_String with C String
    friend EST_String operator + (const EST_String &a, const char *b);
    ///@}

    /// Repeat string N times
    friend EST_String operator * (const EST_String &s, int n);

    /**@name relational operators */
    ///@{
    ///
    friend int operator == (const char *a, const EST_String &b);
    ///
    friend int operator == (const EST_String &a, const char *b)
	{ return b == a; }
    ///
    friend int operator == (const EST_String &a, const EST_String &b);

    ///
    friend int operator != (const char *a, const EST_String &b)
	{ return !(a==b); }
    ///
    friend int operator != (const EST_String &a, const char *b)
	{ return !(a==b); }
    ///
    friend int operator != (const EST_String &a, const EST_String &b)
	{ return !(a==b); }

    ///
    friend inline int operator < (const char *a, const EST_String &b)
	{ return compare(a,b) < 0; }
    ///
    friend inline int operator < (const EST_String &a, const char *b) 
	{ return compare(a,b) < 0; }
    ///
    friend inline int operator < (const EST_String &a, const EST_String &b) 
	{ return compare(a,b) < 0; }
    ///
    friend inline int operator > (const char *a, const EST_String &b) 
	{ return compare(a,b) > 0; }
    ///
    friend inline int operator > (const EST_String &a, const char *b) 
	{ return compare(a,b) > 0; }
    ///
    friend inline int operator > (const EST_String &a, const EST_String &b) 
	{ return compare(a,b) > 0; }
    ///
    friend inline int operator <= (const char *a, const EST_String &b) 
	{ return compare(a,b) <= 0; }
    ///
    friend inline int operator <= (const EST_String &a, const char *b) 
	{ return compare(a,b) <= 0; }
    ///
    friend inline int operator <= (const EST_String &a, const EST_String &b) 
	{ return compare(a,b) <= 0; }
    ///
    friend inline int operator >= (const char *a, const EST_String &b) 
	{ return compare(a,b) >= 0; }
    ///
    friend inline int operator >= (const EST_String &a, const char *b) 
	{ return compare(a,b) >= 0; }
    ///
    friend inline int operator >= (const EST_String &a, const EST_String &b) 
	{ return compare(a,b) >= 0; }
    ///@}

    ///@}

    /**@name String comparison.
     * All these operators return -1, 0 or 1 to indicate the sort
     * order of the strings.
     */
    ///@{
    /// 
    friend int compare(const EST_String &a, const EST_String &b);
    /// 
    friend int compare(const EST_String &a, const char *b);
    /// 
    friend inline int compare(const char *a, const EST_String &b)
	{ return -compare(b,a);  }
    /** Case folded comparison.
      *
      * The table argument can defined how upper and lower
      * case characters correspond. The default works for
      * ASCII.
      */
    ///@{
    friend int fcompare(const EST_String &a, const EST_String &b, 
			const unsigned char *table);

    friend int fcompare(const EST_String &a, const char *b, 
				const unsigned char *table);
    ///
    friend inline int fcompare(const EST_String &a, const EST_String &b, 
			       const EST_String &table) 
	{ return fcompare(a, b, (const unsigned char *)(const char *)table); }
    ///@}
    ///@}
    ///@}


    /**@name Split a string into parts.
      * 
      * These functions divide up a string producing an array of
      * substrings.
      */
    ///@{
    /// Split at a given separator.
    friend int split(const EST_String & s, EST_String result[], 
		     int max, const EST_String& seperator, char quote=0)
	{ return s.split_internal(result, max, (const char *)seperator, seperator.length(), NULL, quote); }
    /// Split at a given separator.
    friend int split(const EST_String &s, EST_String result[], 
		     int max, const char *seperator, char quote=0)
	{ return s.split_internal(result, max, seperator, strlen(seperator), NULL, quote); }
    /// Split at each match of the regular expression.
    friend int split(const EST_String & s, EST_String result[], int max, 
		     EST_Regex& seperator, char quote=0)
	{ return s.split_internal(result, max, NULL, 0, &seperator, quote); }
    ///@}

    /// Convert to upper case.
    friend EST_String upcase(const EST_String &s);
    /// Convert to lower case.
    friend EST_String downcase(const EST_String &s);

    /** Concatenate a number of strings. 
      * This is more efficient than multiple uses of + or +=
      */
    static EST_String cat(const EST_String s1, 
			  const EST_String s2 = Empty, 
			  const EST_String s3 = Empty, 
			  const EST_String s4 = Empty, 
			  const EST_String s5 = Empty,
			  const EST_String s6 = Empty,
			  const EST_String s7 = Empty,
			  const EST_String s8 = Empty,
			  const EST_String s9 = Empty
	);

  /*  Hacky way to ignore volatile */
      EST_String & ignore_volatile(void) volatile { return *((EST_String *)(void *)this); }

    /// Stream output for EST_String.
    friend ostream &operator << (ostream &s, const EST_String &str);
    friend class EST_Regex;

}; 

int operator == (const char *a, const EST_String &b);
int operator == (const EST_String &a, const EST_String &b);
ostream &operator << (ostream &s, const EST_String &str);

#include "EST_Regex.h"

#endif	
