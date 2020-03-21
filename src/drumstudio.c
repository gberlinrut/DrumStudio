
/* DrumStudio Kit Builder [beta]
   DrumStudio.c
   Copyright (c)2004 Gregory Berlinrut  All Rights Reserved.
   
   source file: implements function definitions in drumstudio.h
   [created on 12/05/03] [last modified 02/21/04]

   DISCLAIMER
   ----------
	THIS SOFTWARE IS PROVIDED BY GREGORY BERLINRUT ``AS IS'' AND ANY EXPRESS OR
	IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
	OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
	IN NO EVENT SHALL GREGORY BERLINRUT BE LIABLE FOR ANY DIRECT, INDIRECT,
	INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
	NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
	DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
	THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
	(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
	THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

   ACKNOWLEDGEMENTS
   ----------------
	1. GLee.h and GLee.c courtesy of Ben Woodhouse [See README or GLee.h/c]
	2. All textures except Logo.ppm are courtesy of GRsites.com.
	3. Special thanks to Dmitry Gimzelberg for technical assistance.
*/

#include "drumstudio.h"

/* --------------------------------------------------
			LIGHTING and TEXTURING FUNCTIONS
   -------------------------------------------------- */

void SetMaterial(material *mat){
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat->ambient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat->diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat->specular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, &mat->n);
}

GLubyte *LoadTexture(char *texfile){
	//	Adapted from: OpenGL, A Primer, E. Angel, Addison Wesley
	FILE *fd;
	int i, k, n, m, nm;
	char c = '#', b[100];
	GLubyte * tmp, *texdata;
	fd = fopen(texfile, "rb");
	if(fd == NULL){
		return NULL;}
	while(c == '#') {
		fscanf(fd, "%[^\n] ", b);
		fscanf(fd, "%c", &c);}
	ungetc(c,fd); 
	fscanf(fd, "%d %d %d ", &n, &m, &k);	
	nm = n*m;
	texdata= (GLubyte *) malloc(3 * sizeof(GLubyte) * nm);
	tmp=malloc(3 * sizeof(GLubyte) * nm);
	fread(tmp, sizeof(GLubyte), 3 * nm, fd);
	for(i=0;i<nm;i++){
		texdata[3*nm-3*i-3]=tmp[0];
		texdata[3*nm-3*i-2]=tmp[1];
		texdata[3*nm-3*i-1]=tmp[2];
		tmp+=3;}
	tmp-=3*nm;
	free(tmp);	
	fclose(fd);
	return texdata;
}

/* --------------------------------------------------
			DRUM CREATION and CONTROL FUNCTIONS
   -------------------------------------------------- */

// ----- creation functions -----
void make_drum(char *name, GLfloat s1, GLfloat s2, DrumFunc Render, MoveFunc Move, MoveFunc Extend, int dtype, int findex){
	drumlist[NUMDRUMS] = (drum*) malloc(sizeof(drum));
	// render data
	strcpy(drumlist[NUMDRUMS]->name, name);
	drumlist[NUMDRUMS]->size1 = s1; drumlist[NUMDRUMS]->size2 = s2;
	drumlist[NUMDRUMS]->color = ACTIVECOLOR; drumlist[NUMDRUMS]->tex = ACTIVETEXTURE;
	drumlist[NUMDRUMS]->x = 0.0; drumlist[NUMDRUMS]->y = 0.0;
	drumlist[NUMDRUMS]->R1 = 0.0; drumlist[NUMDRUMS]->R2 = 0.0;
	drumlist[NUMDRUMS]->E1 = 0.0; drumlist[NUMDRUMS]->E2 = 0.0;
	drumlist[NUMDRUMS]->T1 = 0.0; drumlist[NUMDRUMS]->T2 = 0.0;
	drumlist[NUMDRUMS]->Draw = Render;
	drumlist[NUMDRUMS]->Move[0] = Move; drumlist[NUMDRUMS]->Move[1] = Rotate;
	drumlist[NUMDRUMS]->Move[2] = Extend; drumlist[NUMDRUMS]->Move[3] = Tilt;
	// save file data
	drumlist[NUMDRUMS]->drumtype = dtype;
	drumlist[NUMDRUMS]->cfcnindex = findex;
	drumlist[NUMDRUMS]->colorindex = ACTIVECOLOR->num;
	drumlist[NUMDRUMS]->texindex = ACTIVETEXTURE->num;
	// update object list
	ACTIVE_DRUM = NUMDRUMS;
	NUMDRUMS++;
}

void delete_drum(int num){
	int i;
	if(num >= 0 && num < NUMDRUMS){
		free(drumlist[num]);
		for(i = num+1; i < NUMDRUMS; i++){
			drumlist[i-1] = drumlist[i]; }
		NUMDRUMS--;
		drumlist[NUMDRUMS] = NULL;
		if(ACTIVE_DRUM >= NUMDRUMS){ ACTIVE_DRUM = -1; }
	}
}

void NewBassDrum(int size){
	if(NUMDRUMS < 20){
		switch(size){
		case 1: make_drum("18x20 Bass Drum", 10.0*SCALE, 18.0*SCALE, DrawBassDrum, MoveFloor, ExtendSnare, 1, 1); break; 
		case 2: make_drum("18x22 Bass Drum", 11.0*SCALE, 18.0*SCALE, DrawBassDrum, MoveFloor, ExtendSnare, 1, 2); break;
		case 3: make_drum("20x24 Bass Drum", 12.0*SCALE, 20.0*SCALE, DrawBassDrum, MoveFloor, ExtendSnare, 1, 3); break;
		case 8: CurrentMenu = DRUM_MENU; break;
		default: break;
		}
	}
} 

void NewFloorTom(int size){
	if(NUMDRUMS < 20){
		switch(size){
		case 1: make_drum("16x16 Floor Tom", 8.0*SCALE, 16.0*SCALE, DrawFloorTom, MoveFloor, ExtendSnare, 2, 1); break; 
		case 2: make_drum("18x18 Floor Tom", 9.0*SCALE, 18.0*SCALE, DrawFloorTom, MoveFloor, ExtendSnare, 2, 2); break;
		case 8: CurrentMenu = DRUM_MENU; break;
		default: break;
		}
	}
} 

void NewRackTom(int size){
	if(NUMDRUMS < 20){
		switch(size){
		case 1: make_drum("8x8 Rack Tom", 4.0*SCALE, 8.0*SCALE, DrawRackTom, MoveRack, ExtendSnare, 3, 1); break; 
		case 2: make_drum("8x9 Rack Tom", 4.5*SCALE, 8.0*SCALE, DrawRackTom, MoveRack, ExtendSnare, 3, 2); break; 
		case 3: make_drum("9x10 Rack Tom", 5.0*SCALE, 9.0*SCALE, DrawRackTom, MoveRack, ExtendSnare, 3, 3); break; 
		case 4: make_drum("9x11 Rack Tom", 5.5*SCALE, 9.0*SCALE, DrawRackTom, MoveRack, ExtendSnare, 3, 4); break; 
		case 5: make_drum("10x12 Rack Tom", 6.0*SCALE, 10.0*SCALE, DrawRackTom, MoveRack, ExtendSnare, 3, 5); break; 
		case 6: make_drum("10x13 Rack Tom", 6.5*SCALE, 10.0*SCALE, DrawRackTom, MoveRack, ExtendSnare, 3, 6); break; 
		case 7: make_drum("11x14 Rack Tom", 7.0*SCALE, 11.0*SCALE, DrawRackTom, MoveRack, ExtendSnare, 3, 7); break;
		case 8: CurrentMenu = DRUM_MENU; break;
		default: break;
		}
	}
}

void NewSnareDrum(int size){
	if(NUMDRUMS < 20){
		switch(size){
		case 1: make_drum("6x12 Snare Drum", 6.0*SCALE, 6.0*SCALE, DrawSnareDrum, MoveFloor, ExtendSnare, 4, 1); break;
		case 2: make_drum("6x13 Snare Drum", 6.5*SCALE, 6.0*SCALE, DrawSnareDrum, MoveFloor, ExtendSnare, 4, 2); break;
		case 3: make_drum("6x14 Snare Drum", 7.0*SCALE, 6.0*SCALE, DrawSnareDrum, MoveFloor, ExtendSnare, 4, 3); break;
		case 8: CurrentMenu = DRUM_MENU; break;
		default: break;
		}
	}
}

void NewHiHats(int size){
	if(NUMDRUMS < 20){
		switch(size){
		case 1: make_drum("12\" Hi Hats", 6.0*SCALE, 0.0, DrawHiHats, MoveFloor, ExtendBoom, 5, 1); break;
		case 2: make_drum("13\" Hi Hats", 6.6*SCALE, 0.0, DrawHiHats, MoveFloor, ExtendBoom, 5, 2); break;
		case 3: make_drum("14\" Hi Hats", 7.0*SCALE, 0.0, DrawHiHats, MoveFloor, ExtendBoom, 5, 3); break;
		case 8: CurrentMenu = CYMBAL_MENU; break;
		default: break;
		}	
	}
}

void NewCrashCymbal(int size){
	DrumFunc draw;
	MoveFunc move;
	int backmenu;
	int cymtype;
	if(NUMDRUMS < 20){
		if(CYMTAG){ draw = DrawCrashCymbalR; move = MoveRack; backmenu = RACKCYM_MENU; cymtype = 6;}
		else { draw = DrawCrashCymbalF; move = MoveFloor; backmenu = FLOORCYM_MENU; cymtype = 7;}
		switch(size){
		case 1: make_drum("15\" Crash Cymbal", 7.5*SCALE, 0.0, draw, move, ExtendBoom, cymtype, 1); break;
		case 2: make_drum("16\" Crash Cymbal", 8.0*SCALE, 0.0, draw, move, ExtendBoom, cymtype, 2); break;
		case 3: make_drum("17\" Crash Cymbal", 8.5*SCALE, 0.0, draw, move, ExtendBoom, cymtype, 3); break;
		case 4: make_drum("18\" Crash Cymbal", 9.0*SCALE, 0.0, draw, move, ExtendBoom, cymtype, 4); break;
		case 5: make_drum("19\" Crash Cymbal", 9.5*SCALE, 0.0, draw, move, ExtendBoom, cymtype, 5); break;
		case 8: CurrentMenu = backmenu; break;
		default: break;
		}
	}
}

void NewRideCymbal(int size){
	DrumFunc draw;
	MoveFunc move;
	int backmenu;
	int cymtype;
	if(NUMDRUMS < 20){
		if(CYMTAG){ draw = DrawRideCymbalR; move = MoveRack; backmenu = RACKCYM_MENU; cymtype = 8;}
		else { draw = DrawRideCymbalF; move = MoveFloor; backmenu = FLOORCYM_MENU; cymtype = 9;}
		switch(size){
		case 1: make_drum("18\" Ride Cymbal", 9.0*SCALE, 0.0, draw, move, ExtendBoom, cymtype, 1); break;
		case 2: make_drum("20\" Ride Cymbal", 10.0*SCALE, 0.0, draw, move, ExtendBoom, cymtype, 2); break;
		case 3: make_drum("22\" Ride Cymbal", 11.0*SCALE, 0.0, draw, move, ExtendBoom, cymtype, 3); break;
		case 8: CurrentMenu = backmenu; break;
		default: break;
		}
	}
}

void NewChinaCymbal(int size){
	DrumFunc draw;
	MoveFunc move;
	int backmenu;
	int cymtype;
	if(NUMDRUMS < 20){
		if(CYMTAG){ draw = DrawChinaCymbalR; move = MoveRack; backmenu = RACKCYM_MENU; cymtype = 10;}
		else { draw = DrawChinaCymbalF; move = MoveFloor; backmenu = FLOORCYM_MENU; cymtype = 11;}
		switch(size){
		case 1: make_drum("14\" China Cymbal", 7.0*SCALE, 0.0, draw, move, ExtendBoom, cymtype, 1); break;
		case 2: make_drum("16\" China Cymbal", 8.0*SCALE, 0.0, draw, move, ExtendBoom, cymtype, 2); break;
		case 3: make_drum("18\" China Cymbal", 9.0*SCALE, 0.0, draw, move, ExtendBoom, cymtype, 3); break;
		case 4: make_drum("20\" China Cymbal", 10.0*SCALE, 0.0, draw, move, ExtendBoom, cymtype, 4); break;
		case 8: CurrentMenu = backmenu; break;
		default: break;
		}
	}
}

void NewSplashCymbal(int size){
	if(NUMDRUMS < 20){
		switch(size){
		case 1: make_drum("6\" Splash Cymbal", 3.0*SCALE, 0.0, DrawSplashCymbal, MoveRack, ExtendBoom, 12, 1); break;
		case 2: make_drum("8\" Splash Cymbal", 4.6*SCALE, 0.0, DrawSplashCymbal, MoveRack, ExtendBoom, 12, 2); break;
		case 3: make_drum("10\" Splash Cymbal", 5.0*SCALE, 0.0, DrawSplashCymbal, MoveRack, ExtendBoom, 12, 3); break;
		case 4: make_drum("12\" Splash Cymbal", 6.0*SCALE, 0.0, DrawSplashCymbal, MoveRack, ExtendBoom, 12, 4); break;
		case 8: CurrentMenu = RACKCYM_MENU; break;
		default: break;
		}	
	}
}

void NewBellCymbal(int size){
	if(NUMDRUMS < 20){
		switch(size){
		case 1: make_drum("6\" Bell Cymbal", 3.0*SCALE, 0.0, DrawBellCymbal, MoveRack, ExtendBoom, 13, 1); break;
		case 2: make_drum("8\" Bell Cymbal", 4.6*SCALE, 0.0, DrawBellCymbal, MoveRack, ExtendBoom, 13, 2); break;
		case 3: make_drum("10\" Bell Cymbal", 5.0*SCALE, 0.0, DrawBellCymbal, MoveRack, ExtendBoom, 13, 3); break;
		case 8: CurrentMenu = RACKCYM_MENU; break;
		default: break;
		}	
	}
}

void NewCymbalStand(){
	if(NUMDRUMS < 20){
		make_drum("Cymbal Stand", 0.0, 0.0, DrawCymbalStand, MoveFloor, ExtendBoom, 14, 0);
	}
}

void NewHiHatStand(){
	if(NUMDRUMS < 20){
		make_drum("Hi Hat Stand", 0.0, 0.0, DrawHiHatStand, MoveFloor, ExtendBoom, 15, 0);
	}
}

void NewSnareStand(){
	if(NUMDRUMS < 20){
		make_drum("Snare Stand", 0.0, 0.0, DrawSnareStand, MoveFloor, ExtendSnare, 16, 0);
	}
}

void NewRackMount(){
	if(NUMDRUMS < 20){
		make_drum("Rack Boom Mount", 0.0, 0.0, DrawRackMount, MoveRack, ExtendBoom, 17, 0);
	}
}

void NewBassPedal(){
	if(NUMDRUMS < 20){
		make_drum("Bass Pedal", 0.0, 0.0, DrawBassPedal, MoveFloor, ExtendBoom, 18, 0);
	}
}

void NewDoublePedal(){
	if(NUMDRUMS < 20){
		make_drum("Double Bass Pedal", 0.0, 0.0, DrawDoublePedal, MoveFloor, ExtendBoom, 19, 0);
	}
}

void NewDrumThrone(){
	if(NUMDRUMS < 20){
		make_drum("Drum Throne", 0.0, 0.0, DrawDrumThrone, MoveFloor, ExtendSnare, 20, 0);
	}
}

// ----- rendering functions -----
void DrawBassDrum(drum *bassdrum){
	GLUquadricObj* quad = gluNewQuadric();
	int i;
	GLfloat lug_angle = 22.5;
	gluQuadricDrawStyle(quad, DRAWMODE);
// shell	
	SetMaterial(bassdrum->color);
	glPushMatrix();	
	glTranslatef(bassdrum->x, bassdrum->y, bassdrum->size1);
	glRotatef(90.0, 1.0, 0.0, 0.0);
	glRotatef(bassdrum->R1, 0.0, 1.0, 0.0);
	glTranslatef(0.0, 0.0, -bassdrum->size2/2.0);

	if(bassdrum->tex->num && *bassdrum->tex->texture != NULL){
		glEnable(GL_TEXTURE_2D);
		gluQuadricTexture(quad, GL_TRUE);
		glTexImage2D(GL_TEXTURE_2D,0, GL_RGB, 256, 128, 0, GL_RGB, GL_UNSIGNED_BYTE, bassdrum->tex->texture[0]);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP);
		glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP);
		glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
		SetMaterial(&POLISH);
		gluCylinder(quad, bassdrum->size1, bassdrum->size1, bassdrum->size2, 50, 10);
		gluQuadricTexture(quad, GL_FALSE);
		glDisable(GL_TEXTURE_2D);
	}
	else{ gluCylinder(quad, bassdrum->size1, bassdrum->size1, bassdrum->size2, 50, 10); }

// rims and skins
	SetMaterial(&STEEL);
	glPushMatrix();
	glTranslatef(0.0, 0.0, -0.02);
	gluCylinder(quad, bassdrum->size1, bassdrum->size1, 0.02, 50, 2);
	gluCylinder(quad, bassdrum->size1-0.01, bassdrum->size1-0.01, 0.02, 50, 2);
	glTranslatef(0.0, 0.0, bassdrum->size2 + 0.02);
	gluCylinder(quad, bassdrum->size1, bassdrum->size1, 0.02, 50, 2);
	gluCylinder(quad, bassdrum->size1-0.01, bassdrum->size1-0.01, 0.02, 50, 2);
	SetMaterial(bassdrum->color);
	glTranslatef(0.0, 0.0, 0.02);
	gluDisk(quad, bassdrum->size1-0.01, bassdrum->size1,  50, 2);
	glTranslatef(0.0, 0.0, -bassdrum->size2 - 0.04);
	gluDisk(quad, bassdrum->size1-0.01, bassdrum->size1,  50, 2);
	SetMaterial(&CLEAR);
	glTranslatef(0.0, 0.0, 0.02);
	gluDisk(quad, 0.0, bassdrum->size1-0.01,  50, 5);
	SetMaterial(&BLACK);
	glTranslatef(0.0, 0.0, bassdrum->size2);
	gluDisk(quad, 0.0, bassdrum->size1-0.01,  50, 5);
	glPopMatrix();
//lugs
	SetMaterial(&STEEL);
	for(i = 0; i < 8; i++){
		glPushMatrix();
		glRotatef(lug_angle, 0.0, 0.0, 1.0);
		glTranslatef(bassdrum->size1, 0.0, 0.01);
		glScalef(0.01, 0.01, 0.05);
		RenderFcn[SolidState](1.0);
		glPopMatrix();
		lug_angle += 45.0;
	}
	lug_angle = 22.5;
	for(i = 0; i < 8; i++){
		glPushMatrix();
		glRotatef(lug_angle, 0.0, 0.0, 1.0);
		glTranslatef(bassdrum->size1, 0.0, 0.05);
		glRotatef(90.0, 0.0, 1.0, 0.0);
		gluCylinder(quad, 0.02, 0.015, 0.01, 10, 2);
		glTranslatef(0.0, 0.0, 0.01);
		gluDisk(quad, 0.0, 0.015, 10, 2);
		glPopMatrix();
		lug_angle += 45.0;
	}
	lug_angle = 22.5;
	for(i = 0; i < 8; i++){
		glPushMatrix();
		glRotatef(lug_angle, 0.0, 0.0, 1.0);
		glTranslatef(bassdrum->size1, 0.0, bassdrum->size2-0.01);
		glScalef(0.01, 0.01, 0.05);
		RenderFcn[SolidState](1.0);
		glPopMatrix();
		lug_angle += 45.0;
	}
	lug_angle = 22.5;
	for(i = 0; i < 8; i++){
		glPushMatrix();
		glRotatef(lug_angle, 0.0, 0.0, 1.0);
		glTranslatef(bassdrum->size1, 0.0, bassdrum->size2-0.05);
		glRotatef(90.0, 0.0, 1.0, 0.0);
		gluCylinder(quad, 0.02, 0.015, 0.01, 10, 2);
		glTranslatef(0.0, 0.0, 0.01);
		gluDisk(quad, 0.0, 0.015, 10, 2);
		glPopMatrix();
		lug_angle += 45.0;
	}
// legs
	glPushMatrix();
	glRotatef(-15.0, 0.0, 0.0, 1.0);
	glTranslatef(bassdrum->size1, 0.0, bassdrum->size2*0.75);
	gluSphere(quad, 0.03, 10, 10);
	glPopMatrix();
	
	glPushMatrix();
	glRotatef(-165.0, 0.0, 0.0, 1.0);
	glTranslatef(bassdrum->size1, 0.0, bassdrum->size2*0.75);
	gluSphere(quad, 0.03, 10, 10);
	glPopMatrix();
	
	glPushMatrix();
	glTranslatef(bassdrum->size1+0.01, -bassdrum->size1*0.25, bassdrum->size2*0.75);
	glRotatef(90.0, 1.0, 0.0, 0.0);
	gluCylinder(quad, 0.01, 0.01, bassdrum->size1*0.75-0.02, 8, 3);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-bassdrum->size1-0.01, -bassdrum->size1*0.25, bassdrum->size2*0.75);
	glRotatef(90.0, 1.0, 0.0, 0.0);
	gluCylinder(quad, 0.01, 0.01, bassdrum->size1*0.75-0.02, 8, 3);
	glPopMatrix();

	SetMaterial(&RUBBER);
	glPushMatrix();
	glTranslatef(bassdrum->size1+0.01, -bassdrum->size1+0.02, bassdrum->size2*0.75);
	glRotatef(90.0, 1.0, 0.0, 0.0);
	gluCylinder(quad, 0.01, 0.015, 0.02, 8, 1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-bassdrum->size1-0.01, -bassdrum->size1+0.02, bassdrum->size2*0.75);
	glRotatef(90.0, 1.0, 0.0, 0.0);
	gluCylinder(quad, 0.01, 0.015, 0.02, 8, 1);
	glPopMatrix();
	glPopMatrix();
}

void DrawFloorTom(drum *floortom){
	GLUquadricObj* quad = gluNewQuadric();
	int i;
	GLfloat lug_angle = 22.5;
	gluQuadricDrawStyle(quad, DRAWMODE);
// shell and rims
	glPushMatrix();
	glTranslatef(floortom->x, floortom->y, 0.0);
	glRotatef(floortom->R1, 0.0, 0.0, 1.0);
	glPushMatrix();
	glTranslatef(0.0, 0.0, floortom->E1+0.04);
	SetMaterial(&STEEL);
	gluCylinder(quad, floortom->size1, floortom->size1, 0.01, 50, 1);
	glTranslatef(0.0, 0.0, 0.01);
	SetMaterial(floortom->color);
	if(floortom->tex->num && *floortom->tex->texture != NULL){
		glEnable(GL_TEXTURE_2D);
		gluQuadricTexture(quad, GL_TRUE);
		glTexImage2D(GL_TEXTURE_2D,0, GL_RGB, 256, 128, 0, GL_RGB, GL_UNSIGNED_BYTE, floortom->tex->texture[0]);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP);
		glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP);
		glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
		SetMaterial(&POLISH);
		gluCylinder(quad, floortom->size1, floortom->size1, floortom->size2, 50, 10);
		gluQuadricTexture(quad, GL_FALSE);
		glDisable(GL_TEXTURE_2D);
	}
	else{ gluCylinder(quad, floortom->size1, floortom->size1, floortom->size2, 50, 10); }
	glTranslatef(0.0, 0.0, floortom->size2);
	SetMaterial(&STEEL);
	gluCylinder(quad, floortom->size1, floortom->size1, 0.01, 50, 1);
// top skin
	SetMaterial(&CLEAR);
	gluDisk(quad, 0.0, floortom->size1-0.01, 50, 4);
	SetMaterial(floortom->color);
	gluDisk(quad, floortom->size1-0.01, floortom->size1, 50, 1);
// lugs
	glTranslatef(0.0, 0.0, -floortom->size2-0.01);
	SetMaterial(&STEEL);
	for(i = 0; i < 8; i++){
		glPushMatrix();
		glRotatef(lug_angle, 0.0, 0.0, 1.0);
		glTranslatef(floortom->size1+0.0025, 0.0, 0.03);
		glScalef(0.005, 0.01, 0.05);
		RenderFcn[SolidState](1.0);
		glPopMatrix();
		lug_angle += 45.0;
	}
	for(i = 0; i < 8; i++){
		glPushMatrix();
		glRotatef(lug_angle, 0.0, 0.0, 1.0);
		glTranslatef(floortom->size1, 0.0, 0.07);
		glRotatef(90.0, 0.0, 1.0, 0.0);
		gluCylinder(quad, 0.02, 0.015, 0.01, 10, 2);
		glTranslatef(0.0, 0.0, 0.01);
		gluDisk(quad, 0.0, 0.015, 10, 2);
		glPopMatrix();
		lug_angle += 45.0;
	}
	for(i = 0; i < 8; i++){
		glPushMatrix();
		glRotatef(lug_angle, 0.0, 0.0, 1.0);
		glTranslatef(floortom->size1+0.0025, 0.0, floortom->size2-0.01);
		glScalef(0.005, 0.01, 0.05);
		RenderFcn[SolidState](1.0);
		glPopMatrix();
		lug_angle += 45.0;
	}
	for(i = 0; i < 8; i++){
		glPushMatrix();
		glRotatef(lug_angle, 0.0, 0.0, 1.0);
		glTranslatef(floortom->size1, 0.0, floortom->size2-0.05);
		glRotatef(90.0, 0.0, 1.0, 0.0);
		gluCylinder(quad, 0.02, 0.015, 0.01, 10, 2);
		glTranslatef(0.0, 0.0, 0.01);
		gluDisk(quad, 0.0, 0.015, 10, 2);
		glPopMatrix();
		lug_angle += 45.0;
	}
	glPopMatrix();
// legs
	for(i=0; i<3; i++){
		glPushMatrix();
		SetMaterial(&BLACK);
		glRotatef(120.0*(GLfloat) i, 0.0, 0.0, 1.0);
		glTranslatef(floortom->size1+0.015, 0.0, 0.0);
		gluCylinder(quad, 0.03, 0.01, 0.04, 10, 2);
		SetMaterial(&STEEL);
		glTranslatef(0.0, 0.0, 0.04);
		gluCylinder(quad, 0.01, 0.01, floortom->size2*0.7 + floortom->E1, 10, 4);
		glTranslatef(-0.015, 0.0, floortom->size2*0.7 + floortom->E1);
		gluSphere(quad, 0.025, 10, 10);
		glPopMatrix();
	}
	glPopMatrix();
}

void DrawRackTom(drum *racktom){
	GLUquadricObj* quad = gluNewQuadric();
	int i;
	GLfloat lug_angle = 22.5;
	gluQuadricDrawStyle(quad, DRAWMODE);
	glPushMatrix();
	glTranslatef(0.0, 0.5, RACKHEIGHT);
	glRotatef(racktom->x, 0.0, 0.0, 1.0);
	glTranslatef(0.0, -1.5, 0.0);
// mount
	SetMaterial(racktom->color);
	glPushMatrix();
	glRotatef(90.0, 0.0, 1.0, 0.0);
	glTranslatef(0.0, 0.0, -0.0175);
	gluCylinder(quad, 0.035, 0.035, 0.035, 10, 5);
	SetMaterial(&BLACK);
	gluDisk(quad, 0.02, 0.035, 10, 1);
	glTranslatef(0.0, 0.0, 0.035);
	gluDisk(quad, 0.02, 0.035, 10, 1);
	glPopMatrix();
	glPushMatrix();
	SetMaterial(&STEEL);
	glRotatef(racktom->T1, 1.0, 0.0, 0.0);
	glTranslatef(0.0, 0.0, -racktom->E1+0.02);
	gluCylinder(quad, 0.01, 0.01, 0.15, 10, 4);
	glRotatef(racktom->R1, 0.0, 0.0, 1.0);
	glTranslatef(-0.0175, 0.0, 0.15);
	glRotatef(90.0, 0.0, 1.0, 0.0);
	gluDisk(quad, 0.0, 0.02, 10, 2);
	gluCylinder(quad, 0.02, 0.02, 0.03, 10, 2);
	glTranslatef(0.0, 0.0, 0.03);
	gluDisk(quad, 0.0, 0.02, 10, 2);
	glTranslatef(0.0, 0.0, -0.015);
	glRotatef(-90.0, 1.0, 0.0, 0.0);
	glRotatef(racktom->T2, 0.0, 1.0, 0.0);
	gluCylinder(quad, 0.01, 0.01, 0.06, 10, 2);
	glTranslatef(0.0, 0.0, 0.06);
	gluDisk(quad, 0.01, 0.02, 10, 1);
	gluCylinder(quad, 0.02, 0.02, 0.025, 10, 2);
	glTranslatef(0.0, 0.0, racktom->size1+0.02);
	glRotatef(90.0, 0.0, 1.0, 0.0);
	glRotatef(racktom->R2, 1.0, 0.0, 0.0);
// rim
	glTranslatef(0.0, 0.0, -racktom->size2/4.0-0.01);
	gluCylinder(quad, racktom->size1, racktom->size1, 0.01, 50, 1);
	glTranslatef(0.0, 0.0, 0.01);
	SetMaterial(&CLEAR);
	gluDisk(quad, 0.0, racktom->size1-0.01, 50, 4);
	SetMaterial(racktom->color);
	gluDisk(quad, racktom->size1-0.01, racktom->size1, 50, 4);
// shell
	if(racktom->tex->num && *racktom->tex->texture != NULL){
		glEnable(GL_TEXTURE_2D);
		gluQuadricTexture(quad, GL_TRUE);
		glTexImage2D(GL_TEXTURE_2D,0, GL_RGB, 256, 128, 0, GL_RGB, GL_UNSIGNED_BYTE, racktom->tex->texture[0]);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP);
		glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP);
		glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
		SetMaterial(&POLISH);
		gluCylinder(quad, racktom->size1, racktom->size1, racktom->size2, 50, 5);
		gluQuadricTexture(quad, GL_FALSE);
		glDisable(GL_TEXTURE_2D);
	}
	else{ gluCylinder(quad, racktom->size1, racktom->size1, racktom->size2, 50, 5); }
// rim
	SetMaterial(&STEEL);
	glTranslatef(0.0, 0.0, racktom->size2);
	gluCylinder(quad, racktom->size1, racktom->size1, 0.01, 50, 1);
	SetMaterial(&BLACK);
	gluDisk(quad, 0.0, racktom->size1-0.01, 50, 4);
	SetMaterial(racktom->color);
	gluDisk(quad, racktom->size1-0.01, racktom->size1, 50, 4);
// lugs
	SetMaterial(&STEEL);
	glTranslatef(0.0, 0.0, -racktom->size2-0.01);
	for(i = 0; i < 8; i++){
		glPushMatrix();
		glRotatef(lug_angle, 0.0, 0.0, 1.0);
		glTranslatef(racktom->size1+0.0025, 0.0, racktom->size2);
		glScalef(0.005, 0.01, 0.03);
		RenderFcn[SolidState](1.0);
		glPopMatrix();
		lug_angle += 45.0;
	}
	for(i = 0; i < 8; i++){
		glPushMatrix();
		glRotatef(lug_angle, 0.0, 0.0, 1.0);
		glTranslatef(racktom->size1, 0.0, racktom->size2-0.02);
		glRotatef(90.0, 0.0, 1.0, 0.0);
		gluCylinder(quad, 0.015, 0.01, 0.01, 10, 2);
		glTranslatef(0.0, 0.0, 0.01);
		gluDisk(quad, 0.0, 0.01, 10, 2);
		glPopMatrix();
		lug_angle += 45.0;
	}
	for(i = 0; i < 8; i++){
		glPushMatrix();
		glRotatef(lug_angle, 0.0, 0.0, 1.0);
		glTranslatef(racktom->size1+0.0025, 0.0, 0.02);
		glScalef(0.005, 0.01, 0.03);
		RenderFcn[SolidState](1.0);
		glPopMatrix();
		lug_angle += 45.0;
	}
	for(i = 0; i < 8; i++){
		glPushMatrix();
		glRotatef(lug_angle, 0.0, 0.0, 1.0);
		glTranslatef(racktom->size1, 0.0, 0.04);
		glRotatef(90.0, 0.0, 1.0, 0.0);
		gluCylinder(quad, 0.015, 0.01, 0.01, 10, 2);
		glTranslatef(0.0, 0.0, 0.01);
		gluDisk(quad, 0.0, 0.01, 10, 2);
		glPopMatrix();
		lug_angle += 45.0;
	}
	glPopMatrix();
	glPopMatrix();
}

void DrawSnareDrum(drum* snaredrum){
	GLUquadricObj* quad = gluNewQuadric();
	int i;
	GLfloat lug_angle = 22.5;
	gluQuadricDrawStyle(quad, DRAWMODE);
// stand
	DrawSnareStand(snaredrum);

	glPushMatrix();
	glTranslatef(snaredrum->x, snaredrum->y, snaredrum->E1+0.235);
	glRotatef(snaredrum->R1, 0.0, 0.0, 1.0);
	glRotatef(snaredrum->T2, 1.0, 0.0, 0.0);
	glTranslatef(0.0, 0.0, 0.06);
// rim bottom skin
	SetMaterial(&STEEL);
	gluCylinder(quad, snaredrum->size1, snaredrum->size1, 0.01, 50, 1);
	glTranslatef(0.0, 0.0, 0.005);
// snare beads
	glBegin(GL_LINES);
	glVertex3f(-snaredrum->size1+0.02, 0.0, 0.0); glVertex3f(snaredrum->size1-0.02, 0.0, 0.0);
	glVertex3f(-snaredrum->size1, 0.004, 0.0); glVertex3f(snaredrum->size1, 0.004, 0.0);
	glVertex3f(-snaredrum->size1+0.02, 0.008, 0.0); glVertex3f(snaredrum->size1-0.02, 0.008, 0.0);
	glVertex3f(-snaredrum->size1+0.02, 0.012, 0.0); glVertex3f(snaredrum->size1-0.02, 0.012, 0.0);
	glVertex3f(-snaredrum->size1, -0.004, 0.0); glVertex3f(snaredrum->size1, -0.004, 0.0);
	glVertex3f(-snaredrum->size1+0.02, -0.008, 0.0); glVertex3f(snaredrum->size1-0.02, -0.008, 0.0);
	glVertex3f(-snaredrum->size1+0.02, -0.012, 0.0); glVertex3f(snaredrum->size1-0.02, -0.012, 0.0);
	glVertex3f(-snaredrum->size1+0.02, 0.012, 0.0); glVertex3f(-snaredrum->size1+0.02, -0.012, 0.0);
	glVertex3f(snaredrum->size1-0.02, 0.012, 0.0); glVertex3f(snaredrum->size1-0.02, -0.012, 0.0);
	glEnd();
	glTranslatef(0.0, 0.0, 0.005);
	SetMaterial(&CLEAR);
	gluDisk(quad, 0.0, snaredrum->size1, 50, 5);
// shell
	SetMaterial(snaredrum->color);
	if(snaredrum->tex->num && *snaredrum->tex->texture != NULL){
		glEnable(GL_TEXTURE_2D);
		gluQuadricTexture(quad, GL_TRUE);
		glTexImage2D(GL_TEXTURE_2D,0, GL_RGB, 256, 128, 0, GL_RGB, GL_UNSIGNED_BYTE, snaredrum->tex->texture[0]);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP);
		glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP);
		glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
		SetMaterial(&POLISH);
		gluCylinder(quad, snaredrum->size1, snaredrum->size1, snaredrum->size2, 50, 5);
		gluQuadricTexture(quad, GL_FALSE);
		glDisable(GL_TEXTURE_2D);
	}
	else{ gluCylinder(quad, snaredrum->size1, snaredrum->size1, snaredrum->size2, 50, 5); }
// batter skin and rim
	glTranslatef(0.0, 0.0, snaredrum->size2);
	SetMaterial(&CLEAR);
	gluDisk(quad, 0.0, snaredrum->size1, 50, 5);
	SetMaterial(&STEEL);
	gluCylinder(quad, snaredrum->size1, snaredrum->size1, 0.01, 50, 1);
// lugs
	glTranslatef(0.0, 0.0, -0.06);
	for(i = 0; i < 8; i++){
		glPushMatrix();
		glRotatef(lug_angle, 0.0, 0.0, 1.0);
		glTranslatef(snaredrum->size1+0.0025, 0.0, 0.0);
		glScalef(0.005, 0.01, 0.13);
		RenderFcn[SolidState](1.0);
		glPopMatrix();
		lug_angle += 45.0;
	}
	for(i = 0; i < 8; i++){
		glPushMatrix();
		glRotatef(lug_angle, 0.0, 0.0, 1.0);
		glTranslatef(snaredrum->size1, 0.0, 0.0);
		glRotatef(90.0, 0.0, 1.0, 0.0);
		gluCylinder(quad, 0.02, 0.015, 0.01, 10, 2);
		glTranslatef(0.0, 0.0, 0.01);
		gluDisk(quad, 0.0, 0.015, 10, 2);
		glPopMatrix();
		lug_angle += 45.0;
	}
	glTranslatef(snaredrum->size1+0.004, 0.0, -0.03);
	glScalef(0.008, 0.02, 0.06);
	RenderFcn[SolidState](1.0);
	glPopMatrix();
}


void DrawHiHats(drum *hihats){
	GLUquadricObj* quad = gluNewQuadric();
	gluQuadricDrawStyle(quad, DRAWMODE);
	DrawHiHatStand(hihats);

	SetMaterial(&BRASS2);
	glPushMatrix();
	glTranslatef(hihats->x, hihats->y, hihats->E2+0.46);
	gluCylinder(quad, 0.005, hihats->size1, 0.02, 50, 2);
	glTranslatef(0.0, 0.0, 0.0225);
	gluCylinder(quad, hihats->size1, 0.01, 0.02, 50, 2);
	glPopMatrix();	
}

void DrawCrashCymbalF(drum* crashcym){
	GLUquadricObj* quad = gluNewQuadric();
	gluQuadricDrawStyle(quad, DRAWMODE);
	DrawCymbalStand(crashcym);

	glPushMatrix();
	glTranslatef(crashcym->x, crashcym->y, crashcym->E2+0.51);
	glRotatef(90.0, 0.0, 1.0, 0.0);
	glRotatef(-crashcym->R1-90.0, 1.0, 0.0, 0.0);
	glRotatef(crashcym->T2, 0.0, 1.0, 0.0);
	glTranslatef(0.0, 0.0, 0.3+crashcym->E1);
	glRotatef(crashcym->R2, 0.0, 0.0, 1.0);
	glRotatef(crashcym->T1, 0.0, 1.0, 0.0);
	glRotatef(-90.0, 0.0, 1.0, 0.0);
	glTranslatef(0.0, 0.0, 0.0575);

	SetMaterial(&BRASS2);
	glClipPlane(GL_CLIP_PLANE1, ClipCrashDome);
	glEnable(GL_CLIP_PLANE1);
	gluSphere(quad, 0.0325, 20, 20);
	glDisable(GL_CLIP_PLANE1);
	glTranslatef(0.0, 0.0, -0.0075);
	gluCylinder(quad, crashcym->size1, 0.029, 0.02, 50, 10);
	glPopMatrix();
}

void DrawRideCymbalF(drum* ridecym){
	GLUquadricObj* quad = gluNewQuadric();
	gluQuadricDrawStyle(quad, DRAWMODE);
	DrawCymbalStand(ridecym);

	glPushMatrix();
	glTranslatef(ridecym->x, ridecym->y, ridecym->E2+0.51);
	glRotatef(90.0, 0.0, 1.0, 0.0);
	glRotatef(-ridecym->R1-90.0, 1.0, 0.0, 0.0);
	glRotatef(ridecym->T2, 0.0, 1.0, 0.0);
	glTranslatef(0.0, 0.0, 0.3+ridecym->E1);
	glRotatef(ridecym->R2, 0.0, 0.0, 1.0);
	glRotatef(ridecym->T1, 0.0, 1.0, 0.0);
	glRotatef(-90.0, 0.0, 1.0, 0.0);
	glTranslatef(0.0, 0.0, 0.047);

	SetMaterial(&BRASS1);
	glClipPlane(GL_CLIP_PLANE1, ClipRideDome);
	glEnable(GL_CLIP_PLANE1);
	gluSphere(quad, 0.042, 20, 20);
	glDisable(GL_CLIP_PLANE1);
	glTranslatef(0.0, 0.0, -0.0075);
	gluCylinder(quad, ridecym->size1, 0.039, 0.02, 50, 10);
	glPopMatrix();
}

void DrawChinaCymbalF(drum* chinacym){
	GLUquadricObj* quad = gluNewQuadric();
	gluQuadricDrawStyle(quad, DRAWMODE);
	DrawCymbalStand(chinacym);

	glPushMatrix();
	glTranslatef(chinacym->x, chinacym->y, chinacym->E2+0.51);
	glRotatef(90.0, 0.0, 1.0, 0.0);
	glRotatef(-chinacym->R1-90.0, 1.0, 0.0, 0.0);
	glRotatef(chinacym->T2, 0.0, 1.0, 0.0);
	glTranslatef(0.0, 0.0, 0.3+chinacym->E1);
	glRotatef(chinacym->R2, 0.0, 0.0, 1.0);
	glRotatef(chinacym->T1, 0.0, 1.0, 0.0);
	glRotatef(-90.0, 0.0, 1.0, 0.0);
	glTranslatef(0.0, 0.0, 0.047);

	SetMaterial(&BRASS1);
	glClipPlane(GL_CLIP_PLANE1, ClipChinaDome);
	glEnable(GL_CLIP_PLANE1);
	glPushMatrix();
	glScalef(chinacym->size1-0.03, chinacym->size1-0.03, 0.04);
	gluSphere(quad, 1.0, 50, 15);
	glDisable(GL_CLIP_PLANE1);
	glPopMatrix();
	glTranslatef(0.0, 0.0, 0.02);
	glClipPlane(GL_CLIP_PLANE1, ClipChinaRim);
	glEnable(GL_CLIP_PLANE1);
	RenderFcn[SolidState+2](0.02, chinacym->size1-0.02, 15, 50);
	glDisable(GL_CLIP_PLANE1);
	glPopMatrix();
}

void DrawCrashCymbalR(drum* crashcym){
	GLUquadricObj* quad = gluNewQuadric();
	gluQuadricDrawStyle(quad, DRAWMODE);
	glPushMatrix();
	glTranslatef(0.0, 0.5, RACKHEIGHT);
	glRotatef(crashcym->x, 0.0, 0.0, 1.0);
	glTranslatef(0.0, -1.5, 0.0);
// mount
	SetMaterial(crashcym->color);
	glPushMatrix();
	glRotatef(90.0, 0.0, 1.0, 0.0);
	glTranslatef(0.0, 0.0, -0.0175);
	gluCylinder(quad, 0.035, 0.035, 0.035, 10, 5);
	SetMaterial(&BLACK);
	gluDisk(quad, 0.02, 0.035, 10, 1);
	glTranslatef(0.0, 0.0, 0.035);
	gluDisk(quad, 0.02, 0.035, 10, 1);
	glPopMatrix();
	glPushMatrix();
	SetMaterial(&STEEL);
	gluCylinder(quad, 0.01, 0.01, 0.06, 10, 4);
	glRotatef(crashcym->R1, 0.0, 0.0, 1.0);
	glTranslatef(-0.0175, 0.0, 0.06);
	glRotatef(90.0, 0.0, 1.0, 0.0);
	gluDisk(quad, 0.0, 0.02, 10, 2);
	gluCylinder(quad, 0.02, 0.02, 0.03, 10, 2);
	glTranslatef(0.0, 0.0, 0.03);
	gluDisk(quad, 0.0, 0.02, 10, 2);
	glTranslatef(0.0, 0.0, -0.015);
	glRotatef(-90.0, 1.0, 0.0, 0.0);
	glRotatef(crashcym->T2, 0.0, 1.0, 0.0);
// boom
	glTranslatef(0.0, 0.0, -0.3+crashcym->E1);
	gluCylinder(quad, 0.007, 0.007, 0.6, 10, 10);
	SetMaterial(&RUBBER);
	RenderFcn[SolidState](0.02);
// head	
	SetMaterial(&STEEL);
	glTranslatef(0.0, 0.0, 0.6);
	glRotatef(crashcym->R2, 0.0, 0.0, 1.0);
	glRotatef(crashcym->T1, 0.0, 1.0, 0.0);
	glPushMatrix();
	glRotatef(90.0, 1.0, 0.0, 0.0);
	glTranslatef(0.0, 0.0, -0.02);
	gluCylinder(quad, 0.02, 0.02, 0.04, 10, 2);
	gluDisk(quad, 0.0, 0.02, 10, 2);
	glTranslatef(0.0, 0.0, 0.04);
	gluDisk(quad, 0.0, 0.02, 10, 2);
	glPopMatrix();
	glRotatef(-90.0, 0.0, 1.0, 0.0);
	gluCylinder(quad, 0.007, 0.007, 0.1, 10, 2);
// cymbal
	glTranslatef(0.0, 0.0, 0.0575);
	SetMaterial(&BRASS2);
	glClipPlane(GL_CLIP_PLANE1, ClipCrashDome);
	glEnable(GL_CLIP_PLANE1);
	gluSphere(quad, 0.0325, 20, 20);
	glDisable(GL_CLIP_PLANE1);
	glTranslatef(0.0, 0.0, -0.0075);
	gluCylinder(quad, crashcym->size1, 0.029, 0.02, 50, 10);
// head
	glTranslatef(0.0, 0.0, 0.04);
	SetMaterial(&RUBBER);
	gluDisk(quad, 0.007, 0.015, 10, 1);
	glTranslatef(0.0, 0.0, 0.01);
	SetMaterial(&STEEL);
	gluDisk(quad, 0.0, 0.007, 10, 1);
	glPopMatrix();
	glPopMatrix();
}

void DrawRideCymbalR(drum* ridecym){
	GLUquadricObj* quad = gluNewQuadric();
	gluQuadricDrawStyle(quad, DRAWMODE);
	glPushMatrix();
	glTranslatef(0.0, 0.5, RACKHEIGHT);
	glRotatef(ridecym->x, 0.0, 0.0, 1.0);
	glTranslatef(0.0, -1.5, 0.0);
// mount
	SetMaterial(ridecym->color);
	glPushMatrix();
	glRotatef(90.0, 0.0, 1.0, 0.0);
	glTranslatef(0.0, 0.0, -0.0175);
	gluCylinder(quad, 0.035, 0.035, 0.035, 10, 5);
	SetMaterial(&BLACK);
	gluDisk(quad, 0.02, 0.035, 10, 1);
	glTranslatef(0.0, 0.0, 0.035);
	gluDisk(quad, 0.02, 0.035, 10, 1);
	glPopMatrix();
	glPushMatrix();
	SetMaterial(&STEEL);
	gluCylinder(quad, 0.01, 0.01, 0.06, 10, 4);
	glRotatef(ridecym->R1, 0.0, 0.0, 1.0);
	glTranslatef(-0.0175, 0.0, 0.06);
	glRotatef(90.0, 0.0, 1.0, 0.0);
	gluDisk(quad, 0.0, 0.02, 10, 2);
	gluCylinder(quad, 0.02, 0.02, 0.03, 10, 2);
	glTranslatef(0.0, 0.0, 0.03);
	gluDisk(quad, 0.0, 0.02, 10, 2);
	glTranslatef(0.0, 0.0, -0.015);
	glRotatef(-90.0, 1.0, 0.0, 0.0);
	glRotatef(ridecym->T2, 0.0, 1.0, 0.0);
// boom
	glTranslatef(0.0, 0.0, -0.3+ridecym->E1);
	gluCylinder(quad, 0.007, 0.007, 0.6, 10, 10);
	SetMaterial(&RUBBER);
	RenderFcn[SolidState](0.02);
// head	
	SetMaterial(&STEEL);
	glTranslatef(0.0, 0.0, 0.6);
	glRotatef(ridecym->R2, 0.0, 0.0, 1.0);
	glRotatef(ridecym->T1, 0.0, 1.0, 0.0);
	glPushMatrix();
	glRotatef(90.0, 1.0, 0.0, 0.0);
	glTranslatef(0.0, 0.0, -0.02);
	gluCylinder(quad, 0.02, 0.02, 0.04, 10, 2);
	gluDisk(quad, 0.0, 0.02, 10, 2);
	glTranslatef(0.0, 0.0, 0.04);
	gluDisk(quad, 0.0, 0.02, 10, 2);
	glPopMatrix();
	glRotatef(-90.0, 0.0, 1.0, 0.0);
	gluCylinder(quad, 0.007, 0.007, 0.1, 10, 2);
// cymbal
	glTranslatef(0.0, 0.0, 0.047);
	SetMaterial(&BRASS1);
	glClipPlane(GL_CLIP_PLANE1, ClipRideDome);
	glEnable(GL_CLIP_PLANE1);
	gluSphere(quad, 0.042, 20, 20);
	glDisable(GL_CLIP_PLANE1);
	glTranslatef(0.0, 0.0, -0.0075);
	gluCylinder(quad, ridecym->size1, 0.039, 0.02, 50, 10);
// head
	glTranslatef(0.0, 0.0, 0.0505);
	SetMaterial(&RUBBER);
	gluDisk(quad, 0.007, 0.015, 10, 1);
	glTranslatef(0.0, 0.0, 0.01);
	SetMaterial(&STEEL);
	gluDisk(quad, 0.0, 0.007, 10, 1);
	glPopMatrix();
	glPopMatrix();
}

void DrawChinaCymbalR(drum* chinacym){
	GLUquadricObj* quad = gluNewQuadric();
	gluQuadricDrawStyle(quad, DRAWMODE);
	glPushMatrix();
	glTranslatef(0.0, 0.5, RACKHEIGHT);
	glRotatef(chinacym->x, 0.0, 0.0, 1.0);
	glTranslatef(0.0, -1.5, 0.0);
// mount
	SetMaterial(chinacym->color);
	glPushMatrix();
	glRotatef(90.0, 0.0, 1.0, 0.0);
	glTranslatef(0.0, 0.0, -0.0175);
	gluCylinder(quad, 0.035, 0.035, 0.035, 10, 5);
	SetMaterial(&BLACK);
	gluDisk(quad, 0.02, 0.035, 10, 1);
	glTranslatef(0.0, 0.0, 0.035);
	gluDisk(quad, 0.02, 0.035, 10, 1);
	glPopMatrix();
	glPushMatrix();
	SetMaterial(&STEEL);
	gluCylinder(quad, 0.01, 0.01, 0.06, 10, 4);
	glRotatef(chinacym->R1, 0.0, 0.0, 1.0);
	glTranslatef(-0.0175, 0.0, 0.06);
	glRotatef(90.0, 0.0, 1.0, 0.0);
	gluDisk(quad, 0.0, 0.02, 10, 2);
	gluCylinder(quad, 0.02, 0.02, 0.03, 10, 2);
	glTranslatef(0.0, 0.0, 0.03);
	gluDisk(quad, 0.0, 0.02, 10, 2);
	glTranslatef(0.0, 0.0, -0.015);
	glRotatef(-90.0, 1.0, 0.0, 0.0);
	glRotatef(chinacym->T2, 0.0, 1.0, 0.0);
// boom
	glTranslatef(0.0, 0.0, -0.3+chinacym->E1);
	gluCylinder(quad, 0.007, 0.007, 0.6, 10, 10);
	SetMaterial(&RUBBER);
	RenderFcn[SolidState](0.02);
// head	
	SetMaterial(&STEEL);
	glTranslatef(0.0, 0.0, 0.6);
	glRotatef(chinacym->R2, 0.0, 0.0, 1.0);
	glRotatef(chinacym->T1, 0.0, 1.0, 0.0);
	glPushMatrix();
	glRotatef(90.0, 1.0, 0.0, 0.0);
	glTranslatef(0.0, 0.0, -0.02);
	gluCylinder(quad, 0.02, 0.02, 0.04, 10, 2);
	gluDisk(quad, 0.0, 0.02, 10, 2);
	glTranslatef(0.0, 0.0, 0.04);
	gluDisk(quad, 0.0, 0.02, 10, 2);
	glPopMatrix();
	glRotatef(-90.0, 0.0, 1.0, 0.0);
	gluCylinder(quad, 0.007, 0.007, 0.1, 10, 2);
// cymbal
	glTranslatef(0.0, 0.0, 0.047);
	SetMaterial(&BRASS1);
	glClipPlane(GL_CLIP_PLANE1, ClipChinaDome);
	glEnable(GL_CLIP_PLANE1);
	glPushMatrix();
	glScalef(chinacym->size1-0.03, chinacym->size1-0.03, 0.04);
	gluSphere(quad, 1.0, 50, 15);
	glDisable(GL_CLIP_PLANE1);
	glPopMatrix();
	glTranslatef(0.0, 0.0, 0.02);
	glClipPlane(GL_CLIP_PLANE1, ClipChinaRim);
	glEnable(GL_CLIP_PLANE1);
	RenderFcn[SolidState+2](0.02, chinacym->size1-0.02, 15, 50);
	glDisable(GL_CLIP_PLANE1);
// head
	glTranslatef(0.0, 0.0, 0.023);
	SetMaterial(&RUBBER);
	gluDisk(quad, 0.007, 0.015, 10, 1);
	glTranslatef(0.0, 0.0, 0.01);
	SetMaterial(&STEEL);
	gluDisk(quad, 0.0, 0.007, 10, 1);
	glPopMatrix();
	glPopMatrix();
}

void DrawSplashCymbal(drum *splashcym){
	GLUquadricObj* quad = gluNewQuadric();
	gluQuadricDrawStyle(quad, DRAWMODE);
	glPushMatrix();
	glTranslatef(0.0, 0.5, RACKHEIGHT);
	glRotatef(splashcym->x, 0.0, 0.0, 1.0);
	glTranslatef(0.0, -1.5, 0.0);
// mount
	SetMaterial(splashcym->color);
	glPushMatrix();
	glRotatef(90.0, 0.0, 1.0, 0.0);
	glTranslatef(0.0, 0.0, -0.0175);
	gluCylinder(quad, 0.035, 0.035, 0.035, 10, 5);
	SetMaterial(&BLACK);
	gluDisk(quad, 0.02, 0.035, 10, 1);
	glTranslatef(0.0, 0.0, 0.035);
	gluDisk(quad, 0.02, 0.035, 10, 1);
	glPopMatrix();
	glPushMatrix();
	SetMaterial(&STEEL);
	gluCylinder(quad, 0.01, 0.01, 0.06, 10, 4);
	glRotatef(splashcym->R1, 0.0, 0.0, 1.0);
	glTranslatef(-0.0175, 0.0, 0.06);
	glRotatef(90.0, 0.0, 1.0, 0.0);
	gluDisk(quad, 0.0, 0.02, 10, 2);
	gluCylinder(quad, 0.02, 0.02, 0.03, 10, 2);
	glTranslatef(0.0, 0.0, 0.03);
	gluDisk(quad, 0.0, 0.02, 10, 2);
	glTranslatef(0.0, 0.0, -0.015);
	glRotatef(-90.0, 1.0, 0.0, 0.0);
	glRotatef(splashcym->T2, 0.0, 1.0, 0.0);
// boom
	glTranslatef(0.0, 0.0, -0.3+splashcym->E1);
	gluCylinder(quad, 0.007, 0.007, 0.6, 10, 10);
	SetMaterial(&RUBBER);
	RenderFcn[SolidState](0.02);
// head	
	SetMaterial(&STEEL);
	glTranslatef(0.0, 0.0, 0.6);
	glRotatef(splashcym->R2, 0.0, 0.0, 1.0);
	glRotatef(splashcym->T1, 0.0, 1.0, 0.0);
	glPushMatrix();
	glRotatef(90.0, 1.0, 0.0, 0.0);
	glTranslatef(0.0, 0.0, -0.02);
	gluCylinder(quad, 0.02, 0.02, 0.04, 10, 2);
	gluDisk(quad, 0.0, 0.02, 10, 2);
	glTranslatef(0.0, 0.0, 0.04);
	gluDisk(quad, 0.0, 0.02, 10, 2);
	glPopMatrix();
	glRotatef(-90.0, 0.0, 1.0, 0.0);
	gluCylinder(quad, 0.007, 0.007, 0.1, 10, 2);
// cymbal
	glTranslatef(0.0, 0.0, 0.0675);
	SetMaterial(&BRASS2);
	gluCylinder(quad, splashcym->size1, 0.007, 0.02, 50, 10);
// head
	glTranslatef(0.0, 0.0, 0.0225);
	SetMaterial(&RUBBER);
	gluDisk(quad, 0.007, 0.015, 10, 1);
	glTranslatef(0.0, 0.0, 0.01);
	SetMaterial(&STEEL);
	gluDisk(quad, 0.0, 0.007, 10, 1);
	glPopMatrix();
	glPopMatrix();
}

void DrawBellCymbal(drum *bellcym){
	GLUquadricObj* quad = gluNewQuadric();
	gluQuadricDrawStyle(quad, DRAWMODE);
	glPushMatrix();
	glTranslatef(0.0, 0.5, RACKHEIGHT);
	glRotatef(bellcym->x, 0.0, 0.0, 1.0);
	glTranslatef(0.0, -1.5, 0.0);
// mount
	SetMaterial(bellcym->color);
	glPushMatrix();
	glRotatef(90.0, 0.0, 1.0, 0.0);
	glTranslatef(0.0, 0.0, -0.0175);
	gluCylinder(quad, 0.035, 0.035, 0.035, 10, 5);
	SetMaterial(&BLACK);
	gluDisk(quad, 0.02, 0.035, 10, 1);
	glTranslatef(0.0, 0.0, 0.035);
	gluDisk(quad, 0.02, 0.035, 10, 1);
	glPopMatrix();
	glPushMatrix();
	SetMaterial(&STEEL);
	gluCylinder(quad, 0.01, 0.01, 0.06, 10, 4);
	glRotatef(bellcym->R1, 0.0, 0.0, 1.0);
	glTranslatef(-0.0175, 0.0, 0.06);
	glRotatef(90.0, 0.0, 1.0, 0.0);
	gluDisk(quad, 0.0, 0.02, 10, 2);
	gluCylinder(quad, 0.02, 0.02, 0.03, 10, 2);
	glTranslatef(0.0, 0.0, 0.03);
	gluDisk(quad, 0.0, 0.02, 10, 2);
	glTranslatef(0.0, 0.0, -0.015);
	glRotatef(-90.0, 1.0, 0.0, 0.0);
	glRotatef(bellcym->T2, 0.0, 1.0, 0.0);
// boom
	glTranslatef(0.0, 0.0, -0.3+bellcym->E1);
	gluCylinder(quad, 0.007, 0.007, 0.6, 10, 10);
	SetMaterial(&RUBBER);
	RenderFcn[SolidState](0.02);
// head	
	SetMaterial(&STEEL);
	glTranslatef(0.0, 0.0, 0.6);
	glRotatef(bellcym->R2, 0.0, 0.0, 1.0);
	glRotatef(bellcym->T1, 0.0, 1.0, 0.0);
	glPushMatrix();
	glRotatef(90.0, 1.0, 0.0, 0.0);
	glTranslatef(0.0, 0.0, -0.02);
	gluCylinder(quad, 0.02, 0.02, 0.04, 10, 2);
	gluDisk(quad, 0.0, 0.02, 10, 2);
	glTranslatef(0.0, 0.0, 0.04);
	gluDisk(quad, 0.0, 0.02, 10, 2);
	glPopMatrix();
	glRotatef(-90.0, 0.0, 1.0, 0.0);
	gluCylinder(quad, 0.007, 0.007, 0.1, 10, 2);
// cymbal
	glTranslatef(0.0, 0.0, 0.055);
	SetMaterial(&BRASS1);
	glClipPlane(GL_CLIP_PLANE1, ClipZ);
	glEnable(GL_CLIP_PLANE1);
	glPushMatrix();
	glScalef(bellcym->size1, bellcym->size1, 0.035);
	gluSphere(quad, 1.0, 50, 15);
	glDisable(GL_CLIP_PLANE1);
	glPopMatrix();
// head
	glTranslatef(0.0, 0.0, 0.035);
	SetMaterial(&RUBBER);
	gluDisk(quad, 0.007, 0.015, 10, 1);
	glTranslatef(0.0, 0.0, 0.01);
	SetMaterial(&STEEL);
	gluDisk(quad, 0.0, 0.007, 10, 1);
	glPopMatrix();
	glPopMatrix();
}

void DrawCymbalStand(drum* stand){
	GLUquadricObj* quad = gluNewQuadric();
	int i;
	GLfloat angle = -45.0;
	gluQuadricDrawStyle(quad, DRAWMODE);
	glPushMatrix();
	glTranslatef(stand->x, stand->y, 0.01);
// shaft
	glPushMatrix();
	glTranslatef(0.0, 0.0, 0.04);
	SetMaterial(&STEEL);
	gluCylinder(quad, 0.01, 0.01, 0.4, 10, 1);
	glTranslatef(0.0, 0.0, 0.4);
	SetMaterial(stand->color);
	gluCylinder(quad, 0.01, 0.02, 0.04, 10, 2);
	glTranslatef(0.0, 0.0, 0.04);
	gluDisk(quad, 0.01, 0.02, 10, 1);
	SetMaterial(&STEEL);
	gluCylinder(quad, 0.01, 0.01, stand->E2+0.01, 10, 8);
	glTranslatef(0.0, 0.0, stand->E2+0.01);
// boom
	glPushMatrix();
	glTranslatef(0.0, 0.0, 0.01);
	glRotatef(90.0, 1.0, 0.0, 0.0);
	glRotatef(stand->R1+90.0, 0.0, 1.0, 0.0);
	glTranslatef(0.0, 0.0, -0.02);
	gluCylinder(quad, 0.02, 0.02, 0.04, 10, 2);
	gluDisk(quad, 0.0, 0.02, 10, 2);
	glTranslatef(0.0, 0.0, 0.04);
	gluDisk(quad, 0.0, 0.02, 10, 2);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.0, 0.0, 0.01);	
	glRotatef(90.0, 0.0, 1.0, 0.0);
	glRotatef(-stand->R1-90.0, 1.0, 0.0, 0.0);
	glRotatef(stand->T2, 0.0, 1.0, 0.0);
	glTranslatef(0.0, 0.0, -0.30+stand->E1);
	gluCylinder(quad, 0.007, 0.007, 0.6, 10, 10);
	SetMaterial(&RUBBER);
	RenderFcn[SolidState](0.02);
// head	
	SetMaterial(&STEEL);
	glTranslatef(0.0, 0.0, 0.6);
	glRotatef(stand->R2, 0.0, 0.0, 1.0);
	glRotatef(stand->T1, 0.0, 1.0, 0.0);

	glPushMatrix();
	glRotatef(90.0, 1.0, 0.0, 0.0);
	glTranslatef(0.0, 0.0, -0.02);
	gluCylinder(quad, 0.02, 0.02, 0.04, 10, 2);
	gluDisk(quad, 0.0, 0.02, 10, 2);
	glTranslatef(0.0, 0.0, 0.04);
	gluDisk(quad, 0.0, 0.02, 10, 2);
	glPopMatrix();

	glRotatef(-90.0, 0.0, 1.0, 0.0);
	gluCylinder(quad, 0.007, 0.007, 0.1, 10, 2);
	glTranslatef(0.0, 0.0, 0.09);
	SetMaterial(&RUBBER);
	gluDisk(quad, 0.007, 0.015, 10, 1);
	glTranslatef(0.0, 0.0, 0.01);
	SetMaterial(&STEEL);
	gluDisk(quad, 0.0, 0.007, 10, 1);
	glPopMatrix();
	glPopMatrix();
// legs
	SetMaterial(&STEEL);
	for(i = 0; i < 3; i++){
		glPushMatrix();
		glRotatef(angle, 0.0, 0.0, 1.0);
		glTranslatef(0.127, 0.0, 0.127);
		glRotatef(-45.0, 0.0, 1.0, 0.0);
		glScalef(0.015, 0.005, 0.36);
		RenderFcn[SolidState](1.0);
		glPopMatrix();
		angle += 120.0;
	}
	for(i = 0; i < 3; i++){
		glPushMatrix();
		glRotatef(angle, 0.0, 0.0, 1.0);
		glTranslatef(0.05, 0.0, 0.1);
		glRotatef(45.0, 0.0, 1.0, 0.0);
		glScalef(0.01, 0.005, 0.14);
		RenderFcn[SolidState](1.0);
		glPopMatrix();
		angle += 120.0;
	}
	SetMaterial(&RUBBER);
	for(i = 0; i < 3; i++){
		glPushMatrix();
		glRotatef(angle, 0.0, 0.0, 1.0);
		glTranslatef(0.255, 0.0, 0.0);
		glMultMatrixd(Shear);
		glScalef(0.025, 0.01, 0.02);
		RenderFcn[SolidState](1.0);
		glPopMatrix();
		angle += 120.0;
	}
glPopMatrix();
}

void DrawHiHatStand(drum* stand){
	GLUquadricObj* quad = gluNewQuadric();
	int i;
	GLfloat angle = -45.0;
	gluQuadricDrawStyle(quad, DRAWMODE);
	glPushMatrix();
	glTranslatef(stand->x, stand->y, 0.01);
	glRotatef(stand->R1, 0.0, 0.0, 1.0);
// shaft
	glPushMatrix();
	glTranslatef(0.0, 0.0, 0.04);
	SetMaterial(&STEEL);
	gluCylinder(quad, 0.01, 0.01, 0.36, 10, 1);
	glTranslatef(0.0, 0.0, 0.36);
	SetMaterial(stand->color);
	gluCylinder(quad, 0.01, 0.02, 0.04, 10, 2);
	glTranslatef(0.0, 0.0, 0.04);
	gluDisk(quad, 0.01, 0.02, 10, 1);
	SetMaterial(&STEEL);
	gluCylinder(quad, 0.01, 0.01, stand->E2+0.01, 10, 8);
	glTranslatef(0.0, 0.0, stand->E2+0.01);
	SetMaterial(&RUBBER);
	gluDisk(quad, 0.0, 0.02, 10, 2);
	SetMaterial(&STEEL);
	gluCylinder(quad, 0.005, 0.005, 0.0225, 10, 1);
	glTranslatef(0.0, 0.0, 0.0225);
	gluCylinder(quad, 0.005, 0.01, 0.01, 10, 1);
	glTranslatef(0.0, 0.0, 0.01);
	SetMaterial(&RUBBER);
	gluCylinder(quad, 0.01, 0.01, 0.02, 10, 1);
	glTranslatef(0.0, 0.0, 0.02);
	SetMaterial(&STEEL);
	gluCylinder(quad, 0.01, 0.005, 0.01, 10, 1);
	glTranslatef(0.0, 0.0, 0.01);
	gluCylinder(quad, 0.005, 0.005, 0.16, 10, 3);
	glTranslatef(0.0, 0.0, 0.16);
	gluDisk(quad, 0.0, 0.005, 10, 1); 
	glPopMatrix();
// legs
	SetMaterial(&STEEL);
	for(i = 0; i < 3; i++){
		glPushMatrix();
		glRotatef(angle, 0.0, 0.0, 1.0);
		glTranslatef(0.127, 0.0, 0.127);
		glRotatef(-45.0, 0.0, 1.0, 0.0);
		glScalef(0.015, 0.005, 0.36);
		RenderFcn[SolidState](1.0);
		glPopMatrix();
		angle += 120.0;
	}
	for(i = 0; i < 3; i++){
		glPushMatrix();
		glRotatef(angle, 0.0, 0.0, 1.0);
		glTranslatef(0.05, 0.0, 0.1);
		glRotatef(45.0, 0.0, 1.0, 0.0);
		glScalef(0.01, 0.005, 0.14);
		RenderFcn[SolidState](1.0);
		glPopMatrix();
		angle += 120.0;
	}
	SetMaterial(&RUBBER);
	for(i = 0; i < 3; i++){
		glPushMatrix();
		glRotatef(angle, 0.0, 0.0, 1.0);
		glTranslatef(0.255, 0.0, 0.0);
		glMultMatrixd(Shear);
		glScalef(0.025, 0.01, 0.02);
		RenderFcn[SolidState](1.0);
		glPopMatrix();
		angle += 120.0;
	}
// pedal
	glTranslatef(0.0, 0.0, -0.01);
	glPushMatrix();
	glRotatef(45.0+stand->R2, 0.0, 0.0, 1.0);
	glPushMatrix();
	glClipPlane(GL_CLIP_PLANE0, ClipY);
	glEnable(GL_CLIP_PLANE0);
	glScalef(0.05, 0.2, 1.0);
	gluCylinder(quad, 1.0, 1.0, 0.005, 40, 1);
	glTranslatef(0.0, 0.0, 0.005);
	gluDisk(quad, 0.0, 1.0, 40, 3);
	glDisable(GL_CLIP_PLANE0);
	glPopMatrix();
	SetMaterial(&STEEL);
	glPushMatrix();
	glTranslatef(0.0, -0.01, 0.0025);
	glScalef(0.1, 0.02, 0.005);
	RenderFcn[SolidState](1.0);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(0.0, 0.16, 0.005);
	glRotatef(90.0, 0.0, 1.0, 0.0);
	glTranslatef(0.0, 0.0, -0.03);
	gluCylinder(quad, 0.0, 0.005, 0.01, 10, 2);
	glTranslatef(0.0, 0.0, 0.01);
	gluCylinder(quad, 0.005, 0.005, 0.04, 10, 4);
	glTranslatef(0.0, 0.0, 0.04);
	gluCylinder(quad, 0.005, 0.0, 0.01, 10, 2);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(0.04, 0.0, 0.005);
	gluCylinder(quad, 0.005, 0.005, 0.04, 8, 4);
	glTranslatef(-0.08, 0.0, 0.0);
	gluCylinder(quad, 0.005, 0.005, 0.04, 8, 4);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(0.0, 0.0, 0.05);
	glRotatef(90.0, 0.0, 1.0, 0.0);
	glTranslatef(0.0, 0.0, -0.045);
	gluDisk(quad, 0.0, 0.01, 10, 2);
	gluCylinder(quad, 0.01, 0.01, 0.09, 10, 2);
	glTranslatef(0.0, 0.0, 0.09);
	gluDisk(quad, 0.0, 0.01, 10, 2);
	glPopMatrix();
	SetMaterial(stand->color);
	glTranslatef(0.0, 0.16, 0.005);
	glRotatef(-11.0, 1.0, 0.0, 0.0);
	glClipPlane(GL_CLIP_PLANE0, ClipPedal);
	glEnable(GL_CLIP_PLANE0);
	glTranslatef(0.0, -0.08, 0.0);
	glPushMatrix();
	glScalef(0.035, 0.09, 1.0);
	gluDisk(quad, 0.0, 1.0, 30, 5);
	gluCylinder(quad, 1.0, 1.0, 0.005, 30, 1);
	glTranslatef(0.0, 0.0, 0.005);
	gluDisk(quad, 0.0, 1.0, 30, 5);
	glPopMatrix();
	glDisable(GL_CLIP_PLANE0);
	glPopMatrix();
	glPopMatrix();
}

void DrawSnareStand(drum* stand){
	GLUquadricObj* quad = gluNewQuadric();
	int i;
	GLfloat angle = -45.0;
	gluQuadricDrawStyle(quad, DRAWMODE);
// shaft
	glPushMatrix();
	glTranslatef(stand->x, stand->y, 0.0);
	glPushMatrix();
	glTranslatef(0.0, 0.0, 0.03);
	SetMaterial(&STEEL);
	gluCylinder(quad, 0.01, 0.01, 0.15, 10, 1);
	glTranslatef(0.0, 0.0, 0.15);
	SetMaterial(stand->color);
	gluCylinder(quad, 0.01, 0.02, 0.03, 10, 2);
	glTranslatef(0.0, 0.0, 0.03);
	gluDisk(quad, 0.01, 0.02, 10, 1);
	SetMaterial(&STEEL);
	gluCylinder(quad, 0.01, 0.01, stand->E1+0.01, 10, 3);
	glTranslatef(0.0, 0.0, stand->E1+0.025);
	gluSphere(quad, 0.02, 10, 5);
	glRotatef(stand->R1, 0.0, 0.0, 1.0);
	glRotatef(stand->T2, 1.0, 0.0, 0.0);
	gluCylinder(quad, 0.01, 0.01, 0.06, 10, 2);
	glTranslatef(0.0, 0.0, 0.06);
	gluDisk(quad, 0.0, 0.02, 10, 2);

// grips
	for(i = 0; i < 3; i++){
		glPushMatrix();
		glRotatef(angle, 0.0, 0.0, 1.0);
		glTranslatef(0.075, 0.0, -0.0025);
		glScalef(0.15, 0.01, 0.005);
		RenderFcn[SolidState](1.0);
		glPopMatrix();
		angle += 120.0;
	}
	SetMaterial(&RUBBER);
	for(i = 0; i < 3; i++){
		glPushMatrix();
		glRotatef(angle, 0.0, 0.0, 1.0);
		glTranslatef(0.1525, 0.0, 0.005);
		glScalef(0.005, 0.01, 0.02);
		RenderFcn[SolidState](1.0);
		glPopMatrix();
		angle += 120.0;
	}
	glPopMatrix();
// legs
	SetMaterial(&STEEL);
	for(i = 0; i < 3; i++){
		glPushMatrix();
		glRotatef(angle, 0.0, 0.0, 1.0);
		glTranslatef(0.06, 0.0, 0.07);
		glRotatef(-45.0, 0.0, 1.0, 0.0);
		glScalef(0.015, 0.005, 0.17);
		RenderFcn[SolidState](1.0);
		glPopMatrix();
		angle += 120.0;
	}
	for(i = 0; i < 3; i++){
		glPushMatrix();
		glRotatef(angle, 0.0, 0.0, 1.0);
		glTranslatef(0.02, 0.0, 0.06);
		glRotatef(45.0, 0.0, 1.0, 0.0);
		glScalef(0.01, 0.005, 0.064);
		RenderFcn[SolidState](1.0);
		glPopMatrix();
		angle += 120.0;
	}
	SetMaterial(&RUBBER);
	for(i = 0; i < 3; i++){
		glPushMatrix();
		glRotatef(angle, 0.0, 0.0, 1.0);
		glTranslatef(0.12, 0.0, 0.01);
		glMultMatrixd(Shear);
		glScalef(0.025, 0.01, 0.02);
		RenderFcn[SolidState](1.0);
		glPopMatrix();
		angle += 120.0;
	}
	glPopMatrix();
}

void DrawRackMount(drum *mount){
	GLUquadricObj* quad = gluNewQuadric();
	gluQuadricDrawStyle(quad, DRAWMODE);
	glPushMatrix();
	glTranslatef(0.0, 0.5, RACKHEIGHT);
	glRotatef(mount->x, 0.0, 0.0, 1.0);
	glTranslatef(0.0, -1.5, 0.0);
// mount
	SetMaterial(&STEEL);
	glPushMatrix();
	glRotatef(90.0, 0.0, 1.0, 0.0);
	glTranslatef(0.0, 0.0, -0.0175);
	SetMaterial(mount->color);
	gluCylinder(quad, 0.035, 0.035, 0.035, 10, 5);
	SetMaterial(&BLACK);
	gluDisk(quad, 0.02, 0.035, 10, 1);
	glTranslatef(0.0, 0.0, 0.035);
	gluDisk(quad, 0.02, 0.035, 10, 1);
	glPopMatrix();
	glPushMatrix();
	SetMaterial(&STEEL);
	gluCylinder(quad, 0.01, 0.01, 0.06, 10, 4);
	glRotatef(mount->R1, 0.0, 0.0, 1.0);
	glTranslatef(-0.0175, 0.0, 0.06);
	glRotatef(90.0, 0.0, 1.0, 0.0);
	gluDisk(quad, 0.0, 0.02, 10, 2);
	gluCylinder(quad, 0.02, 0.02, 0.03, 10, 2);
	glTranslatef(0.0, 0.0, 0.03);
	gluDisk(quad, 0.0, 0.02, 10, 2);
	glTranslatef(0.0, 0.0, -0.015);
	glRotatef(-90.0, 1.0, 0.0, 0.0);
	glRotatef(mount->T2, 0.0, 1.0, 0.0);
// boom
	glTranslatef(0.0, 0.0, -0.3+mount->E1);
	gluCylinder(quad, 0.007, 0.007, 0.6, 10, 10);
	SetMaterial(&RUBBER);
	RenderFcn[SolidState](0.02);
// head	
	SetMaterial(&STEEL);
	glTranslatef(0.0, 0.0, 0.6);
	glRotatef(mount->R2, 0.0, 0.0, 1.0);
	glRotatef(mount->T1, 0.0, 1.0, 0.0);

	glPushMatrix();
	glRotatef(90.0, 1.0, 0.0, 0.0);
	glTranslatef(0.0, 0.0, -0.02);
	gluCylinder(quad, 0.02, 0.02, 0.04, 10, 2);
	gluDisk(quad, 0.0, 0.02, 10, 2);
	glTranslatef(0.0, 0.0, 0.04);
	gluDisk(quad, 0.0, 0.02, 10, 2);
	glPopMatrix();

	glRotatef(-90.0, 0.0, 1.0, 0.0);
	gluCylinder(quad, 0.007, 0.007, 0.1, 10, 2);
	glTranslatef(0.0, 0.0, 0.09);
	SetMaterial(&RUBBER);
	gluDisk(quad, 0.007, 0.015, 10, 1);
	glTranslatef(0.0, 0.0, 0.01);
	SetMaterial(&STEEL);
	gluDisk(quad, 0.0, 0.007, 10, 1);
	glPopMatrix();
	glPopMatrix();
}

void DrawBassPedal(drum *pedal){
	GLUquadricObj* quad = gluNewQuadric();
	int i;
	GLfloat chain = 0.005;
	gluQuadricDrawStyle(quad, DRAWMODE);
	glPushMatrix();
	glTranslatef(pedal->x, pedal->y, 0.0);
	glRotatef(pedal->R1, 0.0, 0.0, 1.0);
// base
	SetMaterial(&RUBBER);
	glPushMatrix();
	glClipPlane(GL_CLIP_PLANE0, ClipY);
	glEnable(GL_CLIP_PLANE0);
	glScalef(0.05, 0.22, 1.0);
	gluCylinder(quad, 1.0, 1.0, 0.005, 40, 1);
	glTranslatef(0.0, 0.0, 0.005);
	gluDisk(quad, 0.0, 1.0, 40, 3);
	glDisable(GL_CLIP_PLANE0);
	glPopMatrix();
	SetMaterial(&STEEL);
	glPushMatrix();
	glTranslatef(0.0, -0.01, 0.005);
	glScalef(0.11, 0.02, 0.01);
	RenderFcn[SolidState](1.0);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(0.04, -0.03, 0.0025); glScalef(0.02, 0.02, 0.005);
	RenderFcn[SolidState](1.0);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(-0.04, -0.03, 0.0025); glScalef(0.02, 0.02, 0.005);
	RenderFcn[SolidState](1.0);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(0.0, -0.02, 0.0125); glScalef(0.02, 0.04, 0.005);
	RenderFcn[SolidState](1.0);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(0.0, 0.16, 0.005);
	glRotatef(90.0, 0.0, 1.0, 0.0);
	glTranslatef(0.0, 0.0, -0.035);
	gluCylinder(quad, 0.0, 0.005, 0.01, 10, 2);
	glTranslatef(0.0, 0.0, 0.01);
	gluCylinder(quad, 0.005, 0.005, 0.05, 10, 4);
	glTranslatef(0.0, 0.0, 0.05);
	gluCylinder(quad, 0.005, 0.0, 0.01, 10, 2);
	glPopMatrix();
// arch
	glPushMatrix();
	glTranslatef(0.04, -0.01, 0.01);
	gluCylinder(quad, 0.005, 0.005, 0.11, 8, 4);
	glTranslatef(-0.08, 0.0, 0.0);
	gluCylinder(quad, 0.005, 0.005, 0.11, 8, 4);
	glTranslatef(-0.009, 0.0, 0.0);
	gluCylinder(quad, 0.005, 0.005, 0.01, 6, 1);
	glTranslatef(0.0, 0.0, 0.01);
	gluDisk(quad, 0.0, 0.005, 6, 1);
	SetMaterial(&RUBBER);
	glBegin(GL_LINES);
	glVertex3f(0.0, 0.0, 0.0); glVertex3f(0.0, 0.0, 0.1);
	glEnd();
	glTranslatef(0.0, 0.0, 0.04);
	gluCylinder(quad, 0.003, 0.003, 0.02, 8, 2);
	glPopMatrix();
	glPushMatrix();
	SetMaterial(&STEEL);
	glTranslatef(0.0, -0.01, 0.12);
	glRotatef(90.0, 0.0, 1.0, 0.0);
	glTranslatef(0.0, 0.0, -0.055);
	gluDisk(quad, 0.0, 0.01, 10, 2);
	gluCylinder(quad, 0.01, 0.01, 0.02, 10, 2);
	glTranslatef(0.0, 0.0, 0.02);
	gluDisk(quad, 0.005, 0.01, 10, 2);
	gluCylinder(quad, 0.005, 0.005, 0.03, 10, 5);
	glTranslatef(0.0, 0.0, 0.03);
	gluDisk(quad, 0.005, 0.015, 20, 2);
	SetMaterial(&RUBBER);
	gluCylinder(quad, 0.015, 0.015, 0.01, 20, 1);
	glTranslatef(0.0, 0.0, 0.01);
	SetMaterial(&STEEL);
	gluDisk(quad, 0.005, 0.015, 20, 2);
	gluCylinder(quad, 0.005, 0.005, 0.03, 10, 5);
	glTranslatef(0.0, 0.0, 0.03);
	gluDisk(quad, 0.005, 0.01, 10, 2);
	gluCylinder(quad, 0.01, 0.01, 0.015, 10, 2);
	glTranslatef(0.0, 0.0, 0.015);
	gluDisk(quad, 0.0, 0.01, 10, 2);
	glPopMatrix();
// beater
	glPushMatrix();
	glTranslatef(0.0, -0.01, 0.12);
	glRotatef(-30.0, 1.0, 0.0, 0.0);
	gluCylinder(quad, 0.0025, 0.0025, 0.08, 6, 2);
	SetMaterial(&RUBBER);
	glTranslatef(0.0, 0.0, 0.08);
	glScalef(0.012, 0.015, 0.012);
	gluSphere(quad, 1.0, 8, 8);
	glPopMatrix();
// pedal	
	SetMaterial(pedal->color);
	glTranslatef(0.0, 0.16, 0.005);
	glRotatef(-15.0, 1.0, 0.0, 0.0);
	glClipPlane(GL_CLIP_PLANE0, ClipPedal);
	glEnable(GL_CLIP_PLANE0);
	glTranslatef(0.0, -0.08, 0.0);
	glPushMatrix();
	glScalef(0.04, 0.11, 1.0);
	gluDisk(quad, 0.0, 1.0, 30, 5);
	gluCylinder(quad, 1.0, 1.0, 0.005, 30, 1);
	glTranslatef(0.0, 0.0, 0.005);
	gluDisk(quad, 0.0, 1.0, 30, 5);
	glPopMatrix();
	glDisable(GL_CLIP_PLANE0);
	SetMaterial(&RUBBER);
	glTranslatef(0.0, -0.11, 0.003);
	RenderFcn[SolidState](0.006);
// chain
	glRotatef(15.0, 1.0, 0.0, 0.0);
	glBegin(GL_LINES);
	glVertex3f(0.0, -0.002, 0.0); glVertex3f(0.0, -0.002, 0.06);
	glVertex3f(0.002, -0.002, 0.0); glVertex3f(0.002, -0.002, 0.06);
	glVertex3f(-0.002, -0.002, 0.0); glVertex3f(-0.002, -0.002, 0.06);
	for(i = 0; i<9; i++){
		glVertex3f(0.002, -0.002, chain); glVertex3f(-0.002, -0.002, chain);
		chain += 0.006;
	}
	glEnd();
	glPopMatrix();
}

void DrawDoublePedal(drum *pedal){
	GLUquadricObj* quad = gluNewQuadric();
	int i;
	GLfloat chain = 0.005;
	gluQuadricDrawStyle(quad, DRAWMODE);
	glPushMatrix();
	glTranslatef(pedal->x, pedal->y, 0.0);
	glRotatef(pedal->R1, 0.0, 0.0, 1.0);
	glPushMatrix();
	SetMaterial(&STEEL);
	glTranslatef(0.0, 0.0, 0.12);
	glRotatef(90.0, 0.0, 1.0, 0.0);
	glTranslatef(0.0, 0.0, -0.06 - 0.5*pedal->E2);
	gluCylinder(quad, 0.007, 0.007, 0.12 + pedal->E2, 8, 10);
	glPopMatrix();
// pedal 1
	glPushMatrix();
	glTranslatef(-0.06 - 0.5*pedal->E2, 0.0, 0.0);
	glRotatef(pedal->T1, 0.0, 0.0, 1.0);
	glTranslatef(-0.04, 0.01, 0.0);
// base
	SetMaterial(&RUBBER);
	glPushMatrix();
	glClipPlane(GL_CLIP_PLANE0, ClipY);
	glEnable(GL_CLIP_PLANE0);
	glScalef(0.05, 0.22, 1.0);
	gluCylinder(quad, 1.0, 1.0, 0.005, 40, 1);
	glTranslatef(0.0, 0.0, 0.005);
	gluDisk(quad, 0.0, 1.0, 40, 3);
	glDisable(GL_CLIP_PLANE0);
	glPopMatrix();
	SetMaterial(&STEEL);
	glPushMatrix();
	glTranslatef(0.0, -0.01, 0.005);
	glScalef(0.11, 0.02, 0.01);
	RenderFcn[SolidState](1.0);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(0.04, -0.03, 0.0025); glScalef(0.02, 0.02, 0.005);
	RenderFcn[SolidState](1.0);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(-0.04, -0.03, 0.0025); glScalef(0.02, 0.02, 0.005);
	RenderFcn[SolidState](1.0);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(0.0, -0.02, 0.0125); glScalef(0.02, 0.04, 0.005);
	RenderFcn[SolidState](1.0);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(0.0, 0.16, 0.005);
	glRotatef(90.0, 0.0, 1.0, 0.0);
	glTranslatef(0.0, 0.0, -0.035);
	gluCylinder(quad, 0.0, 0.005, 0.01, 10, 2);
	glTranslatef(0.0, 0.0, 0.01);
	gluCylinder(quad, 0.005, 0.005, 0.05, 10, 4);
	glTranslatef(0.0, 0.0, 0.05);
	gluCylinder(quad, 0.005, 0.0, 0.01, 10, 2);
	glPopMatrix();
// arch
	glPushMatrix();
	glTranslatef(0.04, -0.01, 0.01);
	gluCylinder(quad, 0.005, 0.005, 0.11, 8, 4);
	glTranslatef(-0.08, 0.0, 0.0);
	gluCylinder(quad, 0.005, 0.005, 0.11, 8, 4);
	glTranslatef(-0.009, 0.0, 0.0);
	gluCylinder(quad, 0.005, 0.005, 0.01, 6, 1);
	glTranslatef(0.0, 0.0, 0.01);
	gluDisk(quad, 0.0, 0.005, 6, 1);
	SetMaterial(&RUBBER);
	glBegin(GL_LINES);
	glVertex3f(0.0, 0.0, 0.0); glVertex3f(0.0, 0.0, 0.1);
	glEnd();
	glTranslatef(0.0, 0.0, 0.04);
	gluCylinder(quad, 0.003, 0.003, 0.02, 8, 2);
	glPopMatrix();
	glPushMatrix();
	SetMaterial(&STEEL);
	glTranslatef(0.0, -0.01, 0.12);
	glRotatef(90.0, 0.0, 1.0, 0.0);
	glTranslatef(0.0, 0.0, -0.055);
	gluDisk(quad, 0.0, 0.01, 10, 2);
	gluCylinder(quad, 0.01, 0.01, 0.02, 10, 2);
	glTranslatef(0.0, 0.0, 0.02);
	gluDisk(quad, 0.005, 0.01, 20, 1);
	gluCylinder(quad, 0.005, 0.005, 0.015, 10, 5);
	glTranslatef(0.0, 0.0, 0.015);
	gluDisk(quad, 0.005, 0.01, 20, 1);
	gluCylinder(quad, 0.01, 0.01, 0.01, 20, 1);
	glTranslatef(0.0, 0.0, 0.01);
	gluDisk(quad, 0.01, 0.015, 20, 1);
	SetMaterial(&RUBBER);
	gluCylinder(quad, 0.015, 0.015, 0.015, 20, 1);
	SetMaterial(&STEEL);
	glTranslatef(0.0, 0.0, 0.015);
	gluDisk(quad, 0.005, 0.015, 20, 1);
	gluCylinder(quad, 0.005, 0.005, 0.01, 10, 5);
	glTranslatef(0.0, 0.0, 0.01);
	gluDisk(quad, 0.005, 0.01, 20, 1);
	gluCylinder(quad, 0.01, 0.01, 0.01, 10, 5);
	glTranslatef(0.0, 0.0, 0.01);
	gluDisk(quad, 0.005, 0.01, 20, 1);
	gluCylinder(quad, 0.005, 0.005, 0.01, 10, 5);
	glTranslatef(0.0, 0.0, 0.01);
	gluDisk(quad, 0.005, 0.01, 20, 1);
	gluCylinder(quad, 0.01, 0.01, 0.015, 10, 2);
	glTranslatef(0.0, 0.0, 0.015);
	gluDisk(quad, 0.0, 0.01, 20, 1);
	glPopMatrix();
// beaters
	glPushMatrix();
	glTranslatef(-0.015, -0.01, 0.12);
	glRotatef(-30.0, 1.0, 0.0, 0.0);
	gluCylinder(quad, 0.0025, 0.0025, 0.08, 6, 2);
	glTranslatef(0.035, 0.0, 0.0);
	gluCylinder(quad, 0.0025, 0.0025, 0.08, 6, 2);
	SetMaterial(&RUBBER);
	glTranslatef(0.0, 0.0, 0.08);
	glPushMatrix();
	glScalef(0.012, 0.015, 0.012);
	gluSphere(quad, 1.0, 8, 8);
	glPopMatrix();
	glTranslatef(-0.035, 0.0, 0.0);
	glScalef(0.012, 0.015, 0.012);
	gluSphere(quad, 1.0, 8, 8);
	glPopMatrix();
// pedal	
	SetMaterial(pedal->color);
	glTranslatef(0.0, 0.16, 0.005);
	glRotatef(-15.0, 1.0, 0.0, 0.0);
	glClipPlane(GL_CLIP_PLANE0, ClipPedal);
	glEnable(GL_CLIP_PLANE0);
	glTranslatef(0.0, -0.08, 0.0);
	glPushMatrix();
	glScalef(0.04, 0.11, 1.0);
	gluDisk(quad, 0.0, 1.0, 30, 5);
	gluCylinder(quad, 1.0, 1.0, 0.005, 30, 1);
	glTranslatef(0.0, 0.0, 0.005);
	gluDisk(quad, 0.0, 1.0, 30, 5);
	glPopMatrix();
	glDisable(GL_CLIP_PLANE0);
	SetMaterial(&RUBBER);
	glTranslatef(0.0, -0.11, 0.003);
	RenderFcn[SolidState](0.006);
// chain
	glRotatef(15.0, 1.0, 0.0, 0.0);
	glBegin(GL_LINES);
	glVertex3f(0.0, -0.002, 0.0); glVertex3f(0.0, -0.002, 0.06);
	glVertex3f(0.002, -0.002, 0.0); glVertex3f(0.002, -0.002, 0.06);
	glVertex3f(-0.002, -0.002, 0.0); glVertex3f(-0.002, -0.002, 0.06);
	for(i = 0; i<9; i++){
		glVertex3f(0.002, -0.002, chain); glVertex3f(-0.002, -0.002, chain);
		chain += 0.006;
	}
	glEnd();
	glPopMatrix();
// pedal 2
	glPushMatrix();
	glTranslatef(0.06 +0.5*pedal->E2, 0.0, 0.0);
	glRotatef(pedal->T2, 0.0, 0.0, 1.0);
	glTranslatef(0.04, 0.01, 0.0);
// base
	SetMaterial(&RUBBER);
	glPushMatrix();
	glClipPlane(GL_CLIP_PLANE0, ClipY);
	glEnable(GL_CLIP_PLANE0);
	glScalef(0.05, 0.22, 1.0);
	gluCylinder(quad, 1.0, 1.0, 0.005, 40, 1);
	glTranslatef(0.0, 0.0, 0.005);
	gluDisk(quad, 0.0, 1.0, 40, 3);
	glDisable(GL_CLIP_PLANE0);
	glPopMatrix();
	SetMaterial(&STEEL);
	glPushMatrix();
	glTranslatef(0.0, -0.01, 0.005);
	glScalef(0.11, 0.02, 0.01);
	RenderFcn[SolidState](1.0);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(0.0, 0.16, 0.005);
	glRotatef(90.0, 0.0, 1.0, 0.0);
	glTranslatef(0.0, 0.0, -0.035);
	gluCylinder(quad, 0.0, 0.005, 0.01, 10, 2);
	glTranslatef(0.0, 0.0, 0.01);
	gluCylinder(quad, 0.005, 0.005, 0.05, 10, 4);
	glTranslatef(0.0, 0.0, 0.05);
	gluCylinder(quad, 0.005, 0.0, 0.01, 10, 2);
	glPopMatrix();
// arch
	glPushMatrix();
	glTranslatef(-0.04, -0.01, 0.01);
	gluCylinder(quad, 0.005, 0.005, 0.11, 8, 4);
	glTranslatef(0.08, 0.0, 0.0);
	gluCylinder(quad, 0.005, 0.005, 0.11, 8, 4);
	glTranslatef(0.009, 0.0, 0.0);
	gluCylinder(quad, 0.005, 0.005, 0.01, 6, 1);
	glTranslatef(0.0, 0.0, 0.01);
	gluDisk(quad, 0.0, 0.005, 6, 1);
	SetMaterial(&RUBBER);
	glBegin(GL_LINES);
	glVertex3f(0.0, 0.0, 0.0); glVertex3f(0.0, 0.0, 0.1);
	glEnd();
	glTranslatef(0.0, 0.0, 0.04);
	gluCylinder(quad, 0.003, 0.003, 0.02, 8, 2);
	glPopMatrix();
	glPushMatrix();
	SetMaterial(&STEEL);
	glTranslatef(0.0, -0.01, 0.12);
	glRotatef(90.0, 0.0, 1.0, 0.0);
	glTranslatef(0.0, 0.0, -0.05);
	gluDisk(quad, 0.0, 0.01, 10, 2);
	gluCylinder(quad, 0.01, 0.01, 0.015, 10, 2);
	glTranslatef(0.0, 0.0, 0.015);
	gluDisk(quad, 0.005, 0.01, 10, 2);
	gluCylinder(quad, 0.005, 0.005, 0.03, 10, 5);
	glTranslatef(0.0, 0.0, 0.03);
	gluDisk(quad, 0.005, 0.015, 20, 2);
	SetMaterial(&RUBBER);
	gluCylinder(quad, 0.015, 0.015, 0.01, 20, 1);
	glTranslatef(0.0, 0.0, 0.01);
	SetMaterial(&STEEL);
	gluDisk(quad, 0.005, 0.015, 20, 2);
	gluCylinder(quad, 0.005, 0.005, 0.03, 10, 5);
	glTranslatef(0.0, 0.0, 0.03);
	gluDisk(quad, 0.005, 0.01, 10, 2);
	gluCylinder(quad, 0.01, 0.01, 0.02, 10, 2);
	glTranslatef(0.0, 0.0, 0.02);
	gluDisk(quad, 0.0, 0.01, 10, 2);
	glPopMatrix();
// pedal	
	SetMaterial(pedal->color);
	glTranslatef(0.0, 0.16, 0.005);
	glRotatef(-15.0, 1.0, 0.0, 0.0);
	glClipPlane(GL_CLIP_PLANE0, ClipPedal);
	glEnable(GL_CLIP_PLANE0);
	glTranslatef(0.0, -0.08, 0.0);
	glPushMatrix();
	glScalef(0.04, 0.11, 1.0);
	gluDisk(quad, 0.0, 1.0, 30, 5);
	gluCylinder(quad, 1.0, 1.0, 0.005, 30, 1);
	glTranslatef(0.0, 0.0, 0.005);
	gluDisk(quad, 0.0, 1.0, 30, 5);
	glPopMatrix();
	glDisable(GL_CLIP_PLANE0);
	SetMaterial(&RUBBER);
	glTranslatef(0.0, -0.11, 0.003);
	RenderFcn[SolidState](0.006);
// chain
	chain = 0.005;
	glRotatef(15.0, 1.0, 0.0, 0.0);
	glBegin(GL_LINES);
	glVertex3f(0.0, -0.002, 0.0); glVertex3f(0.0, -0.002, 0.06);
	glVertex3f(0.002, -0.002, 0.0); glVertex3f(0.002, -0.002, 0.06);
	glVertex3f(-0.002, -0.002, 0.0); glVertex3f(-0.002, -0.002, 0.06);
	for(i = 0; i<9; i++){
		glVertex3f(0.002, -0.002, chain); glVertex3f(-0.002, -0.002, chain);
		chain += 0.006;
	}
	glEnd();
	glPopMatrix();
	glPopMatrix();
}

void DrawDrumThrone(drum* throne){
	GLUquadricObj* quad = gluNewQuadric();
	int i;
	GLfloat angle = throne->R1 - 45.0;
	gluQuadricDrawStyle(quad, DRAWMODE);
// shaft
	glPushMatrix();
	glTranslatef(throne->x, throne->y, 0.0);
	glPushMatrix();
	glTranslatef(0.0, 0.0, 0.03);
	SetMaterial(&STEEL);
	gluCylinder(quad, 0.01, 0.01, 0.2, 10, 1);
	glTranslatef(0.0, 0.0, 0.2);
	SetMaterial(throne->color);
	gluCylinder(quad, 0.01, 0.02, 0.03, 10, 2);
	glTranslatef(0.0, 0.0, 0.03);
	gluDisk(quad, 0.01, 0.02, 10, 1);
	SetMaterial(&STEEL);
	gluCylinder(quad, 0.01, 0.01, throne->E1+0.045, 10, 3);
	glTranslatef(0.0, 0.0, throne->E1+0.045);
	gluDisk(quad, 0.0, 0.13, 50, 3);
	SetMaterial(&RUBBER);
	gluCylinder(quad, 0.13, 0.13, 0.07, 50, 3);
	glTranslatef(0.0, 0.0, 0.07);
	glPushMatrix();
	glClipPlane(GL_CLIP_PLANE0, ClipZ);
	glEnable(GL_CLIP_PLANE0);
	glScalef(0.13, 0.13, 0.015);
	gluSphere(quad, 1.0, 50, 10);
	glDisable(GL_CLIP_PLANE0);
	glPopMatrix();

	glPopMatrix();
// legs
	SetMaterial(&STEEL);
	for(i = 0; i < 3; i++){
		glPushMatrix();
		glRotatef(angle, 0.0, 0.0, 1.0);
		glTranslatef(0.06, 0.0, 0.07);
		glRotatef(-45.0, 0.0, 1.0, 0.0);
		glScalef(0.015, 0.005, 0.17);
		RenderFcn[SolidState](1.0);
		glPopMatrix();
		angle += 120.0;
	}
	for(i = 0; i < 3; i++){
		glPushMatrix();
		glRotatef(angle, 0.0, 0.0, 1.0);
		glTranslatef(0.02, 0.0, 0.06);
		glRotatef(45.0, 0.0, 1.0, 0.0);
		glScalef(0.01, 0.005, 0.064);
		RenderFcn[SolidState](1.0);
		glPopMatrix();
		angle += 120.0;
	}
	SetMaterial(&RUBBER);
	for(i = 0; i < 3; i++){
		glPushMatrix();
		glRotatef(angle, 0.0, 0.0, 1.0);
		glTranslatef(0.12, 0.0, 0.01);
		glMultMatrixd(Shear);
		glScalef(0.025, 0.01, 0.02);
		RenderFcn[SolidState](1.0);
		glPopMatrix();
		angle += 120.0;
	}
	glPopMatrix();
}

// ----- movement functions -----
void MoveFloor(int key){
	switch(key){
	case GLUT_KEY_UP:
		if(drumlist[ACTIVE_DRUM]->y > -3.0){drumlist[ACTIVE_DRUM]->y -= 0.025;} break;
	case GLUT_KEY_DOWN:
		if(drumlist[ACTIVE_DRUM]->y < 3.0){drumlist[ACTIVE_DRUM]->y += 0.025;} break;
	case GLUT_KEY_LEFT:
		if(drumlist[ACTIVE_DRUM]->x < 3.0){drumlist[ACTIVE_DRUM]->x += 0.025;} break;
	case GLUT_KEY_RIGHT:
		if(drumlist[ACTIVE_DRUM]->x > -3.0){drumlist[ACTIVE_DRUM]->x -=0.025;} break;
	default: break;
	}
}

void MoveRack(int key){
	switch(key){
	case GLUT_KEY_UP:
	case GLUT_KEY_RIGHT:
		if(drumlist[ACTIVE_DRUM]->x > -69.0){drumlist[ACTIVE_DRUM]->x -= 1.0;} break;
	case GLUT_KEY_DOWN:
	case GLUT_KEY_LEFT:
		if(drumlist[ACTIVE_DRUM]->x < 69.0){drumlist[ACTIVE_DRUM]->x += 1.0;} break;
	default: break;
	}
}

void Rotate(int key){
	switch(key){
	case GLUT_KEY_RIGHT:
		drumlist[ACTIVE_DRUM]->R1 += 2.5;
		if(drumlist[ACTIVE_DRUM]->R1 > 360.0){drumlist[ACTIVE_DRUM]->R1 -= 360.0;}
		break;
	case GLUT_KEY_LEFT:
		drumlist[ACTIVE_DRUM]->R1 -= 2.5;
		if(drumlist[ACTIVE_DRUM]->R1 < -360.0){drumlist[ACTIVE_DRUM]->R1 += 360.0;}
		break;
	case GLUT_KEY_UP:
		drumlist[ACTIVE_DRUM]->R2 += 2.5;
		if(drumlist[ACTIVE_DRUM]->R2 > 360.0){drumlist[ACTIVE_DRUM]->R2 -= 360.0;}
		break;
	case GLUT_KEY_DOWN:
		drumlist[ACTIVE_DRUM]->R2 -= 2.5;
		if(drumlist[ACTIVE_DRUM]->R2 < -360.0){drumlist[ACTIVE_DRUM]->R2 += 360.0;}
		break;
	default: break;
	}
}

void ExtendSnare(int key){
	switch(key){
	case GLUT_KEY_UP:
	case GLUT_KEY_RIGHT:
		if(drumlist[ACTIVE_DRUM]->E1 < 0.10){drumlist[ACTIVE_DRUM]->E1 += 0.01;}
		break;
	case GLUT_KEY_DOWN:
	case GLUT_KEY_LEFT:
		if(drumlist[ACTIVE_DRUM]->E1 > 0.01){drumlist[ACTIVE_DRUM]->E1 -= 0.01;}
		break;
	default: break;
	}
}

void ExtendBoom(int key){
	switch(key){
	case GLUT_KEY_RIGHT:
		if(drumlist[ACTIVE_DRUM]->E1 < 0.24){drumlist[ACTIVE_DRUM]->E1 += 0.01;}
		break;
	case GLUT_KEY_LEFT:
		if(drumlist[ACTIVE_DRUM]->E1 > -0.24){drumlist[ACTIVE_DRUM]->E1 -= 0.01;}
		break;
	case GLUT_KEY_UP:
		if(drumlist[ACTIVE_DRUM]->E2 < 0.43){drumlist[ACTIVE_DRUM]->E2 += 0.01;}
		break;
	case GLUT_KEY_DOWN:
		if(drumlist[ACTIVE_DRUM]->E2 > 0.01){drumlist[ACTIVE_DRUM]->E2 -= 0.01;}
		break;
	default: break;
	}
}

void Tilt(int key){
	switch(key){
	case GLUT_KEY_RIGHT:
		if(drumlist[ACTIVE_DRUM]->T1 < 90.0){drumlist[ACTIVE_DRUM]->T1 += 2.0;}
		break;
	case GLUT_KEY_LEFT:
		if(drumlist[ACTIVE_DRUM]->T1 > -90.0){drumlist[ACTIVE_DRUM]->T1 -= 2.0;}
		break;
	case GLUT_KEY_UP:
		if(drumlist[ACTIVE_DRUM]->T2 > -90.0){drumlist[ACTIVE_DRUM]->T2 -= 2.0;}
		break;
	case GLUT_KEY_DOWN:
		if(drumlist[ACTIVE_DRUM]->T2 < 90.0){drumlist[ACTIVE_DRUM]->T2 += 2.0;}
		break;
	default: break;
	}
}

/* --------------------------------------------------
				MENU CALLBACK FUNCTIONS
   -------------------------------------------------- */

char** getActiveMenu(){
	switch(CurrentMenu){
	case defaultmenu: return DefaultMenu; break;
	case FILE_MENU: return FileMenu; break;
	case DRUM_MENU: return DrumMenu; break;
	case CYMBAL_MENU: return CymbalMenu; break;
	case HARDWARE_MENU: return HardwareMenu; break;
	case OPTION_MENU: return OptionsMenu; break;
	case FLOORCYM_MENU: return FloorCymbalMenu; break;
	case RACKCYM_MENU: return RackCymbalMenu; break;
	case HIHAT_MENU: return HiHatMenu; break;
	case BASSDRUM_MENU: return BassDrumMenu; break;
	case FLOORTOM_MENU: return FloorTomMenu; break;
	case RACKTOM_MENU: return RackTomMenu; break;
	case SNARE_MENU: return SnareDrumMenu; break;
	case CRASH_MENU: return CrashMenu; break;
	case RIDE_MENU: return RideMenu; break;
	case CHINA_MENU: return ChinaMenu; break;
	case SPLASH_MENU: return SplashMenu; break;
	case BELL_MENU: return BellMenu; break;
	case COLORS_MENU: return ColorMenu; break;
	case TEXTURE_MENU: return TextureMenu; break;
	default: return DefaultMenu; break;
	}
}

void file_callback(int item){
	switch(item){
	case 1: reset_studio(); break;
	case 2: loadfile(); CurrentMenu = defaultmenu; break;
	case 3: savefile(); CurrentMenu = defaultmenu; break;
	case 4: exit(0); break;
	default: break;
	}
}

void drum_callback(int item){
	switch(item){
	case 1: CurrentMenu = BASSDRUM_MENU; break;
	case 2: CurrentMenu = FLOORTOM_MENU; break;
	case 3: CurrentMenu = RACKTOM_MENU; break;
	case 4: CurrentMenu = SNARE_MENU; break;
	default: break;
	}
}
void cymbal_callback(int item){
	switch(item){
	case 1: CurrentMenu = FLOORCYM_MENU; break;
	case 2: CurrentMenu = HIHAT_MENU; break;
	case 3: CurrentMenu = RACKCYM_MENU; break;
	default: break;
	}
}

void hardware_callback(int item){
	switch(item){
	case 1: NewCymbalStand(); break;
	case 2: NewHiHatStand(); break;
	case 3: NewSnareStand(); break;
	case 4: NewRackMount(); break;
	case 5: NewBassPedal(); break;
	case 6: NewDoublePedal(); break;
	case 7: NewDrumThrone(); break;
	default: break;
	}
}

void option_callback(int item){
	switch(item){
	case 1: CurrentMenu = COLORS_MENU; break;
	case 2: CurrentMenu = TEXTURE_MENU; break;
	case 3:
		if(STATUS_WINDOW){
			glutDestroyWindow(studio);
			studio = glutCreateSubWindow(window, 215, 55, 680, 625);
			glutReshapeFunc(reshape_proj);
			glutDisplayFunc(studio_display);
			glutMouseFunc(studio_mouse);
			glutMotionFunc(studio_move);
			glutKeyboardFunc(keyboard);
			glutSpecialFunc(function_keys);
			STATUS_WINDOW = 0;
		}
		else{
			glutDestroyWindow(studio);
			studio = glutCreateSubWindow(window, 215, 55, 680, 525);
			glutReshapeFunc(reshape_proj);
			glutDisplayFunc(studio_display);
			glutMouseFunc(studio_mouse);
			glutMotionFunc(studio_move);
			glutKeyboardFunc(keyboard);
			glutSpecialFunc(function_keys);
			STATUS_WINDOW = 1;
		}
		break;
	case 4:
		if(SolidState){
			SolidState = 0; DRAWMODE = GLU_FILL;}
		else{
			SolidState = 1; DRAWMODE = GLU_LINE;}
		break;
	case 5: glutDestroyWindow(studio);
			studio = glutCreateSubWindow(window, 215, 55, 680, 625);
			glutReshapeFunc(reshape_proj);
			glutDisplayFunc(studio_display);
			glutMouseFunc(studio_mouse);
			glutMotionFunc(studio_move);
			glutKeyboardFunc(keyboard);
			glutSpecialFunc(function_keys);
			reset_studio();
			break;
	default: break;
	}
}

void floorcym_callback(int item){
	CYMTAG = 0;
	switch(item){
	case 1: CurrentMenu = CRASH_MENU; break;
	case 2: CurrentMenu = RIDE_MENU; break;
	case 3: CurrentMenu = CHINA_MENU; break;
	case 8: CurrentMenu = CYMBAL_MENU; break;
	default: break;
	}
}

void rackcym_callback(int item){
	CYMTAG = 1;
	switch(item){
	case 1: CurrentMenu = CRASH_MENU; break;
	case 2: CurrentMenu = RIDE_MENU; break;
	case 3: CurrentMenu = CHINA_MENU; break;
	case 4: CurrentMenu = SPLASH_MENU; break;
	case 5: CurrentMenu = BELL_MENU; break;
	case 8: CurrentMenu = CYMBAL_MENU; break;
	default: break;
	}
}

void color_callback(int item){
	if(item > 0 && item < 8){ ACTIVECOLOR = COLOR_LIST[item-1]; ACTIVETEXTURE = &NONE;}
	CurrentMenu = OPTION_MENU;
}

void texture_callback(int item){
	if(item > 0 && item < 8){
		ACTIVETEXTURE = TEXTURE_LIST[item];
		switch(item){
		case 1: ACTIVECOLOR = COLOR_LIST[0]; break;
		case 2:	ACTIVECOLOR = COLOR_LIST[3]; break;
		case 3:	ACTIVECOLOR = COLOR_LIST[1]; break;
		case 4:	ACTIVECOLOR = COLOR_LIST[4]; break;
		case 5:	ACTIVECOLOR = COLOR_LIST[5]; break;
		case 6:	ACTIVECOLOR = COLOR_LIST[2]; break;
		case 7:	ACTIVECOLOR = COLOR_LIST[6]; break;
		default: break;
		}
	}
	CurrentMenu = OPTION_MENU;
}

/* --------------------------------------------------
			DISPLAY and RESHAPE FUNCTIONS
   -------------------------------------------------- */

// ----- utility functions -----
void init(void){
	glClearColor(0.5, 0.5, 0.5, 0.0);
	glShadeModel(GL_SMOOTH);
	floor = LoadTexture("Textures/floor.ppm");
	logo = LoadTexture("Textures/logo.ppm");
	button = LoadTexture("Textures/button.ppm");
	BlackMarble = LoadTexture("Textures/BlackMarble.ppm");
	Gold = LoadTexture("Textures/Gold.ppm");
	Oceana = LoadTexture("Textures/Oceana.ppm");
	Royal = LoadTexture("Textures/Royal.ppm");
	Velvet = LoadTexture("Textures/Velvet.ppm");
	Verdant = LoadTexture("Textures/Verdant.ppm");
	WhiteMarble = LoadTexture("Textures/WhiteMarble.ppm");
}

void drawstr2d(GLuint x, GLuint y, char* text, void* style){
	int i, textlen = strlen(text);
	glRasterPos2i(x, y);
	for (i = 0; i<=textlen; i++){
		glutBitmapCharacter(style, text[i]);
	}
}

void redisplay_all(void){
	glutSetWindow(window); glutPostRedisplay();
	glutSetWindow(command); glutPostRedisplay();
	glutSetWindow(menu); glutPostRedisplay();
	glutSetWindow(objectlist); glutPostRedisplay();
	glutSetWindow(studio); glutPostRedisplay();	
}

void init_studio(void){
	int i;
	GLfloat angle = 35.5;
	GLUquadricObj* quad = gluNewQuadric();
	gluQuadricDrawStyle(quad, GLU_FILL);
	
	// --- floor ---
	if(floor != NULL){
		glEnable(GL_TEXTURE_2D);
		glEnable(GL_TEXTURE_GEN_S);
		glEnable(GL_TEXTURE_GEN_T);	
		glTexImage2D(GL_TEXTURE_2D,0, GL_RGB, 512, 512, 0, GL_RGB, GL_UNSIGNED_BYTE, floor);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		glTexGenfv(GL_S, GL_OBJECT_PLANE, TEX_S_PLANE);
		glTexGenfv(GL_T, GL_OBJECT_PLANE, TEX_T_PLANE);
		glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
		glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
		glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP);
		glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP);
		glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
		SetMaterial(&GLOSS);
		glPushMatrix();
		glTranslatef(0.0, 0.0, -0.01);
		glScalef(7.5, 7.5, 0.02);
		glutSolidCube(1.0);
		glPopMatrix();
		glDisable(GL_TEXTURE_GEN_S);
		glDisable(GL_TEXTURE_GEN_T);	
		glDisable(GL_TEXTURE_2D);
	}
	else{
		SetMaterial(&GLOSS);
		glPushMatrix();
		glTranslatef(0.0, 0.0, -0.01);
		glScalef(7.5, 7.5, 0.02);
		glutSolidCube(1.0);
		glPopMatrix();
	}	
	// --- rack ---
	SetMaterial(&STEEL);
	glClipPlane(GL_CLIP_PLANE0, ClipRack);
	glEnable(GL_CLIP_PLANE0);
	glPushMatrix();
	glTranslatef(0.0, 0.5, RACKHEIGHT);
	glutSolidTorus(0.02, 1.5, 20, 100);
	glDisable(GL_CLIP_PLANE0);

	SetMaterial(&RUBBER);
	for(i = 0; i < 2; i++){
		glPushMatrix();
		glRotatef(angle, 0.0, 0.0, 1.0); glTranslatef(0.0, -1.5, 0.0);
		glutSolidSphere(0.03, 10, 10);
		glPopMatrix();
		glPushMatrix();
		glRotatef(-angle, 0.0, 0.0, 1.0); glTranslatef(0.0, -1.5, 0.0);
		glutSolidSphere(0.03, 10, 10);
		glPopMatrix();
		angle += 35.0;
	}
	SetMaterial(&STEEL);
	angle = 35.5;
	for(i = 0; i < 2; i++){
		glPushMatrix();
		glTranslatef(0.0, 0.0, -RACKHEIGHT + 0.03);
		glRotatef(angle, 0.0, 0.0, 1.0); glTranslatef(0.0, -1.5, 0.0);
		gluCylinder(quad, 0.02, 0.02, RACKHEIGHT-0.03, 20, 5);
		glPopMatrix();
		glPushMatrix();
		glTranslatef(0.0, 0.0, -RACKHEIGHT + 0.03);
		glRotatef(-angle, 0.0, 0.0, 1.0); glTranslatef(0.0, -1.5, 0.0);
		gluCylinder(quad, 0.02, 0.02, RACKHEIGHT-0.03, 20, 5);
		glPopMatrix();
		angle += 35.0;
	}
	SetMaterial(&RUBBER);
	angle = 35.5;
	for(i = 0; i < 2; i++){
		glPushMatrix();
		glTranslatef(0.0, 0.0, -RACKHEIGHT);
		glRotatef(angle, 0.0, 0.0, 1.0); glTranslatef(0.0, -1.5, 0.0);
		gluCylinder(quad, 0.04, 0.02, 0.03, 20, 5);
		glPopMatrix();
		glPushMatrix();
		glTranslatef(0.0, 0.0, -RACKHEIGHT);
		glRotatef(-angle, 0.0, 0.0, 1.0); glTranslatef(0.0, -1.5, 0.0);
		gluCylinder(quad, 0.04, 0.02, 0.03, 20, 5);
		glPopMatrix();
		angle += 35.0;
	}
	glPopMatrix();
}

void reset_studio(void){
	int i;

	for(i = 0; i < NUMDRUMS; i++){
		free(drumlist[i]);
		drumlist[i] = NULL; }
	NUMDRUMS = 0;
	ACTIVE_DRUM = -1;
	ACTIVE_CONTROL = 0;
	ACTIVECOLOR = &RED;
	ACTIVETEXTURE = &NONE;
	MainMenuIndex[0]=0;
	MainMenuIndex[1]=0;
	MainMenuIndex[2]=0;
	MainMenuIndex[3]=0;
	MainMenuIndex[4]=0; 
	CurrentMenu = defaultmenu;
	CYMTAG = 0;
	CAM.lift = 0.0;
	CAM.mode = 0;
	CAM.mouse_x = 0;
	CAM.mouse_y = 0;
	CAM.spin = 0.0;
	CAM.x = 0.0;
	CAM.y = 0.0;
	CAM.zoom = 1.0;
	STATUS_WINDOW = 0;
	SKY_COLOR = 0.0;
	DRAWMODE = GLU_FILL;
	SolidState = 0;
	RACKHEIGHT = 0.84;
}

// ----- reshape functions -----
void reshape_proj(int w, int h){
	glViewport(0, 0, (GLsizei) w, (GLsizei) h); 
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, (GLfloat) w/(GLfloat) h, 0.25, 40.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void reshape_ortho(int w, int h){
	glViewport(0, 0, (GLsizei) w, (GLsizei) h); 
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, w, h, 0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	redisplay_all();
}

// ----- display functions -----
void main_display(void){
	char* s = (char *) malloc(15*sizeof(char));

	glClearColor(0.525, 0.525, 0.525, 0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glColor3f(0.0, 0.0, 0.0);
	drawstr2d(780, 695, "(c) Gregory Berlinrut 2004", GLUT_BITMAP_TIMES_ROMAN_10);

	if(STATUS_WINDOW){
		glRectf(215.0, 590.0, 895.0, 680.0);
		
		glPushMatrix();
		glTranslatef(215.0, 590.0, 0.0);
		glColor3f(0.1, 1.0, 0.1);
		drawstr2d(240, 20, "Drum Studio [beta]", GLUT_BITMAP_9_BY_15);
		// add actual file name here
 
		drawstr2d(5, 40, "Camera > SPIN:         LIFT:         ZOOM:       X:       Y:", GLUT_BITMAP_9_BY_15);
		glColor3f(0.1, 1.0, 1.0);
		sprintf(s, "%.1f", CAM.spin); drawstr2d(140, 40, s, GLUT_BITMAP_9_BY_15);
		sprintf(s, "%.2f", CAM.lift); drawstr2d(266, 40, s, GLUT_BITMAP_9_BY_15);
		sprintf(s, "%.1fx", (1.0 - CAM.zoom + 1.0)); drawstr2d(392, 40, s, GLUT_BITMAP_9_BY_15);
		sprintf(s, "%.1f", -CAM.x); drawstr2d(473, 40, s, GLUT_BITMAP_9_BY_15);
		sprintf(s, "%.1f", CAM.y); drawstr2d(554, 40, s, GLUT_BITMAP_9_BY_15);

		glColor3f(0.1, 1.0, 0.1);
		drawstr2d(5, 60, "Active Color:                Active Pattern:                [F5]: Change", GLUT_BITMAP_9_BY_15);
		glColor3f(0.1, 1.0, 1.0);
		sprintf(s, "%s", ACTIVECOLOR->name); drawstr2d(135, 60, s, GLUT_BITMAP_9_BY_15);
		sprintf(s, "%s", ACTIVETEXTURE->name); drawstr2d(415, 60, s, GLUT_BITMAP_9_BY_15);

		glColor3f(0.1, 1.0, 0.1);
		if(ACTIVE_DRUM >= 0){
			switch(ACTIVE_CONTROL){
			case 0:
				drawstr2d(5, 80, "Active Controls > [Movement]  X:        Y:              [F1] F2 F3 F4", GLUT_BITMAP_9_BY_15);
				glColor3f(0.1, 1.0, 1.0);
				sprintf(s, "%.2f", drumlist[ACTIVE_DRUM]->x); drawstr2d(302, 80, s, GLUT_BITMAP_9_BY_15);
				sprintf(s, "%.2f", drumlist[ACTIVE_DRUM]->y); drawstr2d(392, 80, s, GLUT_BITMAP_9_BY_15);
				break;
			case 1:
				drawstr2d(5, 80, "Active Controls > [Rotation]  R1:        R2:            F1 [F2] F3 F4", GLUT_BITMAP_9_BY_15);
				glColor3f(0.1, 1.0, 1.0);
				sprintf(s, "%.2f", drumlist[ACTIVE_DRUM]->R1); drawstr2d(311, 80, s, GLUT_BITMAP_9_BY_15);
				sprintf(s, "%.2f", drumlist[ACTIVE_DRUM]->R2); drawstr2d(409, 80, s, GLUT_BITMAP_9_BY_15);
				break;
			case 2:
				drawstr2d(5, 80, "Active Controls > [Extension]  E1:        E2:           F1 F2 [F3] F4", GLUT_BITMAP_9_BY_15);
				glColor3f(0.1, 1.0, 1.0);
				sprintf(s, "%.2f", drumlist[ACTIVE_DRUM]->E1); drawstr2d(320, 80, s, GLUT_BITMAP_9_BY_15);
				sprintf(s, "%.2f", drumlist[ACTIVE_DRUM]->E2); drawstr2d(418, 80, s, GLUT_BITMAP_9_BY_15);
				break;
			case 3:
				drawstr2d(5, 80, "Active Controls > [Tilt]  T1:        T2:                F1 F2 F3 [F4]", GLUT_BITMAP_9_BY_15);
				glColor3f(0.1, 1.0, 1.0);
				sprintf(s, "%.2f", drumlist[ACTIVE_DRUM]->T1); drawstr2d(275, 80, s, GLUT_BITMAP_9_BY_15);
				sprintf(s, "%.2f", drumlist[ACTIVE_DRUM]->T2); drawstr2d(373, 80, s, GLUT_BITMAP_9_BY_15);
				break;
			default: break;
			}
		}
		else { drawstr2d(5, 80, "Active Controls > NONE", GLUT_BITMAP_9_BY_15); }
		
		glPopMatrix();
	}
	glutSwapBuffers();
}

void command_display(void){
	GLUquadricObj* circle = gluNewQuadric();
	int i;
	gluQuadricDrawStyle(circle, DRAWMODE);
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if(logo != NULL && button != NULL){
		glEnable(GL_TEXTURE_2D);
		glTexImage2D(GL_TEXTURE_2D,0, GL_RGB, 256, 64, 0, GL_RGB, GL_UNSIGNED_BYTE, logo);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
		glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP);
		glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP);
		glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
		glBegin(GL_QUADS);
		glTexCoord2f(1.0, 1.0); glVertex3f(2.0, 2.0, 0.0);
		glTexCoord2f(0.0, 1.0); glVertex3f(148.0, 2.0, 0.0);
		glTexCoord2f(0.0, 0.0); glVertex3f(148.0, 38.0, 0.0);
		glTexCoord2f(1.0, 0.0); glVertex3f(2.0, 38.0, 0.0);
		glEnd();
		glPushMatrix();
		glTexImage2D(GL_TEXTURE_2D,0, GL_RGB, 128, 32, 0, GL_RGB, GL_UNSIGNED_BYTE, button);
		glTranslatef(160.0, 4.0, 0.0);
		for(i = 0; i < 5; i++){
			glBegin(GL_QUADS);
			glTexCoord2f(1.0, 1.0); glVertex3f(0.0, 0.0, 0.0);
			glTexCoord2f(0.0, 1.0); glVertex3f(128.0, 0.0, 0.0);
			glTexCoord2f(0.0, 0.0); glVertex3f(128.0, 32.0, 0.0);
			glTexCoord2f(1.0, 0.0); glVertex3f(0.0, 32.0, 0.0);
			glEnd();
			glTranslatef(148.0, 0.0, 0.0);
		}
		glPopMatrix();
		glDisable(GL_TEXTURE_2D);
	}
	else{
		glColor3f(0.3, 0.3, 0.3);
		glBegin(GL_QUADS);
		glVertex3f(2.0, 2.0, 0.0);
		glVertex3f(148.0, 2.0, 0.0);
		glVertex3f(148.0, 38.0, 0.0);
		glVertex3f(2.0, 38.0, 0.0);
		glEnd();
		glPushMatrix();
		glTranslatef(160.0, 4.0, 0.0);
		for(i = 0; i < 5; i++){
			glBegin(GL_QUADS);
			glVertex3f(0.0, 0.0, 0.0);
			glVertex3f(128.0, 0.0, 0.0);
			glVertex3f(128.0, 32.0, 0.0);
			glVertex3f(0.0, 32.0, 0.0);
			glEnd();
			glTranslatef(148.0, 0.0, 0.0);
		}
		glPopMatrix();
		glColor3f(0.6, 0.6, 0.6);
		drawstr2d(25,26,"Drum Studio", GLUT_BITMAP_HELVETICA_18);
	}

	glPushMatrix();
	glTranslatef(158.0, 2.0, 0.0);
	for(i = 0; i<=4; i++){
		if(MainMenuIndex[i]){ glColor3f(0.1, 1.0, 0.1); }
		else{ glColor3f(0.6, 0.6, 0.6); }
		glBegin(GL_LINE_LOOP);
		glVertex3f(0.0, 0.0, 0.0); glVertex3f(132.0, 0.0, 0.0);
		glVertex3f(132.0, 36.0, 0.0); glVertex3f(0.0, 36.0, 0.0);
		glEnd();
		drawstr2d(15,24,CommandMenu[i], GLUT_BITMAP_HELVETICA_18);
		glTranslatef(148.0, 0.0, 0.0);
	}
	glPopMatrix();
	glutSwapBuffers();
}

void menu_display(void){
	GLUquadricObj* circle = gluNewQuadric();
	int i, s = MenuSizes[CurrentMenu];
	char** ActiveMenu = getActiveMenu();
	gluQuadricDrawStyle(circle, DRAWMODE);
	
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glColor3f(0.4, 0.4, 0.4);
	glBegin(GL_LINES);
	glVertex3f(15.0,32.0,0.0); glVertex3f(185.0,32.0,0.0);
	glEnd();
	
	glPushMatrix();
	glTranslatef(15.0,28.0,0.0);
	glColor3f(0.1, 1.0, 1.0);
	drawstr2d(0,0,ActiveMenu[0], GLUT_BITMAP_HELVETICA_18);
	glTranslatef(0.0, 35.0, 0.0);
	glColor3f(0.1,1.0,0.1);

	for(i=1; i<=s; i++){
		drawstr2d(0,0,ActiveMenu[i],GLUT_BITMAP_9_BY_15);
		glTranslatef(0.0, 30.0, 0.0);
	}
	glPopMatrix();
	if(CurrentMenu > 5){
		drawstr2d(120,282,"...BACK",GLUT_BITMAP_9_BY_15);}
	glutSwapBuffers();
}

void objectlist_display(void){	
	int i;
	char *s = (char *) malloc(3*sizeof(char));

	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glColor3f(0.4, 0.4, 0.4);
	glBegin(GL_LINES);
	glVertex3f(5.0, 22.0, 0.0);	glVertex3f(195.0, 23.0, 0.0);
	glEnd();
	glColor3f(0.1, 1.0, 1.0);
	drawstr2d(5, 17, "OBJECT LIST", GLUT_BITMAP_HELVETICA_18);
	
	for(i = 0; i < NUMDRUMS; i++){
		if(ACTIVE_DRUM == i){
			glColor3f(0.1, 1.0, 0.1);
			glBegin(GL_LINE_LOOP); 
			glVertex3f(5.0, 25.0 + 15.0*((GLfloat) i), 0.0);
			glVertex3f(195.0, 25.0 + 15.0*((GLfloat) i), 0.0);
			glVertex3f(195.0, 25.0 + 15.0*((GLfloat) (i+1)), 0.0);
			glVertex3f(5.0, 25.0 + 15.0*((GLfloat) (i+1)), 0.0);
			glEnd();
		}
		else{ glColor3f(0.75, 0.75, 0.75); }
		sprintf(s, "[%c]", 'A'+i);
		drawstr2d(5, 20+15*(i+1), s, GLUT_BITMAP_8_BY_13);
		drawstr2d(35, 20+15*(i+1), drumlist[i]->name, GLUT_BITMAP_8_BY_13);
	}
	glutSwapBuffers();
}

void studio_display(void){
	int i;  
// set up lighting
	glClearColor(0.0+SKY_COLOR, 0.0+SKY_COLOR, 0.0+SKY_COLOR, 0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPushMatrix();
	glTranslatef(CAM.x, CAM.y, -10.0*CAM.zoom);
	glRotatef(CAM.lift, 1.0, 0.0, 0.0);
	glRotatef(CAM.spin+180.0, 0.0, 0.0, 1.0);
	glPushMatrix();
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);
	glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, 1.0);
	glLightModelf(GL_LIGHT_MODEL_COLOR_CONTROL,GL_SEPARATE_SPECULAR_COLOR);
	glLightfv(GL_LIGHT0, GL_AMBIENT, whitelighta);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, whitelightd);
	glLightfv(GL_LIGHT0, GL_SPECULAR, whitelights);
	glLightfv(GL_LIGHT0, GL_POSITION, whitelightp);	
	glEnable(GL_LIGHT0);  
	glEnable(GL_LIGHTING);
	glPopMatrix();
// initialize floor and rack
	init_studio();
// draw all objects
	for(i = 0; i < NUMDRUMS; i++){
		drumlist[i]->Draw(drumlist[i]);}
// disable lighting
	glPopMatrix();
	glDisable(GL_LIGHTING);
	glColor3f(1.0, 1.0, 1.0);
	glutSwapBuffers();
}

/* --------------------------------------------------
			USER INPUT and MOTION FUNCTIONS
   -------------------------------------------------- */

void command_mouse(int button, int state, int x, int y){
	int menuitem = (x+5)/150;
	if(state == GLUT_DOWN){
		switch(menuitem){
		case 1:
			MainMenuIndex[0]=1; MainMenuIndex[1]=0; MainMenuIndex[2]=0;
			MainMenuIndex[3]=0; MainMenuIndex[4]=0; CurrentMenu = FILE_MENU; break;
		case 2:
			MainMenuIndex[0]=0; MainMenuIndex[1]=1;	MainMenuIndex[2]=0; 
			MainMenuIndex[3]=0; MainMenuIndex[4]=0; CurrentMenu = DRUM_MENU; break;
		case 3:
			MainMenuIndex[0]=0; MainMenuIndex[1]=0; MainMenuIndex[2]=1;
			MainMenuIndex[3]=0; MainMenuIndex[4]=0; CurrentMenu = CYMBAL_MENU; break;
		case 4:
			MainMenuIndex[0]=0; MainMenuIndex[1]=0; MainMenuIndex[2]=0;
			MainMenuIndex[3]=1; MainMenuIndex[4]=0; CurrentMenu = HARDWARE_MENU; break;
		case 5:
			MainMenuIndex[0]=0; MainMenuIndex[1]=0; MainMenuIndex[2]=0;
			MainMenuIndex[3]=0;	MainMenuIndex[4]=1;	CurrentMenu = OPTION_MENU; break;
		default: break;
		}
		redisplay_all();
	}
}

void menu_mouse(int button, int state, int x, int y){
	int menuitem = (y-20)/30;
	if(state == GLUT_DOWN){
		CALLBACKS[CurrentMenu-1](menuitem);
		redisplay_all();
	}
}

void studio_mouse(int button, int state, int x, int y){
	
	if(state == GLUT_DOWN){
		if(button == GLUT_RIGHT_BUTTON){ CAM.mode = 1; }
		else { CAM.mode = 0; }
		CAM.mouse_x = x;
		CAM.mouse_y = y;
	}
	if(STATUS_WINDOW){ redisplay_all(); }
	else { glutPostRedisplay(); }
}

void studio_move(int x, int y){	
	if(CAM.mode){
		if(x - CAM.mouse_x > 0){
			if(CAM.x < 3.5){ CAM.x += 0.1; }
		}
		else if(x - CAM.mouse_x < 0){
			if(CAM.x > -3.5){ CAM.x -= 0.1; }
		}
		if(y - CAM.mouse_y < 0.0){
			if(CAM.y < 3.5){ CAM.y += 0.1; }
		}
		else if(y - CAM.mouse_y > 0){
			if(CAM.y > -3.5){ CAM.y -= 0.1; }
		}	
	}
	else {
		if(x - CAM.mouse_x < 0){ CAM.spin -= 2.5; }
		else if(x - CAM.mouse_x > 0){ CAM.spin += 2.5; }
		if(CAM.spin > 360.0){ CAM.spin -= 360.0; }
		else if(CAM.spin < -360.0){ CAM.spin += 360.0; }
		if(y - CAM.mouse_y > 0){
			if(CAM.lift > -90.0){ CAM.lift -= 2.5; }
		}
		else if(y - CAM.mouse_y < 0){
			if(CAM.lift < 0.0){ CAM.lift += 2.5; }
		}
	}
		if(STATUS_WINDOW){ redisplay_all(); }
		else { glutPostRedisplay(); }
}

void objectlist_mouse(int button, int state, int x, int y){
	
	int menuitem = (y-12)/15;
	if(state == GLUT_DOWN){
		if(menuitem > 0 && menuitem <= NUMDRUMS){
			ACTIVE_DRUM = menuitem-1;
		}
		else{ ACTIVE_DRUM = -1; }
	}
	if(STATUS_WINDOW){ 
		glutPostRedisplay();
		glutSetWindow(window); glutPostRedisplay();}
	else { glutPostRedisplay(); }
}

void keyboard(unsigned char key, int x, int y){
	
	switch(key){
	case 127:
		delete_drum(ACTIVE_DRUM); break;
	case '-':
	case '_':
		if(CAM.zoom < 1.5){ CAM.zoom += 0.1; } break;
	case '+':
	case '=':
		if(CAM.zoom > 0.2){ CAM.zoom -= 0.1; } break;
	case '.':
	case '>':
		if(RACKHEIGHT < 0.96){ RACKHEIGHT += 0.02; } break;
	case ',':
	case '<':
		if(RACKHEIGHT > 0.72){ RACKHEIGHT -= 0.02; } break;
	default:
		break;
	}
	redisplay_all();
}

void function_keys(int key, int x, int y){
	switch(key){
	case GLUT_KEY_F1:
	case GLUT_KEY_F2:
	case GLUT_KEY_F3:
	case GLUT_KEY_F4: 
		ACTIVE_CONTROL = key - GLUT_KEY_F1; break;
	case GLUT_KEY_F5:
		if(ACTIVE_DRUM >= 0){
			drumlist[ACTIVE_DRUM]->color = ACTIVECOLOR;
			drumlist[ACTIVE_DRUM]->tex = ACTIVETEXTURE;
			drumlist[ACTIVE_DRUM]->colorindex = ACTIVECOLOR->num;
			drumlist[ACTIVE_DRUM]->texindex = ACTIVETEXTURE->num;} break;
	case GLUT_KEY_UP:
	case GLUT_KEY_DOWN:
	case GLUT_KEY_LEFT:
	case GLUT_KEY_RIGHT: 
		if(ACTIVE_DRUM >= 0){ drumlist[ACTIVE_DRUM]->Move[ACTIVE_CONTROL](key); } break;
	case GLUT_KEY_PAGE_UP:
		if(SKY_COLOR < 1.0){ SKY_COLOR += 0.05;} break;
	case GLUT_KEY_PAGE_DOWN:
		if(SKY_COLOR > 0.0){ SKY_COLOR -= 0.05;} break;
	default: break;
	}

	if(STATUS_WINDOW){ redisplay_all(); }
	else { glutSetWindow(studio); glutPostRedisplay(); }
}

/* --------------------------------------------------
		   FILE READING and WRITING FUNCTIONS
   -------------------------------------------------- */

void savefile(){
	FILE *stream;
	int i;
	stream = fopen( "studiolog000.dsf", "w+" );
	if(stream != NULL){
		fprintf(stream, "Drum_Studio_Kit_Builder_[beta]_Savefile_(DO_NOT_EDIT)\n");
		fprintf(stream, "%d", NUMDRUMS);
		for(i = 0; i < NUMDRUMS; i++){
			fprintf(stream, "\n%d %d %d %d %f %f %f %f %f %f %f %f",
				drumlist[i]->drumtype,
				drumlist[i]->cfcnindex,
				drumlist[i]->colorindex,
				drumlist[i]->texindex,
				drumlist[i]->x,
				drumlist[i]->y,
				drumlist[i]->R1,
				drumlist[i]->R2,
				drumlist[i]->E1,
				drumlist[i]->E2,
				drumlist[i]->T1,
				drumlist[i]->T2);
		}
		fclose(stream);
	}
}

void loadfile(){
	FILE *stream;
	int type, size, color, texture;
	GLfloat x, y, r1, r2, e1, e2, t1, t2;
	int i, items;
	char str[50];
	
	stream = fopen( "studiolog000.dsf", "r" );
	if(stream != NULL){
		reset_studio();
		fscanf(stream, "%s", &str);
		fscanf(stream, "%d", &items);
		for(i = 0; i < items; i++){
			fscanf(stream, "%d %d %d %d %f %f %f %f %f %f %f %f",
				&type, &size, &color, &texture, &x, &y, &r1, &r2, &e1, &e2, &t1, &t2);

			ACTIVECOLOR = COLOR_LIST[color-1];
			ACTIVETEXTURE = TEXTURE_LIST[texture];
			switch(type){
			case 1: NewBassDrum(size); break;
			case 2: NewFloorTom(size); break;
			case 3: NewRackTom(size); break;
			case 4: NewSnareDrum(size); break;
			case 5: NewHiHats(size); break;
			case 6: CYMTAG = 1; NewCrashCymbal(size); break;
			case 7: CYMTAG = 0; NewCrashCymbal(size); break;
			case 8: CYMTAG = 1; NewRideCymbal(size); break;
			case 9: CYMTAG = 0; NewRideCymbal(size); break;
			case 10: CYMTAG = 1; NewChinaCymbal(size); break;
			case 11: CYMTAG = 0; NewChinaCymbal(size); break;
			case 12: NewSplashCymbal(size); break;
			case 13: NewBellCymbal(size); break;
			case 14: NewCymbalStand(); break;
			case 15: NewHiHatStand(); break;
			case 16: NewSnareStand(); break;
			case 17: NewRackMount(); break;
			case 18: NewBassPedal(); break;
			case 19: NewDoublePedal(); break;
			case 20: NewDrumThrone(); break;
			default: break;
			}

			drumlist[ACTIVE_DRUM]->x = x;
			drumlist[ACTIVE_DRUM]->y = y;
			drumlist[ACTIVE_DRUM]->R1 = r1;
			drumlist[ACTIVE_DRUM]->R2 = r2;
			drumlist[ACTIVE_DRUM]->E1 = e1;
			drumlist[ACTIVE_DRUM]->E2 = e2;
			drumlist[ACTIVE_DRUM]->T1 = t1;
			drumlist[ACTIVE_DRUM]->T2 = t2;			
		}
		fclose(stream);
	}
}

/* --------------------------------------------------
					MAIN FUNCTION
   -------------------------------------------------- */

int main(int argc, char** argv){
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGB);
	glutInitWindowSize(900, 700); 
	glutInitWindowPosition (50, 0);
	
	window = glutCreateWindow("DrumStudio Drum Kit Builder [beta]");
	glutReshapeFunc(reshape_ortho);
	glutDisplayFunc(main_display);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(function_keys);
	init();
	
	command = glutCreateSubWindow(window, 5, 5, 890, 40);
	glutReshapeFunc(reshape_ortho);
	glutDisplayFunc(command_display);
	glutMouseFunc(command_mouse);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(function_keys);
	
	menu = glutCreateSubWindow(window, 5, 55, 200, 290); 
	glutReshapeFunc(reshape_ortho);
	glutDisplayFunc(menu_display);
	glutMouseFunc(menu_mouse);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(function_keys);
	
	objectlist = glutCreateSubWindow(window, 5, 355, 200, 325); 
	glutReshapeFunc(reshape_ortho);
	glutDisplayFunc(objectlist_display);
	glutMouseFunc(objectlist_mouse);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(function_keys);
	
	studio = glutCreateSubWindow(window, 215, 55, 680, 625);
	glutReshapeFunc(reshape_proj);
	glutDisplayFunc(studio_display);
	glutMouseFunc(studio_mouse);
	glutMotionFunc(studio_move);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(function_keys);
	
	redisplay_all();
	glutMainLoop();
	return 0;
}