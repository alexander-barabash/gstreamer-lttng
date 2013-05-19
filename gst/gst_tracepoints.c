/* GStreamer
 * Copyright (C) 2013 Mentor Graphics, Inc.
 *
 * gst_tracepoints.c: LTTng tracepoints.
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

#if defined(__GNUC__) && (((__GNUC__ == 4) && (__GNUC_MINOR__ >= 2)) || (__GNUC__ > 4))
#pragma GCC diagnostic ignored "-Wredundant-decls"
#endif

#include "config.h"

#ifdef GST_ENABLE_LTTNG_TRACEPOINTS

#define GST_TRACEPOINTS_CREATE_PROBES
#define TRACEPOINT_CREATE_PROBES
#define TRACEPOINT_DEFINE
#include "gst_tracepoints.h"

#endif /* GST_ENABLE_LTTNG_TRACEPOINTS */
