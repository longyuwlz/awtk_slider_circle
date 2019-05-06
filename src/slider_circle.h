/**
 * File:   slider_circle.h
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

#ifndef TK_SLIDER_CIRCLE_H
#define TK_SLIDER_CIRCLE_H

#include "base/widget.h"


BEGIN_C_DECLS

/** 
 * @class slider_circle_t
 * @parent widget_t
 * @annotation ["scriptable", "design", "widget"]
 * 圆形滑块
 * slider\_circle\_t是[widget\_t](widget_t.md)的子类控件，widget\_t的函数均适用于slider\_circle\_t控件。
 *
 * 在xml中使用"slider_circle"标签创建圆形滑块控件。如：
 *
 * ```xml
 * <slider_circle x="c" y="100" w="100" h="120" name="circle" line_width="6" counter_clock_wise="TRUE" start_angle="0" end_angle="180"/>
 * ```
 *
 *
 * 在c代码中使用函数slider\_circle\_create创建圆形滑块控件。如：
 *
 * ```c
 *    widget_t* slider_circle = slider_circle_create(win, 40, 40, 100, 120);
 *    widget_on(slider_circle, EVT_VALUE_CHANGED, on_changed, NULL);
 *    widget_on(slider_circle, EVT_VALUE_CHANGING, on_changing, NULL);
 * ```
 *
 * > 完整示例请参考：
 * [slider circle
 * demo](http://git.zhaoqi.info:9998/awtk/awtk-slider-circle/blob/master/demos/slider_circle_test.c)
 *
 * 可用通过style来设置控件的显示风格，如图片和颜色等等。如：
 *
 * ```xml
 *<style name="default" icon="slider_drag">
 * <normal fg_color="blue" bg_color="black"/>
 * <pressed fg_color="blue" bg_color="black"/>
 * <pressed  fg_color="blue" bg_color="black"/>
 * <over     fg_color="blue" bg_color="black"/>
 *</style>
 * ```
 *
 * > 更多用法请参考：
 * [theme
 * default](https://github.com/zlgopen/awtk/blob/master/demos/assets/raw/styles/default.xml#L179)
 *
 */

typedef struct _slider_circle_t {
    widget_t widget;

    /** 
     * @property {uint16_t} value
     * @annotation ["set_prop", "get_prop", "readable", "persitent", "design", "scriptable"]
     * 滑块的值(默认为0)
     */
    uint16_t value;
    
     /**
      * @property {uint16_t} min
      * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
      * 最小值(默认为0)
      */ 
    uint16_t min;
     
    /** 
     * @property {uint16_t} max
     * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
     * 最大值(默认为100)
     */
    uint16_t max;

    /**
     * @property {uint16_t} step
     * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
     * 拖动的最小单位(默认为1)
     */ 
    uint16_t step;

    /**
     * @property {int32_t} start_angle
     * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
     * 起始角度(单位为度，默认为0) 
     */
    int32_t start_angle;
    
    /**
     * @property {int32_t} end_angle
     * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
     * 结束角度(单位为度，默认为180) 
     */ 
    int32_t end_angle;

    /** 
     * @property {uint32_t} line_width
     * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
     * 环线的厚度(默认为6)
     */
    uint32_t line_width;

    /* private */
    float_t rad;

    /* private */
    int32_t cx;

    /* private */
    int32_t cy;

    /**
     * @property {bool_t} counter_clock_wise
     * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
     * 浮标的起始位置(默认为FALSE)
     */ 
    
    bool_t counter_clock_wise;

    /*private*/
    bool_t dragging;
} slider_circle_t;

/**
 * @event {event_t} EVT_VALUE_WILL_CHANGE
 * 值即将改变事件
 */

/**
 * @event {event_t} EVT_VALUE_CHANGING
 * 值正在改变事件(拖动中)
 */

/**
 * @event {event_t} EVT_VALUE_CHANGED
 * 值改变事件
 */

/**
 * @method slider_circle_create
 * 创建slider_circle对象
 * @annotation ["constructor", "scriptable"]
 * @param {widget_t*} parent 父控件
 * @param {xy_t} x x坐标
 * @param {xy_t} y y坐标
 * @param {wh_t} w 宽度
 * @param {wh_t} h 高度
 *
 * @return {widget_t*} 对象
 */
widget_t* slider_circle_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h);

/**
 * @method slider_circle_cast
 * 转换为slider_circle对象(供脚本语言使用)
 * @annotation ["cast", "scriptable"]
 * @param {widget_t*} widget slider_circle对象
 *
 * @return {widget_t*} slider_circle对象。
 */
widget_t* slider_circle_cast(widget_t* widget);

/**
 * @method slider_circle_set_value
 * 设置滑块的值。
 * @anno tation ["scriptable"]
 * @param {widget_t*} widget 控件对象
 * @param {uint16_t}  value 值
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败
 */
ret_t slider_circle_set_value(widget_t* widget, uint16_t value);

/**
 * @method slider_circle_set_min
 * 设置滑块的最小值
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 控件对象
 * @param {uint16_t}  min 最小值
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败
 */
ret_t slider_circle_set_min(widget_t* widget, uint16_t min);

/**
 * @method slider_circle_circle_set_max
 * 设置滑块的最大值
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 控件对象
 * @param {uint16_t}  max 最大值
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败
 */
ret_t slider_circle_set_max(widget_t* widget, uint16_t max);

/**
 * @method slider_circle_set_step
 * 设置滑块的拖动的最小单位
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 控件对象
 * @param {uint16_t}  step 拖动的最小单位
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败
 */
ret_t slider_circle_set_step(widget_t* widget, uint16_t step);

/**
 * @method slider_circle_set_start_angle
 * 设置圆弧的起始角度
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 控件对象
 * @param {int32_t} start_angle 起始角度
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败
 */
ret_t slider_circle_set_start_angle(widget_t* widget, int32_t start_angle);
    
/**
 * @method slider_circle_set_end_angle
 * 设置圆弧的结束角度
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 控件对象
 * @param {int32_t} end_angle 圆弧结束角度
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败
 */
ret_t slider_circle_set_end_angle(widget_t* widget, int32_t end_angle);

/**
 * @method slider_circle_set_line_width
 * 设置线宽
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 控件对象
 * @param {uint16_t} line_width 圆弧线宽
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败
 */
ret_t slider_circle_set_line_width(widget_t* widget, uint32_t line_width);

/**
 * @method slider_circle_set_counter_clock_wise
 * 设置浮标的起始位置
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 控件对象
 * @param {bool_t} counter_clock_wise 浮标的起始位置
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败
 */
ret_t slider_circle_set_counter_clock_wise(widget_t* widget, bool_t counter_clock_wise);

ret_t slider_circle_register(void);

#define SLIDER_CIRCLE_PROP_LINE_WIDTH "line_width"
#define SLIDER_CIRCLE_PROP_START_ANGLE "start_angle"
#define SLIDER_CIRCLE_PROP_COUNTER_CLOCK_WISE "counter_clock_wise"
#define SLIDER_CIRCLE_PROP_END_ANGLE "end_angle"

#define SLIDER_CIRCLE(widget) ((slider_circle_t*)(slider_circle_cast(WIDGET(widget))))

#define WIDGET_TYPE_SLIDER_CIRCLE "slider_circle"

/* public for subclass and runtime type check */
TK_EXTERN_VTABLE(slider_circle);

END_C_DECLS

#endif /* TK_SLIDER_CIRCLE_H */
