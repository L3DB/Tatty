#ifndef T3IO_H
#define T3IO_H

#include<Windows.h>

//������Ҽ�����
#define LEFTBUTTON_DOWN VK_LBUTTON
#define RIGHTBUTTON_DOWN VK_RBUTTON

//w������
#define W_DOWN 0x57
//a������
#define A_DOWN 0x41
//s������
#define S_DOWN 0x53
//d������
#define D_DOWN 0x44

//���尴��
#define KeyDown(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)
#define KeyUp(vk_code)   ((GetAsyncKeyState(vk_code) & 0x8000) ? 0 : 1)

#endif