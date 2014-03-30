/*************************************************************************/
/*                                                                       */
/*                Centre for Speech Technology Research                  */
/*                     University of Edinburgh, UK                       */
/*                      Copyright (c) 1997,1998                          */
/*                            Red Hat, Inc.                              */
/*                         Copyright (c) 2008                            */
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
/*                Author :  Michal Schmidt                               */
/*                Date   :  November 2008                                */
/*-----------------------------------------------------------------------*/
/*  Optional support for PulseAudio                                      */
/*=======================================================================*/

#include "EST_Wave.h"
#include "EST_Option.h"
#include "audioP.h"

using namespace std;

#ifdef SUPPORT_PULSE

#include <pulse/simple.h>
#include <pulse/error.h>

int pulse_supported = TRUE;
const static char *err_prefix = "Pulseaudio: ";

static int transfer_pulse_wave(EST_Wave &inwave, EST_Option &al, int record)
{
    (void) al;
    short *waveform;
    int num_samples;
    int err, pa_ret;
    int ret = -1;
    pa_simple *s = NULL;
    pa_sample_spec ss;

    ss.format   = PA_SAMPLE_S16NE;
    ss.channels = 1;
    ss.rate     = inwave.sample_rate();

    waveform    = inwave.values().memory();
    num_samples = inwave.num_samples();

    /* In case num_samples == 0, don't play. Pulseaudio returns "invalid
     * argument" if num_samples == 0, so it's better to check now.
     * I don't expect num_samples < 0, but as we have to check anyway
     * it doesn't hurt to check.
     */
    if (num_samples <= 0) {
		ret=1;
		goto finish;
	}

    s = pa_simple_new(NULL,           // Use the default server.
	"Festival",                   // Our application's name.
	record ? PA_STREAM_RECORD : PA_STREAM_PLAYBACK,
	NULL,                         // Use the default device.
	record ? "Record" : "Speech", // Description of our stream.
	&ss,                // Our sample format.
	NULL,               // Use default channel map
	NULL,               // Use default buffering attributes.
	&err);

    if (!s) {
	cerr << err_prefix << pa_strerror(err) << endl;
	goto finish;
    }

    pa_ret = record ?
	pa_simple_read (s, waveform, num_samples*sizeof(short), &err) :
	pa_simple_write(s, waveform, num_samples*sizeof(short), &err);

    if (pa_ret < 0) {
	cerr << err_prefix << pa_strerror(err) << endl;
	goto finish;
    }

    if (!record && pa_simple_drain(s, &err) < 0) {
	cerr << err_prefix << pa_strerror(err) << endl;
	goto finish;
    }

    ret = 1;
finish:
    if (s)
	pa_simple_free(s);
    return ret;
}

int play_pulse_wave(EST_Wave &inwave, EST_Option &al)
{
	return transfer_pulse_wave(inwave, al, 0);
}

int record_pulse_wave(EST_Wave &inwave, EST_Option &al)
{
	return transfer_pulse_wave(inwave, al, 1);
}

#else
int pulse_supported = FALSE;

int play_pulse_wave(EST_Wave &inwave, EST_Option &al)
{
    (void)inwave;
    (void)al;
    cerr << "Audio: pulse not compiled in this version" << endl;
    return -1;
}

int record_pulse_wave(EST_Wave &inwave, EST_Option &al)
{
    (void)inwave;
    (void)al;
    cerr << "Audio: pulse not compiled in this version" << endl;
    return -1;
}

#endif
