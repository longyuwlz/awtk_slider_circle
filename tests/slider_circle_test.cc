#include "slider_circle.h"
#include "gtest/gtest.h"
#include <stdlib.h>

TEST(slider_circle, basic) {
    value_t v1;
    value_t v2;
    widget_t* s = slider_circle_create(NULL, 40, 40, 40, 40);
    
    value_set_int(&v1, 40);
    ASSERT_EQ(widget_set_prop(s, "value", &v1), RET_OK);
    ASSERT_EQ(widget_get_prop(s, "value", &v2), RET_OK);
    ASSERT_EQ(value_int(&v1), value_int(&v2));

    value_set_int(&v1, 5);
    ASSERT_EQ(widget_set_prop(s, "min", &v1), RET_OK);
    ASSERT_EQ(widget_get_prop(s, "min", &v2), RET_OK);
    ASSERT_EQ(value_int(&v1), value_int(&v2));

    value_set_int(&v1, 50);
    ASSERT_EQ(widget_set_prop(s, "max", &v1), RET_OK);
    ASSERT_EQ(widget_get_prop(s, "max", &v2), RET_OK);
    ASSERT_EQ(value_int(&v1), value_int(&v2));

    value_set_int(&v1, 5);
    ASSERT_EQ(widget_set_prop(s, "step", &v1), RET_OK);
    ASSERT_EQ(widget_get_prop(s, "step", &v2), RET_OK);
    ASSERT_EQ(value_int(&v1), value_int(&v2));

    value_set_int(&v1, 40);
    ASSERT_EQ(widget_set_prop(s, "start_angle", &v1), RET_OK);
    ASSERT_EQ(widget_get_prop(s, "start_angle", &v2), RET_OK);
    ASSERT_EQ(value_int(&v1), value_int(&v2));

    value_set_int(&v1, 100);
    ASSERT_EQ(widget_set_prop(s, "end_angle", &v1), RET_OK);
    ASSERT_EQ(widget_get_prop(s, "end_angle", &v2), RET_OK);
    ASSERT_EQ(value_int(&v1), value_int(&v2));

    value_set_int(&v1, 10);
    ASSERT_EQ(widget_set_prop(s, SLIDER_CIRCLE_PROP_LINE_WIDTH, &v1), RET_OK);
    ASSERT_EQ(widget_get_prop(s, SLIDER_CIRCLE_PROP_LINE_WIDTH, &v2), RET_OK);
    ASSERT_EQ(value_int(&v1), value_int(&v2));
    
    value_set_bool(&v1, TRUE);
    ASSERT_EQ(widget_set_prop(s, SLIDER_CIRCLE_PROP_COUNTER_CLOCK_WISE, &v1), RET_OK);
    ASSERT_EQ(widget_get_prop(s, SLIDER_CIRCLE_PROP_COUNTER_CLOCK_WISE, &v2), RET_OK);
    ASSERT_EQ(value_bool(&v1), value_bool(&v2));

    widget_destroy(s);
}


#include "log_change_events.inc"

TEST(slider_circle, event) {
  widget_t* w = slider_circle_create(NULL, 0, 0, 100, 100);

  slider_circle_set_value(w, 10);

  s_log = "";
  widget_on(w, EVT_VALUE_WILL_CHANGE, on_change_events, NULL);
  widget_on(w, EVT_VALUE_CHANGED, on_change_events, NULL);

  slider_circle_set_value(w, 10);
  ASSERT_EQ(s_log, "");

  slider_circle_set_value(w, 20);
  ASSERT_EQ(s_log, "will_change;change;");

  widget_destroy(w);
}

TEST(slider_circle, cast) {
  widget_t* w = slider_circle_create(NULL, 0, 0, 100, 100);

  ASSERT_EQ(w, slider_circle_cast(w));

  widget_destroy(w);
}
