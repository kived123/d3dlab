#include "LabMesh.h"

static void vertical_wall(MeshBuilder & mb, float x, float y, int len);
static void horizontal_wall(MeshBuilder & mb, float x, float y, int len);


void MeshBuilder::addVertex(float x, float y, float z, float u, float v)
{
	vertices.push_back(-x);
	vertices.push_back(y);
	vertices.push_back(z);

	vertices.push_back(u);
	vertices.push_back(v);
}

void BuildLabirintMesh(MeshBuilder & mb, Map& map, float StartX, float StartY )
{
	
	float WallX, WallY;
	int len;

	float x=StartX;
	float y=StartY+CELL_SIDE;

	// горизонтальные стены

	for(int j=1; j<map.Height();j++)       {
		len =0;		
		for (int i=0; i<map.Width();i++) 	{
			map.MoveTo(i,j);
      			if (map.Wall(NORTH)) {
				if (len==0) { 	
					//сохраняем 
					WallX=x;	
					WallY=y;
				}		
				len++; 	
			} else {
				if (len>0) {
					horizontal_wall(mb,WallX,WallY,len);
					len =0;
				}				
			}
			x+=CELL_SIDE;
		}		
		
		//добавляем остаток (если есть)
		if (len>0) {
			horizontal_wall(mb,WallX,WallY,len);
		}

				
		
		x=StartX;
		y+=CELL_SIDE;
	}
	
	// вертикальные стены

	x=StartX + CELL_SIDE;
	y=StartY;
	for(int j=1; j<map.Width();j++)       {
		len =0;		
		for (int i=0; i<map.Height();i++) 	{
			map.MoveTo(j,i);
      			if (map.Wall(WEST)) {
				if (len==0) { 	
					//сохраняем 
					WallX=x;	
					WallY=y;
				}		
				len++; 	
			} else {
				if (len>0) {
					vertical_wall(mb,WallX,WallY,len);
					len =0;
				}				
			}
			y+=CELL_SIDE;
		}		
		
		//добавляем остаток (если есть)
		if (len>0) {
			vertical_wall(mb,WallX,WallY,len);
		}

				
		
		y=StartY;
		x+=CELL_SIDE;
	}	

	
	//коробка
	float w = map.Width() * CELL_SIDE;
	float h = map.Height() * CELL_SIDE;
	
	//стены

	horizontal_wall(mb,StartX,StartY,map.Width());
	horizontal_wall(mb,StartX,StartY+h,map.Width());

	vertical_wall(mb,StartX, StartY,map.Height());
	vertical_wall(mb,StartX+w,StartY,map.Height());

	// пол

	float d = CELL_SIDE / 2.0f;

	mb.addVertex(StartX,-d,StartY+h,0.0f,0.0f);
 	mb.addVertex(StartX,-d,StartY, 0.0f,map.Height());
	mb.addVertex(StartX+w,-d,StartY,map.Width(),map.Height());

	mb.addVertex(StartX,-d,StartY+h,0.0f,0.0f);
	mb.addVertex(StartX+w,-d,StartY,map.Width(),map.Height());
	mb.addVertex(StartX+w,-d,StartY+h,map.Width(),0.0f);	
}

void vertical_wall(MeshBuilder & mb, float x, float y, int len)
{
	float l = len * CELL_SIDE;
	float d = CELL_SIDE / 2.0f;

	mb.addVertex(x,-d,y,len,1.0f);
	mb.addVertex(x,d,y, len,0.0f);
	mb.addVertex(x,d,y+l,0.0f,0.0f);

	
	mb.addVertex(x,-d,y,len,1.0f);
	mb.addVertex(x,d,y+l,0.0f,0.0f);	 
	mb.addVertex(x,-d,y+l,0.0f,1.0f);

}


void horizontal_wall(MeshBuilder & mb, float x, float y, int len)
{
	float l = len * CELL_SIDE;
	float d = CELL_SIDE / 2.0f;

	
	mb.addVertex(x,-d,y,len,1.0f);	
	mb.addVertex(x,d,y, len,0.0f);	
	mb.addVertex(x+l,d,y,0.0f,0.0f);

	
	mb.addVertex(x,-d,y,len,1.0f);	
	mb.addVertex(x+l,d,y,0.0f,0.0f);
	mb.addVertex(x+l,-d,y,0.0f,1.0f); 
}

