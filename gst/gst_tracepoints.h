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

#include "gstpad.h"
#include "gstghostpad.h"
#include "gstbufferlist.h"

extern GstPad *_priv_gst_ghostpad_get_target (GstGhostPad *gpad);
extern void _priv_gst_tracepoints_trace_buffer_list (GstBufferList *list);

enum GstFlowTracepointType {
  GST_TRACEPOINT_FLAG_ENTER = 0,
  GST_TRACEPOINT_FLAG_EXIT = 1,
  GST_TRACEPOINT_FLAG_ERROR = 2,
  GST_TRACEPOINT_FLAG_DROPPED = 3,
};

enum GstFlowTracepointKind {
  GST_TRACEPOINT_FLAG_PUSH = 0,
  GST_TRACEPOINT_FLAG_PULL = 1,
  GST_TRACEPOINT_FLAG_SEND_EVENT = 2,
};

typedef enum GstFlowTracepointDataType {
  GST_TRACEPOINT_DATA_TYPE_BUFFER = 0,
  GST_TRACEPOINT_DATA_TYPE_BUFFER_LIST = 1,
} GstFlowTracepointDataType;

#define GST_TRACEPOINT_HELPER2(...) __VA_ARGS__
#define GST_TRACEPOINT_HELPER(...) GST_TRACEPOINT_HELPER2 (__VA_ARGS__)
#define GST_TRACEPOINT_EVENT(name, args, fields) \
    TRACEPOINT_EVENT (TRACEPOINT_PROVIDER, name, GST_TRACEPOINT_HELPER (args), GST_TRACEPOINT_HELPER (fields))

#define ctf_gst_pad_field(name, ptr) ctf_integer_hex (gintptr, name, (gintptr) (ptr))
#define ctf_gst_data_field(name, ptr) ctf_integer_hex (gintptr, name, (gintptr) (ptr))
#define ctf_gst_flow_return_field(name, value) ctf_integer_hex (gint, name, (gint) (value))
#define ctf_gst_thread_id_field(name) ctf_integer (guint16, name, gst_tracepoints_get_thread_id ())
#define ctf_gst_tracepoint_type_field(name, type) ctf_integer (guint8, name, (guint8) (type))
#define ctf_gst_tracepoint_kind_field(name, kind) ctf_integer (guint8, name, (guint8) (kind))
#define ctf_gst_data_type_field(name, type) ctf_integer (guint8, name, (guint8) (type))

#endif /* _GST_TRACEPOINTS_H */

#ifdef GST_ENABLE_LTTNG_TRACEPOINTS

#undef TRACEPOINT_PROVIDER
#define TRACEPOINT_PROVIDER Gst

#undef TRACEPOINT_INCLUDE_FILE
#define TRACEPOINT_INCLUDE_FILE ./gst_tracepoints.h

#if !defined(_GST_TRACEPOINTS_H) || defined(TRACEPOINT_HEADER_MULTI_READ)
#define _GST_TRACEPOINTS_H

#include <lttng/tracepoint.h>

GST_TRACEPOINT_EVENT (gst_flow,
                      TP_ARGS (GstPad *, pad,
                               void *, data,
                               GstFlowReturn, result,
                               enum GstFlowTracepointKind, tracepoint_kind,
                               enum GstFlowTracepointType, tracepoint_type),
                      TP_FIELDS (ctf_gst_pad_field (pad, pad)
                                 ctf_gst_data_field (data, data)
                                 ctf_gst_flow_return_field (flow_return, result)
                                 ctf_gst_thread_id_field (thread_id)
                                 ctf_gst_tracepoint_type_field (tracepoint_type, tracepoint_type)
                                 ctf_gst_tracepoint_kind_field (tracepoint_kind, tracepoint_kind)
                                 ctf_string (element_name,
                                             gst_tracepoints_get_pad_element_name_if_needed (pad, tracepoint_type))))

GST_TRACEPOINT_EVENT (gst_flow_data,
                      TP_ARGS (void *, data, GstFlowTracepointDataType, data_type, int *, trace_is_on),
                      TP_FIELDS (ctf_integer (guint, size, (data_type == GST_TRACEPOINT_DATA_TYPE_BUFFER) ? ((GstBuffer *) (data))->size : 0)
                                 ctf_gst_thread_id_field (thread_id)
                                 ctf_gst_data_type_field (data_type, ((trace_is_on ? (*trace_is_on = 1) : 0), data_type))))

#endif /* _GST_TRACEPOINTS_H */

#include <lttng/tracepoint-event.h>

#define GST_FLOW_TRACEPOINT_PUSH_ENTER(pad, data) \
    GST_TRACEPOINT (gst_flow, pad, data, GST_FLOW_OK, GST_TRACEPOINT_FLAG_PUSH, GST_TRACEPOINT_FLAG_ENTER)

#define GST_FLOW_TRACEPOINT_PUSH_EXIT(pad, ret) \
    GST_TRACEPOINT (gst_flow, pad, NULL, ret, GST_TRACEPOINT_FLAG_PUSH, GST_TRACEPOINT_FLAG_EXIT)

#define GST_FLOW_TRACEPOINT_PUSH_ERROR(pad, ret) \
    GST_TRACEPOINT (gst_flow, pad, NULL, ret, GST_TRACEPOINT_FLAG_PUSH, GST_TRACEPOINT_FLAG_ERROR)

#define GST_FLOW_TRACEPOINT_PUSH_DROPPED(pad) \
    GST_TRACEPOINT (gst_flow, pad, NULL, GST_FLOW_OK, GST_TRACEPOINT_FLAG_PUSH, GST_TRACEPOINT_FLAG_DROPPED)

#define GST_FLOW_TRACEPOINT_PULL_ENTER(pad) \
    GST_TRACEPOINT (gst_flow, pad, NULL, GST_FLOW_OK, GST_TRACEPOINT_FLAG_PULL, GST_TRACEPOINT_FLAG_ENTER)

#define GST_FLOW_TRACEPOINT_PULL_EXIT(pad, data, ret) \
    GST_TRACEPOINT (gst_flow, pad, data, ret, GST_TRACEPOINT_FLAG_PULL, GST_TRACEPOINT_FLAG_EXIT)

#define GST_FLOW_TRACEPOINT_PULL_ERROR(pad, ret) \
    GST_TRACEPOINT (gst_flow, pad, NULL, ret, GST_TRACEPOINT_FLAG_PULL, GST_TRACEPOINT_FLAG_ERROR)

#define GST_FLOW_TRACEPOINT_PULL_DROPPED(pad) \
    GST_TRACEPOINT (gst_flow, pad, NULL, GST_FLOW_OK, GST_TRACEPOINT_FLAG_PULL, GST_TRACEPOINT_FLAG_DROPPED)

#define GST_FLOW_TRACEPOINT_SEND_EVENT_ENTER(pad, event)                \
    GST_TRACEPOINT (gst_flow, pad, event, GST_FLOW_OK, GST_TRACEPOINT_FLAG_SEND_EVENT, GST_TRACEPOINT_FLAG_ENTER)

#define GST_FLOW_TRACEPOINT_SEND_EVENT_EXIT(pad, ret) \
  GST_TRACEPOINT (gst_flow, pad, NULL, (ret) ? GST_FLOW_OK : GST_FLOW_ERROR, GST_TRACEPOINT_FLAG_SEND_EVENT, GST_TRACEPOINT_FLAG_EXIT)

#define GST_FLOW_TRACEPOINT_SEND_EVENT_ERROR(pad) \
    GST_TRACEPOINT (gst_flow, pad, NULL, GST_FLOW_ERROR, GST_TRACEPOINT_FLAG_SEND_EVENT, GST_TRACEPOINT_FLAG_ERROR)

#define GST_FLOW_TRACEPOINT_SEND_EVENT_DROPPED(pad) \
    GST_TRACEPOINT (gst_flow, pad, NULL, GST_FLOW_OK, GST_TRACEPOINT_FLAG_SEND_EVENT, GST_TRACEPOINT_FLAG_DROPPED)

#define GST_FLOW_TRACEPOINT_BUFFER(buffer) \
  GST_TRACEPOINT (gst_flow_data, buffer, GST_TRACEPOINT_DATA_TYPE_BUFFER, NULL)

#define GST_FLOW_TRACEPOINT_DATA(data, is_buffer)                       \
    do {                                                                \
      int trace_is_on = 0;                                              \
      GST_TRACEPOINT (gst_flow_data, data, G_LIKELY (is_buffer) ? GST_TRACEPOINT_DATA_TYPE_BUFFER : GST_TRACEPOINT_DATA_TYPE_BUFFER_LIST, &trace_is_on); \
      if (trace_is_on && G_UNLIKELY (!is_buffer)) {                     \
          _priv_gst_tracepoints_trace_buffer_list (data);               \
      }                                                                 \
  } while (0)

#define GST_FLOW_TRACEPOINT_BUFFER_LIST(list)                           \
  do {                                                                  \
    int trace_is_on = 0;                                                \
    GST_TRACEPOINT (gst_flow_data, list, GST_TRACEPOINT_DATA_TYPE_BUFFER_LIST, &trace_is_on); \
    if (trace_is_on) {                                                  \
      _priv_gst_tracepoints_trace_buffer_list (list);                   \
    }                                                                   \
  } while (0)

#define GST_TRACEPOINT(...) tracepoint (Gst, __VA_ARGS__)

#else /* !GST_ENABLE_LTTNG_TRACEPOINTS */

#define GST_FLOW_TRACEPOINT_PUSH_ENTER(pad, data)
#define GST_FLOW_TRACEPOINT_PUSH_EXIT(pad, ret)
#define GST_FLOW_TRACEPOINT_PUSH_ERROR(pad, ret)
#define GST_FLOW_TRACEPOINT_PUSH_DROPPED(pad)
#define GST_FLOW_TRACEPOINT_PULL_ENTER(pad)
#define GST_FLOW_TRACEPOINT_PULL_EXIT(pad, data, ret)
#define GST_FLOW_TRACEPOINT_PULL_ERROR(pad, ret)
#define GST_FLOW_TRACEPOINT_PULL_DROPPED(pad)
#define GST_FLOW_TRACEPOINT_SEND_EVENT_ENTER(pad, event)
#define GST_FLOW_TRACEPOINT_SEND_EVENT_EXIT(pad, ret)
#define GST_FLOW_TRACEPOINT_SEND_EVENT_ERROR(pad)
#define GST_FLOW_TRACEPOINT_SEND_EVENT_DROPPED(pad)
#define GST_FLOW_TRACEPOINT_BUFFER(buffer)
#define GST_FLOW_TRACEPOINT_DATA(data, is_buffer)
#define GST_FLOW_TRACEPOINT_BUFFER_LIST(list)

#define GST_TRACEPOINT(...)

#endif /* !GST_ENABLE_LTTNG_TRACEPOINTS */
