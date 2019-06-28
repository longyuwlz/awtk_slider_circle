/**
 * File:   slider_circle.c
 * Author: AWTK Develop Team
 * Brief:  slider_circle
 *
 * Copyright (c) 2018 - 2019  Guangzhou ZHIYUAN Electronics Co.,Ltd.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * License file for more details.
 *
 */

#include "tkc/mem.h"
#include "tkc/utils.h"
#include "base/widget_vtable.h"
#include "base/image_manager.h"
#include "slider_circle.h"
#include "base/widget_factory.h"

static ret_t slider_circle_get_dragger_rect(widget_t* widget, rect_t* r)
{
    bitmap_t img;
    const char* image_name = NULL;
    style_t* style = widget->astyle;
    slider_circle_t* slider_circle = SLIDER_CIRCLE(widget);
    float_t angle_range = slider_circle->end_angle - slider_circle->start_angle;
    uint16_t value_range = slider_circle->max - slider_circle->min;
    float_t angle = (angle_range * slider_circle->value) /
        (value_range ? value_range:1);

    if (slider_circle->counter_clock_wise) {
        angle = angle_range - angle;
    }

    /* calculate angle */
    angle += slider_circle->start_angle;
    angle = TK_D2R(angle + 90);

    /* calculate coordinate */
    r->x = slider_circle->cx + slider_circle->rad * sinf(angle);
    r->y = slider_circle->cy - slider_circle->rad * cosf(angle);
    
    image_name = style_get_str(style, STYLE_ID_ICON, NULL);

    if (image_name && image_manager_get_bitmap(image_manager(), image_name,
                                               &img) == RET_OK)  {
        r->x -= img.w / 2;
        r->y -= img.h / 2;
        r->h = img.w;
        r->w = img.h;
    } else {
        r->w = slider_circle->line_width * 3.5;
        r->h = r->w;
        r->x -= slider_circle->line_width;
        r->y -= slider_circle->line_width;
    }

    return RET_OK;
}

static ret_t slider_circle_paint_dragger(widget_t* widget, canvas_t* c)
{
    rect_t r;
    bitmap_t img;
    color_t color;
    color_t trans = color_init(0, 0, 0, 0);
    const char* image_name = NULL;
    style_t* style = widget->astyle;
    color = style_get_color(style, STYLE_ID_BORDER_COLOR, trans);

    slider_circle_get_dragger_rect(widget, &r);
    
    if (color.rgba.a) {
        xy_t x = c->ox;
        xy_t y = c->oy;

        canvas_set_fill_color(c, color);
        canvas_untranslate(c, c->ox, c->oy);
        canvas_fill_rect(c, r.x, r.y, r.w, r.h);
        canvas_translate(c, x, y);
    } else {
        image_name = style_get_str(style, STYLE_ID_ICON, NULL);
        
        if (image_name && image_manager_get_bitmap(image_manager(), image_name,
                                                   &img) == RET_OK)  {
            xy_t x = c->ox;
            xy_t y = c->oy;
            canvas_untranslate(c, c->ox, c->oy);
            canvas_draw_icon_in_rect(c, &img, &r);
            canvas_translate(c, x, y);
        }
    }
    
    return RET_OK;
}

static ret_t slider_circle_paint_arc(widget_t* widget,
                                     canvas_t* c,
                                     color_t color,
                                     float start_angle,
                                     float end_angle,
                                     bitmap_t* img)
{    
    if (color.rgba.a) {
        vgcanvas_t* vg = canvas_get_vgcanvas(c);
        slider_circle_t* slider_circle = SLIDER_CIRCLE(widget);

        vgcanvas_save(vg);
        vgcanvas_translate(vg, c->ox, c->oy);
        vgcanvas_set_stroke_color(vg, color);
        vgcanvas_set_line_width(vg, slider_circle->line_width);
        vgcanvas_set_line_cap(vg, "round");
        vgcanvas_begin_path(vg);
        
        vgcanvas_arc(vg, slider_circle->cx - c->ox, slider_circle->cy - c->oy,
                 slider_circle->rad, start_angle, end_angle, FALSE);
    
        vgcanvas_stroke(vg);
        vgcanvas_restore(vg);
    }
    
    if (img) {
        rect_t r =  rect_init(widget->x, widget->y, widget->w, widget->h);
        canvas_draw_icon_in_rect(c, img, &r);
    }
    
    return RET_OK;
}

static ret_t slider_circle_on_paint_self(widget_t* widget, canvas_t* c) {
    bitmap_t img;
    style_t* style = widget->astyle;
    vgcanvas_t* vg = canvas_get_vgcanvas(c);
    color_t trans = color_init(0, 0, 0, 0);
    slider_circle_t* slider_circle = SLIDER_CIRCLE(widget);
    color_t fg_color = style_get_color(style, STYLE_ID_FG_COLOR, trans);
    const char* image_name = style_get_str(style, STYLE_ID_BG_IMAGE, NULL);
    bool_t has_image = image_name && widget_load_image(widget, image_name, &img)
        == RET_OK;

    if (slider_circle->value > slider_circle->max) {
        slider_circle->value = slider_circle->max;
    } else if (slider_circle->value < slider_circle->min) {
        slider_circle->value = slider_circle->min;
    }

    if (slider_circle->cx != (widget->w / 2 + c->ox)) {
        xy_t cx = widget->w / 2;
        xy_t cy = widget->h / 2;
        
        slider_circle->cx = cx + c->ox;
        slider_circle->cy = cy + c->oy;
        slider_circle->rad = tk_min(cx, cy) - slider_circle->line_width;
    }

    if (vg != NULL) {
        bool_t ccw = slider_circle->counter_clock_wise;
        int32_t start_angle = slider_circle->start_angle;
        int32_t end_angle = slider_circle->end_angle;
        color_t bg_color = style_get_color(style, STYLE_ID_BG_COLOR, trans);
        float_t angle = (float)(slider_circle->value * (end_angle - start_angle)) / 
            (slider_circle->max - slider_circle->min);

        
        if (bg_color.rgba.a || has_image) {
            if (ccw) {
                end_angle = slider_circle->end_angle - angle;
            } else {
                start_angle = angle + slider_circle->start_angle;
            }
            
            slider_circle_paint_arc(widget, c, bg_color,
                                    TK_D2R(start_angle),
                                    TK_D2R(end_angle), has_image ? &img : NULL);
            
        }
        
        if (fg_color.rgba.a) {
            if (ccw) {
                end_angle = slider_circle->end_angle;
                start_angle = end_angle - angle;
            } else {
                start_angle = slider_circle->start_angle;
                end_angle = slider_circle->start_angle + angle;
            }
            slider_circle_paint_arc(widget, c, fg_color,
                                    TK_D2R(start_angle),
                                    TK_D2R(end_angle), NULL);
            
        }
        
        slider_circle_paint_dragger(widget, c);
    }
    
    return RET_OK;
}

ret_t slider_circle_set_value(widget_t* widget, uint16_t value) {
    slider_circle_t* slider_circle = SLIDER_CIRCLE(widget);
    return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

    if (slider_circle->value != value) {
        event_t e = event_init(EVT_VALUE_WILL_CHANGE, widget);
        widget_dispatch(widget, &e);
        slider_circle->value = value;
        e = event_init(EVT_VALUE_CHANGED, widget);
        widget_dispatch(widget, &e);
        widget_invalidate(widget, NULL);
    }

    return RET_OK;
}

ret_t slider_circle_set_min(widget_t* widget, uint16_t min) {
    slider_circle_t* slider_circle = SLIDER_CIRCLE(widget);
    return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

    slider_circle->min = min;

    return widget_invalidate(widget, NULL);
}

ret_t slider_circle_set_max(widget_t* widget, uint16_t max) {
    slider_circle_t* slider_circle = SLIDER_CIRCLE(widget);
    return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

    slider_circle->max = max;

    return widget_invalidate(widget, NULL);
}

ret_t slider_circle_set_step(widget_t* widget, uint16_t step) {
    slider_circle_t* slider_circle = SLIDER_CIRCLE(widget);
    return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

    slider_circle->step = step;

    return widget_invalidate(widget, NULL);
}

ret_t slider_circle_set_start_angle(widget_t* widget, int32_t start_angle) {
    slider_circle_t* slider_circle = SLIDER_CIRCLE(widget);
    return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

    slider_circle->start_angle = start_angle;

    return widget_invalidate(widget, NULL);
}

ret_t slider_circle_set_end_angle(widget_t* widget, int32_t end_angle) {
    slider_circle_t* slider_circle = SLIDER_CIRCLE(widget);
    return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

    slider_circle->end_angle = end_angle;

    return widget_invalidate(widget, NULL);
}

ret_t slider_circle_set_line_width(widget_t* widget, uint32_t line_width) {
    slider_circle_t* slider_circle = SLIDER_CIRCLE(widget);
    return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

    slider_circle->line_width = line_width;

    return widget_invalidate(widget, NULL);
}

ret_t slider_circle_set_counter_clock_wise(widget_t* widget, bool_t counter_clock_wise) {
    slider_circle_t* slider_circle = SLIDER_CIRCLE(widget);
    return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

    slider_circle->counter_clock_wise = counter_clock_wise;

    return widget_invalidate(widget, NULL);
}

static ret_t slider_circle_get_prop(widget_t* widget, const char* name, value_t* v) {
    slider_circle_t* slider_circle = SLIDER_CIRCLE(widget);
    return_value_if_fail(widget != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

    if (tk_str_eq(name, WIDGET_PROP_VALUE)) {
        value_set_int(v, slider_circle->value);
        return RET_OK;
    } else if (tk_str_eq(name, WIDGET_PROP_MAX)) {
        value_set_int(v, slider_circle->max);
        return RET_OK;
    } else if (tk_str_eq(name, WIDGET_PROP_MIN)) {
        value_set_int(v, slider_circle->min);
        return RET_OK;
    } else if (tk_str_eq(name, SLIDER_CIRCLE_PROP_LINE_WIDTH)) {
        value_set_uint32(v, slider_circle->line_width);
        return RET_OK;
    } else if (tk_str_eq(name, SLIDER_CIRCLE_PROP_START_ANGLE)) {
        value_set_int(v, slider_circle->start_angle);
        return RET_OK;
    } else if (tk_str_eq(name, SLIDER_CIRCLE_PROP_END_ANGLE)) {
        value_set_int(v, slider_circle->end_angle);
        return RET_OK;
    } else if (tk_str_eq(name, WIDGET_PROP_STEP)) {
        value_set_int(v, slider_circle->step);
        return RET_OK;
    } else if (tk_str_eq(name, SLIDER_CIRCLE_PROP_COUNTER_CLOCK_WISE)) {
        value_set_bool(v, slider_circle->counter_clock_wise);
        return RET_OK;
    }

    return RET_NOT_FOUND;
}

static ret_t slider_circle_set_prop(widget_t* widget, const char* name, const value_t* v) {
    return_value_if_fail(widget != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

    if (tk_str_eq(name, WIDGET_PROP_VALUE)) {
        return slider_circle_set_value(widget, value_int(v));
    } else if (tk_str_eq(name, WIDGET_PROP_MAX)) {
        return slider_circle_set_max(widget, value_int(v));
    } else if (tk_str_eq(name, WIDGET_PROP_MIN)) {
        return slider_circle_set_min(widget, value_int(v));
    } else if (tk_str_eq(name, SLIDER_CIRCLE_PROP_COUNTER_CLOCK_WISE)) {
        return slider_circle_set_counter_clock_wise(widget, value_bool(v));
    } else if (tk_str_eq(name, SLIDER_CIRCLE_PROP_LINE_WIDTH)) {
        return slider_circle_set_line_width(widget, value_int(v));
    } else if (tk_str_eq(name, SLIDER_CIRCLE_PROP_START_ANGLE)) {
        return slider_circle_set_start_angle(widget, value_int(v));
    } else if (tk_str_eq(name, SLIDER_CIRCLE_PROP_END_ANGLE)) {
        return slider_circle_set_end_angle(widget, value_int(v));
    }else if (tk_str_eq(name, WIDGET_PROP_STEP)) {
        return slider_circle_set_step(widget, value_int(v));
    } 

    return RET_NOT_FOUND;
}

static float_t calculate_real_angle(widget_t* widget, point_t p, float_t angle)
{
    slider_circle_t* slider_circle = SLIDER_CIRCLE(widget);
    int cx = slider_circle->cx;
    int cy = slider_circle->cy;
    bool_t not_in_range = FALSE;
    
    if (p.x >= cx && p.y < cy) {
        angle = angle + 270;
    } else if (p.x > cx && p.y >= cy) {
        angle = 90 - angle;
    } else if (p.x <= cx && p.y > cy) {
        angle = angle + 90;
    } else if (p.x < cx && p.y <= cy) {
        angle = 270 - angle;
    }

    not_in_range  = angle < slider_circle->start_angle ||
        angle > slider_circle->end_angle;

    if (not_in_range && slider_circle->end_angle > 360) {
        angle += 360;
    }

    return angle;
}

ret_t slider_circle_set_value_internal(widget_t* widget, uint16_t value, event_type_t etype,
                                       bool_t force) {
    uint16_t step = 0;
    uint16_t offset = 0;
    slider_circle_t* slider_circle = SLIDER_CIRCLE(widget);
    return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

    step = slider_circle->step;
    value = tk_max(tk_min(value, slider_circle->max), slider_circle->min);

    if (step > 1) {
        offset = value - slider_circle->min;
        value = slider_circle->min + ((offset + (step >> 1) - 1) / step) * step;
    }

    if (slider_circle->value != value || force) {
        event_t evt = event_init(etype, widget);

        slider_circle->value = value;
        widget_invalidate(widget, NULL);
        widget_dispatch(widget, &evt);
        widget_invalidate(widget, NULL);
    }

    return RET_OK;
}

static ret_t slider_circle_pointer_up_cleanup(widget_t* widget) {
    slider_circle_t* slider_circle = SLIDER_CIRCLE(widget);
    return_value_if_fail(widget != NULL && slider_circle != NULL, RET_BAD_PARAMS);

    slider_circle->dragging = FALSE;
    widget_ungrab(widget->parent, widget);
    widget_set_state(widget, WIDGET_STATE_NORMAL);
    widget_invalidate(widget, NULL);

  return RET_OK;
}

static ret_t slider_circle_on_event(widget_t* widget, event_t* e) {
    rect_t r;
    ret_t ret = RET_OK;
    uint16_t type = e->type;
    slider_circle_t* slider_circle = SLIDER_CIRCLE(widget);
    return_value_if_fail(widget != NULL && slider_circle != NULL, RET_BAD_PARAMS);

    ret = slider_circle->dragging ? RET_STOP : RET_OK;
    switch (type) {
    case EVT_POINTER_DOWN: {
        pointer_event_t* evt = (pointer_event_t*)e;
        point_t p = {evt->x, evt->y};

        slider_circle_get_dragger_rect(widget, &r);

        if (rect_contains(&r, p.x, p.y)) {
            slider_circle->dragging = TRUE;
            widget_set_state(widget, WIDGET_STATE_PRESSED);
            widget_grab(widget->parent, widget);
            widget_invalidate(widget, NULL);
        }
        ret = slider_circle->dragging ? RET_STOP : RET_OK;
        break;
    }
    case EVT_POINTER_DOWN_ABORT: 
        slider_circle_pointer_up_cleanup(widget);
        break;
    case EVT_POINTER_MOVE: {
        pointer_event_t* evt = (pointer_event_t*)e;
        point_t p = {evt->x, evt->y};
        uint16_t value = 0;
        float_t angle;
        float_t dist = sqrtf(powf(p.x - slider_circle->cx, 2) + powf(p.y -
            slider_circle->cy, 2));

        if (dist - slider_circle->line_width <= 0.001) {
            ret = RET_STOP;
            break;
        }

        angle = fabsf(asinf((float)(p.x - slider_circle->cx) / dist));

        angle = calculate_real_angle(widget, p, TK_R2D(angle));
        
        if (slider_circle->dragging) {
            float fvalue = 0;
            uint16_t range = slider_circle->max - slider_circle->min;

            if (slider_circle->counter_clock_wise) {
                angle = slider_circle->end_angle - angle;
            } else {
                angle = angle - slider_circle->start_angle;
            }
            
            int16_t angle_range = slider_circle->end_angle -
                slider_circle->start_angle;

            fvalue = angle / (angle_range ? angle_range : 1);
            
            if (fvalue > 1) {
                fvalue = 1;
            } else if (fvalue < 0) {
                fvalue = 0;
            }
            
            value = fvalue * (slider_circle->max - slider_circle->min) + slider_circle->min;
            
            if (abs(value - slider_circle->value) * 2 > range) {
                ret = RET_STOP;
                break;
            }

            slider_circle_set_value_internal(widget, value, EVT_VALUE_CHANGING, FALSE);
        }
        
        break;
    }
    case EVT_POINTER_UP: 
        if (slider_circle->dragging) {
            slider_circle_set_value_internal(widget, slider_circle->value, EVT_VALUE_CHANGED, TRUE);
        }

        slider_circle_pointer_up_cleanup(widget);
        break;
    case EVT_POINTER_LEAVE:
        widget_set_state(widget, slider_circle->dragging ? WIDGET_STATE_PRESSED : WIDGET_STATE_NORMAL);
        break;
    case EVT_POINTER_ENTER:
        widget_set_state(widget, slider_circle->dragging ? WIDGET_STATE_PRESSED : WIDGET_STATE_OVER);
        break;
    default: 
        ret = RET_OK;
        break;
    }

    return ret;
}

static const char* s_slider_circle_properties[] = {WIDGET_PROP_VALUE,
                                                         WIDGET_PROP_MAX,
                                                         WIDGET_PROP_MIN,
                                                         WIDGET_PROP_STEP,
                                                         SLIDER_CIRCLE_PROP_COUNTER_CLOCK_WISE,
                                                         SLIDER_CIRCLE_PROP_LINE_WIDTH,
                                                         SLIDER_CIRCLE_PROP_START_ANGLE,
                                                         SLIDER_CIRCLE_PROP_END_ANGLE,
                                                         NULL};

TK_DECL_VTABLE(slider_circle) = {.size = sizeof(slider_circle_t),
                                 .type = WIDGET_TYPE_SLIDER_CIRCLE,
                                 .clone_properties = s_slider_circle_properties,
                                 .persistent_properties = s_slider_circle_properties,
                                 .parent = TK_PARENT_VTABLE(widget),
                                 .create = slider_circle_create,
                                 .on_event = slider_circle_on_event,
                                 .on_paint_border = widget_on_paint_null,
                                 .on_paint_background = widget_on_paint_null,
                                 .on_paint_self = slider_circle_on_paint_self,
                                 .get_prop = slider_circle_get_prop,
                                 .set_prop = slider_circle_set_prop};

widget_t* slider_circle_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h) {
    widget_t* widget = widget_create(parent, TK_REF_VTABLE(slider_circle), x, y, w, h);
    slider_circle_t* slider_circle = SLIDER_CIRCLE(widget);
    return_value_if_fail(slider_circle != NULL, NULL);

    slider_circle->max = 100;
    slider_circle->min = 0;
    slider_circle->line_width = 8;
    slider_circle->start_angle = 0;
    slider_circle->end_angle = 180;
    slider_circle->step = 1;
    slider_circle->counter_clock_wise = FALSE;
    
    return widget;
}

widget_t* slider_circle_cast(widget_t* widget) {
    return_value_if_fail(WIDGET_IS_INSTANCE_OF(widget, slider_circle), NULL);

    return widget;
}

ret_t slider_circle_register(void)
{
    return widget_factory_register(widget_factory(),
        WIDGET_TYPE_SLIDER_CIRCLE, slider_circle_create);
}
