/* GStreamer
 * Copyright (C) 2013 Mentor Graphics, Inc.
 *
 * gst_tracepoints.h: Header file for LTTng tracepoints.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 51 Franklin St, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 */
#include "config.h"

#ifndef _GST_TRACEPOINTS_H

#define GST_TRACEPOINT_HELPER2(...) __VA_ARGS__
#define GST_TRACEPOINT_HELPER(...) GST_TRACEPOINT_HELPER2 (__VA_ARGS__)
#define GST_TRACEPOINT_EVENT(name, args, fields) \
    TRACEPOINT_EVENT (TRACEPOINT_PROVIDER, name, GST_TRACEPOINT_HELPER (args), GST_TRACEPOINT_HELPER (fields))

#endif /* _GST_TRACEPOINTS_H */

#ifdef GST_ENABLE_LTTNG_TRACEPOINTS

#undef TRACEPOINT_PROVIDER
#define TRACEPOINT_PROVIDER Gst

#undef TRACEPOINT_INCLUDE_FILE
#define TRACEPOINT_INCLUDE_FILE ./gst_tracepoints.h

#if !defined(_GST_TRACEPOINTS_H) || defined(TRACEPOINT_HEADER_MULTI_READ)
#define _GST_TRACEPOINTS_H

#include <lttng/tracepoint.h>

GST_TRACEPOINT_EVENT (gst_sample_event,
                      TP_ARGS (int, event_argument),
                      TP_FIELDS (ctf_integer(int, event_argument, event_argument)))

#endif /* _GST_TRACEPOINTS_H */

#include <lttng/tracepoint-event.h>

#define GST_TRACEPOINT(...) tracepoint (Gst, __VA_ARGS__)

#else /* !GST_ENABLE_LTTNG_TRACEPOINTS */

#define GST_TRACEPOINT(...)

#endif /* !GST_ENABLE_LTTNG_TRACEPOINTS */
