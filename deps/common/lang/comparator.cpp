/* Copyright (c) 2021 OceanBase and/or its affiliates. All rights reserved.
miniob is licensed under Mulan PSL v2.
You can use this software according to the terms and conditions of the Mulan PSL v2.
You may obtain a copy of Mulan PSL v2 at:
         http://license.coscl.org.cn/MulanPSL2
THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
See the Mulan PSL v2 for more details. */

//
// Created by wangyunlai on 2021/6/11.
//

#include <string.h>
#include <algorithm>
#include "common/defs.h"

namespace common {


int compare_int(void *arg1, void *arg2)
{
  int v1 = *(int *)arg1;
  int v2 = *(int *)arg2;
  return v1 - v2;
}

int compare_float(void *arg1, void *arg2)
{
  float v1 = *(float *)arg1;
  float v2 = *(float *)arg2;
  float cmp = v1 - v2;
  if (cmp > EPSILON) {
    return 1;
  }
  if (cmp < -EPSILON) {
    return -1;
  }
  return 0;
}

int compare_string(void *arg1, int arg1_max_length, void *arg2, int arg2_max_length)
{
  const char *s1 = (const char *)arg1;
  const char *s2 = (const char *)arg2;
  int maxlen = std::min(arg1_max_length, arg2_max_length);
  int result = strncmp(s1, s2, maxlen);
  if (0 != result) {
    return result;
  }

  if (arg1_max_length > maxlen) {
    return s1[maxlen] - 0;
  }

  if (arg2_max_length > maxlen) {
    return 0 - s2[maxlen];
  }
  return 0;
}

bool like_string(void *arg1, int arg1_max_length, void *arg2, int arg2_max_length)
{
  char * pattern = (char *)arg1;
  char * text = (char *)arg2;

  int p = 0; // pointer for pattern
  int t = 0; // pointer for text
  int star = -1; // index of last encountered *
  int match = 0; // index of last matched character in text

  while (t < arg2_max_length)
  {
    if (p < arg1_max_length && (pattern[p] == '_' || pattern[p] == text[t]))
    {
      p++;
      t++;
    }
    else if (p < arg1_max_length && pattern[p] == '%')
    {
      star = p;
      match = t;
      p++;
    }
    else if (star != -1)
    {
      p = star + 1;
      match++;
      t = match;
    }
    else
    {
      return false;
    }
  }

  while (p < arg1_max_length && pattern[p] == '%')
  {
    p++;
  }

  return p == arg1_max_length;
}


} // namespace common
