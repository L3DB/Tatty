#ifndef T3IO_H
#define T3IO_H

#include<Windows.h>

//鼠标左右键按下
#define LEFTBUTTON_DOWN VK_LBUTTON
#define RIGHTBUTTON_DOWN VK_RBUTTON

//w键按下
#define W_DOWN 0x57
//a键按下
#define A_DOWN 0x41
//s键按下
#define S_DOWN 0x53
//d键按下
#define D_DOWN 0x44

//定义按键
#define KeyDown(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)
#define KeyUp(vk_code)   ((GetAsyncKeyState(vk_code) & 0x8000) ? 0 : 1)

#endif