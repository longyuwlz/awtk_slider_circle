/**
 * File:   slider.c
 * Author: AWTK Develop Team
 * Brief:  slider circle demo
 *
 * Copyright (c) 2018 - 2019  Guangzhou ZHIYUAN Electronics Co.,Ltd.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * License file for more details.
 *
 */

#include "awtk.h"
#include "slider_circle.h"

static ret_t on_changed(void* ctx, event_t* evt) {
    widget_t* target = WIDGET(evt->target);

    log_debug("%s changed\n", target->name);

    return RET_OK;
}

static ret_t on_changing(void* ctx, event_t* evt) {
    widget_t* slider = WIDGET(evt->target);
    widget_t* label = WIDGET(ctx);
    char buffer[7] = {'\0'};
    value_t value;

    slider->vt->get_prop(slider, WIDGET_PROP_VALUE, &value);
    tk_snprintf(buffer, sizeof(buffer) - 1, "%3d", value_int(&value));
    widget_set_text_utf8(label, buffer);
    
    log_debug("%s changing\n", slider->name);

    return RET_OK;
}

ret_t create_slider_circle(widget_t* win, bool_t wise, const char* name) {
    widget_t* slider_circle = slider_circle_create(win, 0, 0, 0, 0);
    widget_t* label = label_create(slider_circle, 0, 0, 0, 0);
    
    if (wise) {
        widget_set_self_layout_params(slider_circle, "c", "5%", "30%", "30%");
        slider_circle_set_start_angle(slider_circle, 200);
        slider_circle_set_end_angle(slider_circle, 340);
    } else {
        widget_set_self_layout_params(slider_circle, "c", "50%", "30%", "20%");
        slider_circle_set_start_angle(slider_circle, 135);
        slider_circle_set_end_angle(slider_circle, 405);
    }

    widget_layout(slider_circle);
    
    widget_set_self_layout_params(label, "c", "50%", "20%", "20%");
    widget_layout(label);
    widget_set_text_utf8(label, "0");

    widget_on(slider_circle, EVT_VALUE_CHANGED, on_changed, label);
    widget_on(slider_circle, EVT_VALUE_CHANGING, on_changing, label);
    
    slider_circle_set_counter_clock_wise(slider_circle, wise);    
    slider_circle_set_value(slider_circle, 0);

    widget_set_name(slider_circle, name);
    
    return RET_OK;
}

void application_init() {
    widget_t* win = window_create(NULL, 0, 0, 0, 0);
    
    create_slider_circle(win, FALSE, "circle_one");
	create_slider_circle(win, TRUE, "circle_two");
    
    widget_layout(win);
}
