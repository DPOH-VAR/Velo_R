#pragma once
#include "Arduino.h"

const double PId3 = PI / 3.0;
const double PI2d3 = PI / 3.0 * 2.0;
const double PI4d3 = PI / 3.0 * 4.0;
const double PI5d3 = PI / 3.0 * 5.0;

uint32_t colorByHue(double hue){
  
  uint32_t result = 0;
  for(unsigned int i=0; i<3; i++){
    int ceil = (int) (hue / TWO_PI);
    double drop = hue - ((double)ceil) * TWO_PI;
    while (drop < 0) drop += TWO_PI;
    int componentVal = 0; 
    
    if (drop <= PId3 || drop >= PI5d3) {
      componentVal = 255;
    } else if (drop >= PI2d3 && drop <= PI4d3) {
      componentVal = 0;
    } else if (drop <= PI2d3){
      componentVal = (int) ((PI2d3-drop)/PId3*255.0 );
    } else {
      componentVal = (int) ((drop-PI4d3)/PId3*255.0 );
    }
    if (componentVal > 255) componentVal = 255;
    if (componentVal < 0) componentVal = 0;
    hue += PI4d3;
    result = result << 8;
    result |= componentVal;
  }
  
  return result;
}

uint32_t setBrightness(uint32_t color, double brt){
  uint32_t r = (color >> 16) & 0xFF;
  uint32_t g = (color >> 8) & 0xFF;
  uint32_t b = color & 0xFF;
  
  r = (int) (((double) r) * brt);
  if (r < 0) r = 0;
  if (r > 0xFF) r = 0xFF;
  
  g = (int) (((double) g) * brt);
  if (g < 0) g = 0;
  if (g > 0xFF) g = 0xFF;
  
  b = (int) (((double) b) * brt);
  if (b < 0) b = 0;
  if (b > 0xFF) b = 0xFF;
  
  return (r << 16) | (g << 8) | b;
}

















