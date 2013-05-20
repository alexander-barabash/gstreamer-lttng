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
#include "glib.h"
#include "gstpad.h"
#include "gstelement.h"
#include "gstbufferlist.h"
#include "gstevent.h"

enum GstFlowTracepointType;
static const gchar *gst_tracepoints_get_pad_element_name_if_needed (GstPad *
    pad, enum GstFlowTracepointType tracepoint_type);
static guint16 gst_tracepoints_get_thread_id (void);

static inline GstClockTime
gst_tracepoints_extract_event_latency (GstEvent * event)
{
  GstClockTime latency = 0;
  gst_event_parse_latency (event, &latency);
  return latency;
}

struct GstFlowTracepointQOSEventData;
static inline GstClockTime
gst_tracepoints_extract_qos_event_diff (GstEvent * event,
    struct GstFlowTracepointQOSEventData *event_data);
static inline gdouble
gst_tracepoints_extract_qos_event_proportion (GstEvent * event,
    struct GstFlowTracepointQOSEventData *event_data);

#define GST_TRACEPOINTS_CREATE_PROBES
#define TRACEPOINT_CREATE_PROBES
#define TRACEPOINT_DEFINE
#include "gst_tracepoints.h"

static const gchar *
gst_tracepoints_get_pad_element_name (GstPad * pad)
{
  const gchar *name = "";

  if (pad != NULL) {
    if (GST_IS_GHOST_PAD (pad)) {
      name =
          gst_tracepoints_get_pad_element_name (_priv_gst_ghostpad_get_target
          (GST_GHOST_PAD (pad)));
    } else if (GST_IS_PROXY_PAD (pad)) {
      name =
          gst_tracepoints_get_pad_element_name (GST_PAD_PEER (GST_PROXY_PAD
              (pad)));
    } else if (GST_PAD_PARENT (pad)) {
      name = GST_ELEMENT_NAME (GST_PAD_PARENT (pad));
    }
  }

  return name;
}

static const gchar *
gst_tracepoints_get_pad_element_name_if_needed (GstPad * pad,
    enum GstFlowTracepointType tracepoint_type)
{
  if (tracepoint_type == GST_TRACEPOINT_FLAG_ENTER) {
    return gst_tracepoints_get_pad_element_name (pad);
  } else {
    return "";
  }
}

static guint16
gst_tracepoints_get_thread_id (void)
{
  static gint thread_counter;
  static GPrivate key;
  gintptr thread_id = (gintptr) g_private_get (&key);
  if (G_UNLIKELY (thread_id == 0)) {
    do {
      thread_id = g_atomic_int_add (&thread_counter, 1) + 1;
      thread_id = thread_id & 0xFFFF;   /* Leave 16 bits. */
    } while (thread_id == 0);
    g_private_set (&key, (gpointer) thread_id);
  }
  return thread_id;
}

static GstBufferListItem
gst_tracepoints_trace_buffer_list_item (GstBuffer ** buffer, guint group,
    guint idx, gpointer user_data)
{
  if (*buffer != NULL) {
    GST_TRACEPOINT (gst_flow_data, *buffer,
        GST_TRACEPOINT_DATA_TYPE_BUFFER, NULL);
  }

  return GST_BUFFER_LIST_CONTINUE;
}

void
_priv_gst_tracepoints_trace_buffer_list (GstBufferList * list)
{
  gst_buffer_list_foreach (list, gst_tracepoints_trace_buffer_list_item, NULL);
}

static inline void
gst_tracepoints_extract_qos_event_data (GstEvent * event,
    struct GstFlowTracepointQOSEventData *event_data)
{
  if (!event_data->data_ready) {
    event_data->data_ready = TRUE;
    gst_event_parse_qos_full (event, &event_data->type, &event_data->proportion,
        &event_data->diff, &event_data->timestamp);
  }
}

static inline GstClockTime
gst_tracepoints_extract_qos_event_diff (GstEvent * event,
    struct GstFlowTracepointQOSEventData *event_data)
{
  gst_tracepoints_extract_qos_event_data (event, event_data);
  return event_data->diff;
}

static inline gdouble
gst_tracepoints_extract_qos_event_proportion (GstEvent * event,
    struct GstFlowTracepointQOSEventData *event_data)
{
  gst_tracepoints_extract_qos_event_data (event, event_data);
  return event_data->proportion;
}

#endif /* GST_ENABLE_LTTNG_TRACEPOINTS */
