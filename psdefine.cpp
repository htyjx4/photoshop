#pragma once
//this is a part of Photoshop
//by zfworld hty 2025

//photoshop basic pos
struct POS{
	short x,y;
	POS(){};
	POS(int lx,int ly){
		x=lx;y=ly;
	}
	POS(ExMessage msg){
		x=msg.x;
		y=msg.y;
	}
	POS(MOUSEMSG msg){
		x=msg.x;
		y=msg.y;
	}
};
typedef POS *PPOS;
